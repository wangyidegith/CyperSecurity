#include <stdlib.h>
#include <pthread.h>

#include "queue.h"

typedef struct Queue {
    int QUEUE_SIZE;
    int front;
    int rear;
    int count;
    pthread_mutex_t mutex;
    pthread_cond_t not_full;
    pthread_cond_t not_empty;
    T items[0];
} Queue;

Queue* queueInit(const int QUEUE_SIZE) {
    Queue* q = (Queue*)calloc(sizeof(Queue) + QUEUE_SIZE * sizeof(T), sizeof(char));
    q->QUEUE_SIZE = QUEUE_SIZE;
    if (pthread_mutex_init(&(q->mutex), NULL) == -1) { return NULL; }
    if (pthread_cond_init(&(q->not_full), NULL) == -1) { return NULL; }
    if (pthread_cond_init(&(q->not_empty), NULL) == -1) { return NULL; }
    return q;
}

void enqueue(Queue* q, T item) {
    pthread_mutex_lock(&(q->mutex));
    while (q->count == q->QUEUE_SIZE) {
        pthread_cond_wait(&(q->not_full), &(q->mutex));
    }
    q->items[q->rear] = item;
    q->rear = (q->rear + 1) % q->QUEUE_SIZE;
    (q->count)++;
    pthread_cond_signal(&(q->not_empty));
    pthread_mutex_unlock(&(q->mutex));
}

T dequeue(Queue* q) {
    pthread_mutex_lock(&(q->mutex));
    while (q->count == 0) {
        pthread_cond_wait(&(q->not_empty), &(q->mutex));
    }
    T item = q->items[q->front];
    q->front = (q->front + 1) % q->QUEUE_SIZE;
    (q->count)--;
    pthread_cond_signal(&(q->not_full));
    pthread_mutex_unlock(&(q->mutex));
    return item;
}

void queueDestroy(Queue* q) {
    pthread_mutex_destroy(&(q->mutex));
    pthread_cond_destroy(&(q->not_full));
    pthread_cond_destroy(&(q->not_empty));
    free(q);
}
