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
#include "init.h"

ALLEGRO_DISPLAY* display_init()
{
    al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
    al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);
    ALLEGRO_DISPLAY* disp = al_create_display(DISP_W, DISP_H);
    init_test(disp, "display");

    return disp;
}

fonts_t* font_init()
{
    init_test(al_init_font_addon(), "addon de fonte");
    init_test(al_init_ttf_addon(), "addon de ttf");

    fonts_t* fonts = malloc(sizeof(fonts_t));
    fonts->font_big = al_load_font("resources/DejaVuSans-Bold.ttf", 100, 0);
    init_test(fonts->font_big, "fonte");
    fonts->font_medium = al_load_font("resources/DejaVuSans.ttf", 30, 0);
    init_test(fonts->font_medium, "fonte");
    fonts->font_small = al_load_font("resources/DejaVuSans.ttf", 20, 0);
    init_test(fonts->font_small, "fonte");

    return fonts;
}

sprites_t* sprite_init()
{
    init_test(al_init_image_addon(), "addon de imagem");
    init_test(al_init_primitives_addon(), "primitivas");
    sprites_t* sprites = malloc(sizeof(sprites_t));

    sprites->white_square = al_load_bitmap("resources/white_square.png");
    init_test(sprites->white_square, "quadrado branco");
    sprites->green_square = al_load_bitmap("resources/green_square.png");
    init_test(sprites->green_square, "quadrado verde");
    sprites->blue_square = al_load_bitmap("resources/blue_square.png");
    init_test(sprites->blue_square, "quadrado azul");
    sprites->yellow_square = al_load_bitmap("resources/yellow_square.png");
    init_test(sprites->yellow_square, "quadrado amarelo");
    sprites->red_square = al_load_bitmap("resources/red_square.png");
    init_test(sprites->red_square, "quadrado vermelho");
    sprites->ball = al_load_bitmap("resources/ball.png");
    init_test(sprites->ball, "bola");
    sprites->ball_line = al_load_bitmap("resources/ball_line.png");
    init_test(sprites->ball_line, "linha de bolas");
    sprites->circle = al_load_bitmap("resources/circle.png");
    init_test(sprites->circle, "círculo");

    return sprites;
}

audios_t* audio_init()
{
    init_test(al_install_audio(), "audio");
    init_test(al_init_acodec_addon(), "audio codecs");
    init_test(al_reserve_samples(128), "reserve samples");
    audios_t* audios = malloc(sizeof(audios_t));

    audios->pop = al_load_sample("resources/bong_001.ogg");
    init_test(audios->pop, "pop sound");

    audios->circle_pickup = al_load_sample("resources/circle_pickup.wav");
    init_test(audios->circle_pickup, "circle pickup sound");

    return audios;
}

square_t*** board_alloc()
{
    square_t*** board = malloc(DISP_H/SQUARE_SIZE * sizeof(square_t**));
    for (int row = 0; row < DISP_H/SQUARE_SIZE; row++)
    {
        board[row] = malloc(DISP_W/SQUARE_SIZE * sizeof(square_t*));
        for (int col = 0; col < DISP_W/SQUARE_SIZE; col++)
            board[row][col] = malloc(sizeof(square_t));
    }
    return board;
}

void board_init(square_t*** board)
{
    for (int row = 0; row < DISP_H/SQUARE_SIZE; row++)
        for (int col = 0; col < DISP_W/SQUARE_SIZE; col++)
        {
            board[row][col]->x = col * SQUARE_SIZE;
            board[row][col]->y = row * SQUARE_SIZE;
            board[row][col]->lives = 0;
            board[row][col]->circle = false;
        }
}

void ball_init(queue_t* ball_queue, int mx, int my)
{
    float angle = get_angle(mx, my, ball_queue->spawn_x+BALL_RADIUS, SPAWN_Y+BALL_RADIUS);

    float sinx = sin(angle);
    float cosx = cos(angle);

    int i = 0;
    node_t* aux = ball_queue->begin;
    while (aux)
    {
        aux->key->x = ball_queue->spawn_x;
        aux->key->y = SPAWN_Y;
        aux->key->dx = -(BALL_SPEED * cosx);
        aux->key->dy = -(BALL_SPEED * sinx);
        aux->key->spawn_timer = i * SPAWN_TIMER;
        aux->key->on = true;
        aux = aux->next;
        i++;
    }
}

void font_deinit(fonts_t* fonts)
{
    al_destroy_font(fonts->font_big);
    al_destroy_font(fonts->font_medium);
    al_destroy_font(fonts->font_small);
    free(fonts);
}

void sprite_deinit(sprites_t* sprites)
{
    al_destroy_bitmap(sprites->white_square);
    al_destroy_bitmap(sprites->green_square);
    al_destroy_bitmap(sprites->blue_square);
    al_destroy_bitmap(sprites->yellow_square);
    al_destroy_bitmap(sprites->red_square);
    al_destroy_bitmap(sprites->ball);
    al_destroy_bitmap(sprites->ball_line);
    al_destroy_bitmap(sprites->circle);
    free(sprites);
}

void audio_deinit(audios_t* audios)
{
    al_destroy_sample(audios->pop);
    al_destroy_sample(audios->circle_pickup);
    free(audios);
}

void board_deinit(square_t*** board)
{
    for (int row = 0; row < DISP_H/SQUARE_SIZE; row++)
    {
        for (int col = 0; col < DISP_W/SQUARE_SIZE; col++)
            free(board[row][col]);
        free(board[row]);
    }
    free(board);
}