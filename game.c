#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_image.h>

#include "struct.h"
#include "libqueue.h"
#include "aux.h"
#include "game.h"

void board_update(square_t*** board, queue_t* ball_queue, state_t* state, FILE* score_log, int* score, int* highscore)
{
    // Incrementa o score
    (*score)++;

    // Incrementa o número de bolas
    for (int i = 0; i < ball_queue->circle_count; i++)
    {
        queue_insert(ball_queue);
        ball_queue->circle_count--;
    }

    int row, col;
    // Empurra a matriz uma linha para baixo
    for (row = DISP_H/SQUARE_SIZE-2; row >= 0; row--)
        for (int col = 0; col < DISP_W/SQUARE_SIZE; col++)
        {
            board[row+1][col]->lives = board[row][col]->lives;
            board[row+1][col]->circle = board[row][col]->circle;
        }

    // Escolhe uma coluna aleatória para aparecer o círculo
    int circle_col = random(0, DISP_W/SQUARE_SIZE-1);
    int square_count = 0;

    for (col = 0; col < DISP_W/SQUARE_SIZE; col++)
    {
        // Se a coluna for a escolhida anteriormente, atribui o círculo a ela
        if (col == circle_col)
        {
            board[1][col]->lives = 1;
            board[1][col]->circle = true;
        }
        // Caso contrário, cria um quadrado com probabilidade de 2/5, limitado a 4 quadrados por round
        else if (random(1, 5) <= 2 && square_count < 4)
        {
            // A quantidade de vidas do quadrado é escolhida aleatoriamente entre o score e 1.6*score
            board[1][col]->lives = random(*score, floor((*score)*1.6));
            square_count++;
        }
    }

    // Se houver algum quadrado na última linha da matriz, o jogo acaba
    for (col = 0; col < DISP_W/SQUARE_SIZE; col++)
    {
        if (board[DISP_H/SQUARE_SIZE-1][col]->lives > 0 && !board[DISP_H/SQUARE_SIZE-1][col]->circle)
        {
            file_update(score_log, *score, highscore);
            queue_destroy(ball_queue);
            (*state) = SCORE;
            return;
        }
    }
}

void ball_update(square_t*** board, queue_t* ball_queue, audios_t* audios)
{
    node_t* aux = ball_queue->begin;
    while (aux)
    {
        // Se a bola ainda não tiver sido lançada, decrementa o timer
        if (aux->key->spawn_timer > 0)
            aux->key->spawn_timer--;
        // Caso contrário, testa colisão e atualiza posição
        else
        {
            ball_collision(board, ball_queue, aux->key, audios);
            aux->key->x += aux->key->dx;
            aux->key->y += aux->key->dy;
        }
        aux = aux->next;
    }
}

void file_update(FILE* score_log, int score, int* highscore)
{
    char* time = get_current_time();
    *highscore = get_highscore(score_log);

    // Se o score for maior que o highscore, armazena em disco
    if (score > *highscore)
    {
        fprintf(score_log, "score %d - %s", score, time);
        *highscore = score;
    }
}

void ball_collision(square_t*** board, queue_t* ball_queue, ball_t* ball, audios_t* audios)
{
    bool first = true;

    for (int row = 0; row < DISP_H/SQUARE_SIZE; row++)
        for (int col = 0; col < DISP_W/SQUARE_SIZE; col++)
            if (board[row][col]->lives > 0)
            {
                if (board[row][col]->circle)
                {
                    if (ball_circle_collide(ball, board[row][col]))
                    {
                        al_play_sample(audios->circle_pickup, 0.2, 0.0, 1.2, ALLEGRO_PLAYMODE_ONCE, NULL);
                        board[row][col]->circle = false;
                        board[row][col]->lives--;
                        ball_queue->circle_count++;
                        return;
                    }    
                }
                else
                {
                    if (ball_square_collide(ball, board[row][col]))
                    {
                        al_play_sample(audios->pop, 1.0, 0.0, 1.2, ALLEGRO_PLAYMODE_ONCE, NULL);
                        if (bccaux)
                            board[row][col]->lives = 0;
                        else
                            board[row][col]->lives--;
                        return;
                    }
                }
            }
    
    if (ball->x + ball->dx < 0 || (ball->x + BALL_SIZE + ball->dx) > DISP_W)
        ball->dx *= -1;
    if (ball->y + ball->dy < 0)
        ball->dy *= -1;
    if ((ball->y + BALL_SIZE + ball->dy) > DISP_H)
    {
        ball->on = false;
        if (first)
        {
            ball_queue->spawn_x = ball->x;
            first = false;
        }
    }
}

