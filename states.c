#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include "allegro5/allegro_ttf.h"
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_image.h>

#include "struct.h"
#include "libqueue.h"
#include "aux.h"
#include "graphics.h"

void state_begin(fonts_t* fonts)
{
	al_clear_to_color(al_map_rgb(0, 0, 0));
    al_draw_text(fonts->font_big, al_map_rgb(250, 100, 100), DISP_W/2, DISP_H/2-140, ALLEGRO_ALIGN_CENTRE, "Ballz");
    al_draw_text(fonts->font_medium, al_map_rgb(230, 210, 70), DISP_W/2, DISP_H/2-20, ALLEGRO_ALIGN_CENTRE, "Clique em qualquer lugar da tela para jogar");
    al_draw_text(fonts->font_medium, al_map_rgb(70, 130, 230), DISP_W/2, DISP_H/2+20, ALLEGRO_ALIGN_CENTRE, "Pressione H ou F1 para ajuda ou mais informações");
    al_draw_text(fonts->font_medium, al_map_rgb(70, 210, 40), DISP_W/2, DISP_H/2+60, ALLEGRO_ALIGN_CENTRE, "Pressione ESC para sair");
    al_flip_display();
}

void state_help(fonts_t* fonts)
{
    al_clear_to_color(al_map_rgb(0, 0, 0));
    al_draw_text(fonts->font_medium, al_map_rgb(255, 255, 255), 20, 20, ALLEGRO_ALIGN_LEFT, "Ajuda");
    al_draw_text(fonts->font_small, al_map_rgb(255, 255, 255), 20, 60, ALLEGRO_ALIGN_LEFT, "1. Escolha uma direção com o mouse;");
    al_draw_text(fonts->font_small, al_map_rgb(255, 255, 255), 20, 90, ALLEGRO_ALIGN_LEFT, "2. Clique com o botão esquerdo ou pressione espaço para soltar as bolas;");
    al_draw_text(fonts->font_small, al_map_rgb(255, 255, 255), 20, 120, ALLEGRO_ALIGN_LEFT, "3. Ao encostar em um quadrado, uma vida dele é removida;");
    al_draw_text(fonts->font_small, al_map_rgb(255, 255, 255), 20, 150, ALLEGRO_ALIGN_LEFT, "4. O objetivo é eliminar o máximo de quadrados possíveis;");
    al_draw_text(fonts->font_small, al_map_rgb(255, 255, 255), 20, 180, ALLEGRO_ALIGN_LEFT, "5. A cada round sobrevivido, é somado 1 ponto ao score;");
    al_draw_text(fonts->font_small, al_map_rgb(255, 255, 255), 20, 210, ALLEGRO_ALIGN_LEFT, "6. O jogo acaba quando algum quadrado encostar no chão.");

    al_draw_text(fonts->font_medium, al_map_rgb(255, 255, 255), 20, 280, ALLEGRO_ALIGN_LEFT, "Informações");
    al_draw_text(fonts->font_small, al_map_rgb(255, 255, 255), 20, 320, ALLEGRO_ALIGN_LEFT, "Autoria de Eduardo Stefanel Paludo");
    al_draw_text(fonts->font_small, al_map_rgb(255, 255, 255), 20, 350, ALLEGRO_ALIGN_LEFT, "GRR20210581");
    al_flip_display();
}

void state_ballaim(fonts_t* fonts, sprites_t* sprites, square_t*** board, queue_t* ball_queue, int score, float mx, float my)
{
    al_clear_to_color(al_map_rgb(0, 0, 0));
    draw_board(fonts, sprites, board, score);

    float angle = get_angle(mx, my, ball_queue->spawn_x+BALL_RADIUS, SPAWN_Y+BALL_RADIUS);
    float x_dest = ball_queue->spawn_x+BALL_RADIUS - (cos(angle)*1400);
    float y_dest = SPAWN_Y+BALL_RADIUS - (sin(angle)*1400);
    al_draw_line(ball_queue->spawn_x+BALL_SIZE/2, SPAWN_Y + BALL_SIZE/2, x_dest, y_dest, al_map_rgb_f(1, 1, 1), 8);
    
    al_draw_scaled_bitmap(sprites->ball, 0, 0, 304, 304, ball_queue->spawn_x, SPAWN_Y, BALL_SIZE, BALL_SIZE, 0);
    al_draw_textf(fonts->font_small, al_map_rgb_f(1,1,1), ball_queue->spawn_x-40, DISP_H-BALL_SIZE-2, 0, "%2dx", queue_size(ball_queue));
    al_flip_display();
}

void state_ballshoot(fonts_t* fonts, sprites_t* sprites, square_t*** board, queue_t* ball_queue, int score)
{
    al_clear_to_color(al_map_rgb(0,0,0));
    draw_board(fonts, sprites, board, score);
    draw_balls(sprites, ball_queue);
    al_flip_display();
}

void state_score(fonts_t* fonts, FILE* score_log, int score, int highscore)
{
    al_clear_to_color(al_map_rgb(0, 0, 0));
    al_draw_textf(fonts->font_big, al_map_rgb_f(1,1,1), DISP_W/2, DISP_H/2-140, ALLEGRO_ALIGN_CENTRE, "%d", score);
    al_draw_textf(fonts->font_medium, al_map_rgb_f(250, 100, 100), DISP_W/2, DISP_H/2-20, ALLEGRO_ALIGN_CENTRE, "Highscore: %d", highscore);
    al_draw_text(fonts->font_medium, al_map_rgb(230, 210, 70), DISP_W/2, DISP_H/2+40, ALLEGRO_ALIGN_CENTRE, "Clique para retornar ao menu inicial");
    al_flip_display();
}
