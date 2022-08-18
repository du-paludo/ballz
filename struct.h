#ifndef _STRUCT_
#define _STRUCT_

// Tamanho da tela
#define DISP_W 960
#define DISP_H 720

// Tamanho máximo da linha a ser lida no arquivo de log
#define LINE_SIZE 64

// Largura/altura do quadrado e do círculo
#define SQUARE_SIZE 80
#define CIRCLE_SIZE 40

// Diâmetro, raio e velocidade da bola
#define BALL_SIZE 20
#define BALL_RADIUS (BALL_SIZE/2)
#define BALL_SPEED 14

// Intervalo de frames entre uma bola e outra
#define SPAWN_TIMER 8
#define SPAWN_Y (DISP_H - BALL_SIZE)

// Ângulo mínimo e máximo de lançamento da bola
#define MIN_ANGLE 10
#define MAX_ANGLE 170

#define PI 3.14159265358979323846264338327950288

typedef enum {BEGIN, HELP, BALL_AIM, BALL_SHOOT, SCORE} state_t;

typedef struct ball_t
{
	float x, y;
	float dx, dy;
	int spawn_timer;
	bool on;
} ball_t;

typedef struct square_t
{
	float x, y;
	int lives;
	bool circle;
} square_t;

typedef struct sprites_t
{
	ALLEGRO_BITMAP* white_square;
	ALLEGRO_BITMAP* green_square;
	ALLEGRO_BITMAP* blue_square;
	ALLEGRO_BITMAP* yellow_square;
	ALLEGRO_BITMAP* red_square;
	ALLEGRO_BITMAP* ball;
	ALLEGRO_BITMAP* ball_line;
	ALLEGRO_BITMAP* circle;
} sprites_t;

typedef struct fonts_t
{
	ALLEGRO_FONT* font_small;
	ALLEGRO_FONT* font_medium;
	ALLEGRO_FONT* font_big;
} fonts_t;

typedef struct audios_t
{
	ALLEGRO_SAMPLE* pop;
	ALLEGRO_SAMPLE* circle_pickup;
} audios_t;

extern bool bccaux;

#endif