bool ball_circle_collide(ball_t* ball, square_t* square)
{
    float x1 = ball->x+BALL_RADIUS;
    float y1 = ball->y+BALL_RADIUS;
    float x2 = square->x+CIRCLE_SIZE;
    float y2 = square->y+CIRCLE_SIZE;
    double distance = sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1));
    if (distance < (BALL_RADIUS + CIRCLE_SIZE/2))
        return true;
    return false;
}

bool ball_square_collide(ball_t* ball, square_t* square)
{
    bool aux = false;

    float ballx2 = ball->x + BALL_SIZE;
    float bally2 = ball->y + BALL_SIZE;
    float squarex2 = square->x + SQUARE_SIZE;
    float squarey2 = square->y + SQUARE_SIZE;

    if (ballx2 + ball->dx > square->x && ball->x + ball->dx < squarex2 && bally2 > square->y && ball->y < squarey2)
    {
        ball->dx *= -1;
        aux = true;
    }

    if (ballx2 > square->x &&  ball->x < squarex2 && bally2 + ball->dy > square->y &&  ball->y + ball->dy < squarey2)
    {
        ball->dy *= -1;
        aux = true;
    }

    return aux;
}

void draw_board(fonts_t* fonts, sprites_t* sprites, square_t*** board, int score)
{
    // Desenha o score no topo da tela
    al_draw_textf(fonts->font_medium, al_map_rgb_f(1,1,1), DISP_W/2, 10, ALLEGRO_ALIGN_CENTRE, "%d", score);
    
    for (int row = 0; row < DISP_H/SQUARE_SIZE; row++)
        for (int col = 0; col < DISP_W/SQUARE_SIZE; col++)
            if (board[row][col]->lives > 0)
            {  
                // Desenha os quadrados e círculos
                if (board[row][col]->circle == true)
                {
                    al_draw_scaled_bitmap(sprites->circle, 0, 0, 1024, 1024, board[row][col]->x + 20, board[row][col]->y + 20, 40, 40, 0);
                    continue;
                }
                else if (board[row][col]->lives < 10)
                    al_draw_scaled_bitmap(sprites->white_square, 0, 0, 48, 48, board[row][col]->x, board[row][col]->y, SQUARE_SIZE, SQUARE_SIZE, 0);
                else if (board[row][col]->lives >= 10 && board[row][col]->lives < 20)
                    al_draw_scaled_bitmap(sprites->green_square, 0, 0, 48, 48, board[row][col]->x, board[row][col]->y, SQUARE_SIZE, SQUARE_SIZE, 0);
                else if (board[row][col]->lives >= 20 && board[row][col]->lives < 30)
                    al_draw_scaled_bitmap(sprites->blue_square, 0, 0, 48, 48, board[row][col]->x, board[row][col]->y, SQUARE_SIZE, SQUARE_SIZE, 0);
                else if (board[row][col]->lives >= 30 && board[row][col]->lives < 40)
                    al_draw_scaled_bitmap(sprites->yellow_square, 0, 0, 48, 48, board[row][col]->x, board[row][col]->y, SQUARE_SIZE, SQUARE_SIZE, 0);
                else if (board[row][col]->lives >= 40)
                    al_draw_scaled_bitmap(sprites->red_square, 0, 0, 48, 48, board[row][col]->x, board[row][col]->y, SQUARE_SIZE, SQUARE_SIZE, 0);
                
                // Desenha o número de vidas dos quadrados
                al_draw_textf(fonts->font_medium, al_map_rgb_f(0,0,0), board[row][col]->x+SQUARE_SIZE/2, board[row][col]->y+20, ALLEGRO_ALIGN_CENTRE, "%d", board[row][col]->lives);
            }
}

void draw_balls(sprites_t* sprites, queue_t* ball_queue)
{
    node_t* aux = ball_queue->begin;
    while (aux)
    {
        if (bccaux)
            al_draw_tinted_scaled_bitmap(sprites->ball, al_map_rgb(random(0,255),random(0,255),random(0,255)),
            0, 0, 304, 304, aux->key->x, aux->key->y, BALL_SIZE, BALL_SIZE, 0);
        else
            al_draw_scaled_bitmap(sprites->ball, 0, 0, 304, 304, aux->key->x, aux->key->y, BALL_SIZE, BALL_SIZE, 0);
        aux = aux->next;
    }
}