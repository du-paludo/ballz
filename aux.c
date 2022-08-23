#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
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

void init_test(bool test, const char *description)
{
    if (test)
    	return;

    fprintf(stderr, "Não foi possível inicializar %s\n", description);
    exit(1);
}

int random(int a, int b)
{
    return (a + rand()%(b-a+1));
}

float deg_to_rad(float angle)
{
    return ((angle) * PI / 180.0);
}

float get_angle(float x1, float y1, float x2, float y2)
{
    float angle = atan2(y2 - y1, x2 - x1);
    if (angle < deg_to_rad(MIN_ANGLE))
        angle = deg_to_rad(MIN_ANGLE);
    else if (angle > deg_to_rad(MAX_ANGLE))
        angle = deg_to_rad(MAX_ANGLE);
    return angle;
}

char* get_current_time()
{
    time_t current_time;
    time(&current_time);
    char* string = ctime(&current_time);
    return string;
}

int get_highscore(FILE* score_log)
{
    char* line = malloc(sizeof(char) * LINE_SIZE);
    char* token1, *token2;
    int highscore;
    rewind(score_log);
    while (!feof(score_log))
        fgets(line, LINE_SIZE, score_log);
    token1 = strtok(line, " ");
    token2 = strtok(NULL, " ");
    if (!strcmp(token1, "score"))
        highscore = atoi(token2);
    else
        highscore = 0;
    free(line);
    return highscore;
}

void speed_increase(queue_t* ball_queue, bool speedup)
{
    node_t* aux = ball_queue->begin;
    while (aux)
    {
        if (!speedup)
        {
            aux->key->dx *= 1.5;
            aux->key->dy *= 1.5;
        }
        else
        {
            aux->key->dx /= 1.5;
            aux->key->dy /= 1.5;
        }
        aux = aux->next;
    }
}

bool all_balls_off(queue_t* ball_queue)
{
    node_t* aux = ball_queue->begin;
    while (aux)
    {
        if (aux->key->on == true)
            return false;
        aux = aux->next;
    }
    return true;
}