#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include "allegro5/allegro_ttf.h"
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_image.h>

#include "struct.h"
#include "libqueue.h"

queue_t* queue_init ()
{
    queue_t* q = malloc(sizeof(queue_t));
    if (q == NULL)
        return NULL;
    q->begin = NULL;
    q->end = NULL;
    q->size = 0;
    q->circle_count = 0;
    q->spawn_x = (DISP_W/2)-(BALL_SIZE/2);
    queue_insert(q);
    return q;
}

queue_t* queue_destroy (queue_t* q)
{
    node_t* current = q->begin;
    node_t* aux;

    // Percorre a fila até o fim ser nulo,
    // limpando o nodo atual e armazenando em uma variável o próximo
    while (current != NULL)
    {
        aux = current->next;
        free(current->key);
        free(current);
        current = aux;
    }
    free(q);
    return NULL;
}

int queue_empty (queue_t* q)
{
    if (q->begin == NULL)
        return 1;
    return 0;
}

int queue_size (queue_t* q)
{
    return q->size;
}

int queue_insert (queue_t* q)
{
    // Aloca memória para um novo nodo
    node_t* aux = malloc(sizeof(node_t));
    if (aux == NULL)
        return 0;

    ball_t* key = malloc(sizeof(node_t));
    if (key == NULL)
        return 0;

    /* Inicializa o nodo e incrementa o tamanho da fila */
    aux->key = key;
    aux->next = NULL;
    q->size++;

    /* Se a fila estiver vazia, então o novo nodo é tanto o início como o fim da fila */
    if (queue_empty(q))
    {
        q->end = aux;
        q->begin = q->end;
    }

    /* Se não, aponta o fim da fila para o novo nodo */
    else
    {
        q->end->next = aux;
        q->end = aux;
    }
    return 1; 
}

int dequeue (queue_t* q)
{
    /* Se a fila estiver vazia, retorna 0 */
    if (queue_empty(q))
        return 0;

    /* Armazena o início da fila em uma variável auxiliar */
    node_t* aux = q->begin;

    /* Aponta o início para o próximo elemento da queue */
    q->begin = q->begin->next;

    /* Se o novo início for nulo, então o fim também deve ser */
    if (q->begin == NULL)
        q->end = NULL;

    /* Remove o primeiro elemento */
    free(aux);
    q->size--;
    return 1;
}