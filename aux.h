#ifndef _AUX_
#define _AUX_

// Imprime na saída de erros caso a inicialização não tenha sido bem-sucedida
void init_test(bool test, const char *description);

// Retorna um inteiro aleatório entre a e b
int random(int a, int b);

// Retorna o ângulo dado pela distância entre a posição do mouse e um ponto
float get_angle(float x1, float y1, float x2, float y2);

// Transforma um ângulo dado em graus para radianos
// float deg_to_rad(float angle);

// Transforma um ângulo dado em radianos para graus
// float rad_to_deg(float angle);

// Retorna o horário atual do sistema em formato de string
char* get_current_time();

// Retorna o último highscore salvo no arquivo score_log
int get_highscore(FILE* score_log);

// Duplica a velocidade das bolas
void speed_increase(queue_t* ball_queue, bool speedup);

// Verifica se ainda há bolas dentro dos limites da tela
bool all_balls_off(queue_t* ball_queue);

#endif