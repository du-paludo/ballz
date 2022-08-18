#ifndef _INIT_
#define _INIT_

// Inicializa display e o retorna
ALLEGRO_DISPLAY* display_init();

// Inicializa fontes e retorna ponteiro para a struct
fonts_t* font_init();

// Inicializa sprites e retorna ponteiro para a struct
sprites_t* sprite_init();

// Inicializa áudios e retorna ponteiro para a struct
audios_t* audio_init();

// Aloca memória para a matriz de quadrados e retorna ponteiro
square_t*** board_alloc();

// Atribui as posições corretas de cada quadrado e zera as vidas
void board_init(square_t*** board);

// Inicializa a posição, velocidade e timer de cada bola
void ball_init(queue_t* ball_queue, int mx, int my);

// Libera a memória alocada para cada fonte
void font_deinit(fonts_t* fonts);

// Libera a memória alocada para cada sprite
void sprite_deinit(sprites_t* sprites);

// Libera a memória alocada para cada áudio
void audio_deinit(audios_t* audios);

// Libera a memória alocada para o tabuleiro
void board_deinit(square_t*** board);

#endif