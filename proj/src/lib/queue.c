#include <lcom/lcf.h>
#include "queue.h"

queue_t *(new_queue)(unsigned int size) {
    queue_t *queue = malloc(sizeof(queue_t));            // allocate queue object
    if (queue == NULL) return NULL;

    queue->size = size ? size : 1;
    queue->buf = malloc(queue->size * sizeof(uint8_t)); // allocate space to store queue elements
    memset(queue->buf, 0, queue->size * sizeof(uint8_t));
    
    if (queue->buf == NULL) {
        free(queue);
        return NULL;
    }

    queue->front = queue->back = queue->count = 0;      // initialize state of queue

    return queue;
}

void (delete_queue)(queue_t *queue) {
    free(queue->buf);
    free(queue);
}

bool (is_full_queue)(queue_t *queue) {
    return (queue->size == queue->count);
}

bool (is_empty_queue)(queue_t *queue) {
    return queue->count == 0;
}

int (push_queue)(queue_t *queue, uint8_t n) {
    if (is_full_queue(queue)) {
        if (resize_queue(queue)) return 1;
    }

    queue->buf[queue->back++] = n;
    queue->count++;
    adjust_queue(queue);
    return 0;
}

int (pop_queue)(queue_t *queue, uint8_t *n) {
    if (is_empty_queue(queue)) return 1;

    memcpy(n, queue->buf + queue->front, sizeof(uint8_t));
    queue->front += 1;
    queue->count--;
    adjust_queue(queue);
    return 0;
}

int (resize_queue)(queue_t *queue) {
    uint8_t *p = (uint8_t *) realloc(queue->buf, 2*(queue->size)*sizeof(uint8_t));
    if(p == NULL) return 1;
    queue->buf = p;
    memcpy(queue->buf + queue->size, queue->buf, queue->size*sizeof(uint8_t));
    queue->front = queue->size + queue->front;
    queue->size *= 2;
    return 0;
}

void (adjust_queue)(queue_t *q) {
    q->front %= q->size;
    q->back %= q->size;
}
