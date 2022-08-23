#ifndef _STATES_
#define _STATES_

// Determina o que deve ser exibido na tela em cada estado

void state_begin(fonts_t* fonts);

void state_help(fonts_t* fonts);

void state_ballaim(fonts_t* fonts, sprites_t* sprites, square_t*** board, queue_t* ball_queue, int score, float mx, float my);

void state_ballshoot(fonts_t* fonts, sprites_t* sprites, square_t*** board, queue_t* ball_queue, int score, int texttimer);

void state_score(fonts_t* fonts, FILE* score_log, int score, int highscore);

#endif