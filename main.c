#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_image.h>

#include "struct.h"
#include "libqueue.h"
#include "states.h"
#include "init.h"
#include "game.h"
#include "aux.h"

bool bccaux = false;

int main()
{
    srand(time(NULL));

    // Inicializa Allegro, teclado e mouse
	init_test(al_init(), "Allegro");
    init_test(al_install_keyboard(), "teclado");
    init_test(al_install_mouse(), "mouse");

    // Inicializa timer
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60.0);
    init_test(timer, "timer");

    // Inicializa fila de eventos
    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    init_test(queue, "fila");

    // Inicializa display
    ALLEGRO_DISPLAY* disp = display_init();
    al_hide_mouse_cursor(disp);

    // Inicializa fontes
    fonts_t* fonts = font_init();

    // Inicializa sprites
    sprites_t* sprites = sprite_init();

    // Inicializa áudios
    audios_t* audios = audio_init();

    // Registra as fontes de eventos para a fila
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_mouse_event_source());
    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_timer_event_source(timer));

    // Controla se uma das condições para o fim do programa foi atendida
    bool done = false;

    // Controla a hora de trocar o display
    bool redraw = true;

    // Controla a velocidade das bolas
    bool speedup = false;

    // Variáveis de easter egg
    int last_key_pressed = 0;
    int bcctimer = 0;

    // Declara variável de eventos
    ALLEGRO_EVENT* event = malloc(sizeof(ALLEGRO_EVENT));

    // Variáveis para posição do mouse
    int score = 0;
    int highscore = 0;
    float mx = 0, my = 0;
    state_t state = BEGIN;

    // Variáveis para "tabuleiro" do jogo e fila de bolas
    square_t*** board = board_alloc();
    queue_t* ball_queue;

    // Abre o arquivo de highscores, no modo leitura e escrita (concatenação)
    FILE* score_log = fopen("score.log", "a+");

    // Inicia o timer do loop principal
    al_start_timer(timer);

    // Loop principal
    while (1)
    {
        // Lê o próximo evento da fila
        al_wait_for_event(queue, event);

        switch(event->type)
        {
            // Caso o evento seja um frame do timer
            case ALLEGRO_EVENT_TIMER: 

                // Se o estado for BALL_SHOOT, as posições das bolas são atualizadas
                if (state == BALL_SHOOT)
                {
                    ball_update(board, ball_queue, audios);
                    // Se todas as bolas estiverem fora da tela, muda o estado para BALL_AIM e atualiza o tabuleiro
                    if (all_balls_off(ball_queue))
                    {
                        state = BALL_AIM;
                        speedup = false;
                        board_update(board, ball_queue, &state, score_log, &score, &highscore);
                    }
                }

                if (bcctimer > 0)
                    bcctimer--;
                else if (bcctimer == 0)
                    bccaux = false;

                // Atualiza a variável para redesenhar a tela
                redraw = true;
                break;

            // Caso o evento seja uma tecla apertada
            case ALLEGRO_EVENT_KEY_DOWN:

                // Se estiver no menu inicial e a tecla apertada for H ou F1, muda para o menu de ajuda
                if ((event->keyboard.keycode == ALLEGRO_KEY_H || event->keyboard.keycode == ALLEGRO_KEY_F1) && state == BEGIN)
                    state = HELP;

                // Se estiver no estado BALL_SHOOT e a tecla apertada for S, aumenta a velocidade das bolas
                if (event->keyboard.keycode == ALLEGRO_KEY_S && state == BALL_SHOOT)
                {
                    speed_increase(ball_queue, speedup);
                    speedup = !speedup;
                }

                // Testa se as teclas B C C foram digitadas em sequência
                if (!bccaux)
                {
                    if (event->keyboard.keycode == ALLEGRO_KEY_B)
                        bcctimer = 60;
                    if (event->keyboard.keycode == ALLEGRO_KEY_C)
                    {
                        if (bcctimer > 0)
                        {
                            if (last_key_pressed == ALLEGRO_KEY_B)
                                bcctimer = 60;
                            else if (last_key_pressed == ALLEGRO_KEY_C)
                            {
                                bccaux = true;
                                bcctimer = 60 * 6;
                            }
                            else
                                bcctimer = 0;
                        }
                    }
                }

                // Se a tecla apertada for ESC...
                if (event->keyboard.keycode == ALLEGRO_KEY_ESCAPE)
                {
                    // ... e estiver no menu inicial, encerra o jogo
                    if (state == BEGIN)
                        done = true;

                    // ... e não estiver no menu inicial, vai para o menu inicial
                    else
                    {
                        // Se estiver jogando, destroi a fila de bolas
                        if (state == BALL_AIM || state == BALL_SHOOT)
                            queue_destroy(ball_queue);
                        state = BEGIN;
                    }
                }

                last_key_pressed = event->keyboard.keycode;
                break;

            // Caso o mouse seja movido, atualiza suas posições
            case ALLEGRO_EVENT_MOUSE_AXES:
                mx = event->mouse.x;
                my = event->mouse.y;
                break;

            // Caso o botão primário do mouse seja apertado
            case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:

                // Se estiver no estado inicial, inicia o jogo
                if (state == BEGIN)
                {
                    state = BALL_AIM;
                    speedup = false;
                    score = 0;
                    ball_queue = queue_init();
                    board_init(board);
                    board_update(board, ball_queue, &state, score_log, &score, &highscore);
                }

                // Se estiver no estado de mira, inicia as posições das bolas e muda o estado para BALL_SHOOT
                else if (state == BALL_AIM)
                {
                    ball_init(ball_queue, mx, my);
                    state = BALL_SHOOT;
                }

                // Se estiver na tela de score, muda para o menu inicial
                else if (state == SCORE)
                    state = BEGIN;

                break;

            // Caso a janela gráfica seja fechada, encerra o jogo
            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                done = true;
                break;
        }

        // Caso o jogo seja encerrado, finaliza o loop principal
        if (done)
            break;

        // Se a variável de redesenhar estiver ativada e a fila de eventos estiver vazia
        if (redraw && al_is_event_queue_empty(queue))
        {
            switch (state)
            {
                case BEGIN: state_begin(fonts);
                    break;
                case HELP: state_help(fonts);
                    break;
                case BALL_AIM: state_ballaim(fonts, sprites, board, ball_queue, score, mx, my);
                    break;
                case BALL_SHOOT: state_ballshoot(fonts, sprites, board, ball_queue, score);
                    break;
                case SCORE: state_score(fonts, score_log, score, highscore);
                    break;
                default:
                    break;
            }
            redraw = false;
        }
    }

    // Libera a memória alocada
    free(event);
    font_deinit(fonts);
    sprite_deinit(sprites);
    audio_deinit(audios);
    board_deinit(board);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);
    al_destroy_display(disp);
    fclose(score_log);

    return 0;
}