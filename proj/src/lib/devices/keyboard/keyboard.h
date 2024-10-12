#ifndef _LCOM_KEYBOARD_H_
#define _LCOM_KEYBOARD_H_


/** @file */
/** @defgroup keyboard Keyboard
 * @brief Keyboard device management module.
 * @{
 *
 */

/**
 * @brief Subscribe keyboard interrupts.
 * 
 * @param bit_no Pointer to store the bit number to be set in the interrupt mask.
 * 
 * @return 0 on success, non-zero otherwise.
 */
int (keyboard_subscribe_int)(uint8_t *bit_no);

/**
 * @brief Unsubscribe keyboard interrupts.
 * 
 * @return 0 on success, non-zero otherwise.
 */
int (keyboard_unsubscribe_int)();

/**
 * @brief Keyboard interrupt handler.
 * 
 * Handles the interrupt and reads the scancode from the output buffer.
 */
void (kbd_ih)();

#endif /* _LCOM_KEYBOARD_H_ */
