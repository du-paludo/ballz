#ifndef _LIBQUEUE_
#define _LIBQUEUE_

struct node
{
    ball_t* key;
    struct node* next;
};
typedef struct node node_t;

struct queue
{
    node_t* begin; // Ponteiro para o início da fila
    node_t* end; // Ponteiro para o fim da fila
    int size; // Número de elementos da fila
    int circle_count; // Armazena o número de círculos coletados em um round
    float spawn_x; // Spawn das bolas da fila
};
typedef struct queue queue_t;

// Cria uma fila vazia e a retorna; se falhar retorna NULL
queue_t* queue_init ();

// Remove todos os elementos da fila, libera memória e retorna NULL
queue_t* queue_destroy (queue_t* q);

// Retorna 1 se a fila estiver vazia e 0 caso contrário
int queue_empty (queue_t* q);

// Retorna o número de elementos presentes na fila
int queue_size (queue_t* q);

// Insere o elemento no final da fila (política FIFO)
// Retorna 1 se a operação foi bem sucedida e 0 caso contrário
int queue_insert (queue_t* q);

// Remove o elemento do início da fila (politica FIFO)
// Retorna 1 se a operação foi bem sucedida e 0 caso contrário
int dequeue (queue_t* q);

#endif
