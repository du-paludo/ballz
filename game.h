#ifndef _GAME_
#define _GAME_

// Incrementa o score, atualiza o tabuleiro e testa se o jogo chegou ao fim
void board_update(square_t*** board, queue_t* ball_queue, state_t* state, FILE* score_log, int* score, int* highscore);

// Atualiza o timer e posição de cada bola, além de testar colisões
void ball_update(square_t*** board, queue_t* ball_queue, audios_t* audios);

// Salva o score em disco, caso tenha superado o highscore salvo
void file_update(FILE* score_log, int score, int* highscore);

// Atualiza variáveis em caso de colisão
void ball_collision(square_t*** board, queue_t* ball_queue, ball_t* ball, audios_t* audios);

// Detecta colisão entre bola e círculo
bool ball_circle_collide (ball_t* ball, square_t* square);

// Detecta colisão entre bola e quadrado
bool ball_square_collide(ball_t* ball, square_t* square);

// Desenha o tabuleiro
void draw_board(fonts_t* fonts, sprites_t* sprites, square_t*** board, int score);

// Desenha as bolas
void draw_balls(sprites_t* sprites, queue_t* ball_queue);

#endif