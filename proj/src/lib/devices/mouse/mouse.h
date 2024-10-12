#ifndef _LCOM_MOUSE_H_
#define _LCOM_MOUSE_H_

#include "ps2.h"

/** @file */
/** @defgroup mouse Mouse
 * @brief Mouse device management module.
 * @{
 *
 */

/**
 * @brief Subscribe mouse interrupts.
 * 
 * @param bit_no Pointer to store the bit number to be set in the interrupt mask.
 * 
 * @return 0 on success, non-zero otherwise.
 */
int (mouse_subscribe_int)(uint8_t *bit_no);

/**
 * @brief Unsubscribe mouse interrupts.
 * 
 * @return 0 on success, non-zero otherwise.
 */
int (mouse_unsubscribe_int)();

/**
 * @brief Mouse interrupt handler.
 * 
 * Handles the interrupt and reads the mouse data from the output buffer.
 */
void (mouse_ih)();

/**
 * @brief Assembles a mouse packet from the bytes read from the mouse.
 * 
 * @param pp Pointer to the mouse packet structure to be filled.
 */
void (assemble_packet)(mouse_packet* pp);

/**
 * @brief Disables mouse data reporting.
 * 
 * @return 0 on success, non-zero otherwise.
 */
int (mouse_disable_reporting)();

/**
 * @brief Enables mouse data reporting.
 * 
 * @return 0 on success, non-zero otherwise.
 */
int (mouse_enable_reporting)();

/**
 * @brief Retrieves the latest mouse event.
 * 
 * @param pp Pointer to the mouse packet structure containing the event data.
 */
void (get_mouse_event)(mouse_packet* pp);

/**
 * @brief Sends a command to the mouse.
 * 
 * @param cmd The command byte to be sent to the mouse.
 * 
 * @return 0 on success, non-zero otherwise.
 */
int (mouse_command)(uint8_t cmd);

/**
 * @brief Enables scrolling feature of the mouse, if supported.
 * 
 * @return 0 on success, non-zero otherwise.
 */
int (mouse_enable_scroll)();

/**
 * @brief Sends a command to get the mouse ID.
 * 
 * @return 0 on success, non-zero otherwise.
 */
int (mouse_command_get_id)();

#endif /* _LCOM_MOUSE_H_ */

