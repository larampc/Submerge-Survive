#ifndef _LCOM_TIMER_H_
#define _LCOM_TIMER_H_

#include <lcom/lcf.h>

/** @file */
/** @defgroup timer Timer
 * @brief Timer device management module.
 * @{
 *
 */

/**
 * @brief Timer interrupt handler.
 * 
 * Handles the interrupt and increments count.
 */
void (timer_ih)();

/**
 * @brief Subscribe timer interrupts.
 * 
 * @param bit_no Pointer to store the bit number to be set in the interrupt mask.
 * 
 * @return 0 on success, non-zero otherwise.
 */
int (timer_subscribe_int)(uint8_t* bit_no);

/**
 * @brief Unsubscribe timer interrupts.
 * 
 * @return 0 on success, non-zero otherwise.
 */
int (timer_unsubscribe_int)();


#endif /* _LCOM_TIMER_H_ */
