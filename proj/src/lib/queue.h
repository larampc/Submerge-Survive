#ifndef _QUEUE_H_
#define _QUEUE_H_

#include <lcom/lcf.h>

/** @defgroup queue Queue
 * @brief Queue data structure module.
 * @{
 */

/**
 * @brief Structure representing a queue.
 */
typedef struct {
    uint8_t *buf; /**< Pointer to the array that stores queue elements */
    unsigned int front; /**< Index of the front element in the array */
    unsigned int back; /**< Index of the back element in the array */
    unsigned int size; /**< Size of the array */
    unsigned int count; /**< Number of elements currently in the queue */
} queue_t;

/**
 * @brief Creates a new queue with the given size.
 * 
 * @param size The size of the queue.
 * @return Pointer to the created queue.
 */
queue_t* new_queue(unsigned int size);

/**
 * @brief Deletes the given queue and frees associated resources.
 * 
 * @param queue Pointer to the queue to be deleted.
 */
void delete_queue(queue_t *queue);

/**
 * @brief Checks if the queue is full.
 * 
 * @param queue Pointer to the queue to be checked.
 * @return true if the queue is full, false otherwise.
 */
bool is_full_queue(queue_t *queue);

/**
 * @brief Checks if the queue is empty.
 * 
 * @param queue Pointer to the queue to be checked.
 * @return true if the queue is empty, false otherwise.
 */
bool is_empty_queue(queue_t *queue);

/**
 * @brief Pushes an element into the queue.
 * 
 * @param queue Pointer to the queue.
 * @param n The element to be pushed.
 * @return 0 on success, non-zero on failure.
 */
int push_queue(queue_t *queue, uint8_t n);

/**
 * @brief Pops an element from the queue.
 * 
 * @param queue Pointer to the queue.
 * @param n Pointer to the variable where the popped element will be stored.
 * @return 0 on success, non-zero on failure.
 */
int pop_queue(queue_t *queue, uint8_t *n);

/**
 * @brief Resizes the queue to accommodate more elements.
 * 
 * @param queue Pointer to the queue to be resized.
 * @return 0 on success, non-zero on failure.
 */
int resize_queue(queue_t *queue);

/**
 * @brief Adjusts the indices of the queue elements.
 * 
 * @param queue Pointer to the queue to be adjusted.
 */
void adjust_queue(queue_t *queue);

/** @} end of queue */

#endif /* _QUEUE_H_ */
