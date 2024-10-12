#ifndef _HANDLER_H_
#define _HANDLER_H_

#include "game.h"
#include "../lib/queue.h"

/** @file */
/** @defgroup handler Handler
 * @brief Serial port messages handling module.
 * @{
 *
 */

/**
 * @brief Function to handle connection options received by serial port.
 * 
 * @param game Pointer to the Game environment.
 * @param byte Byte received from the serial port.
 */
void connect_handler(Game* game, uint8_t byte);

/**
 * @brief Function to handle player options received by serial port.
 * 
 * @param game Pointer to the Game environment.
 * @param byte Byte received from the serial port.
 */
void player_handler(Game* game, uint8_t byte);

/**
 * @brief Function to handle Submarine options received by serial port.
 * 
 * @param game Pointer to the game environment.
 * @param byte Byte received from the serial port.
 */
void sub_handler(Game* game, uint8_t byte);

/**
 * @brief Function to handle Cannon options received by serial port.
 * 
 * @param game Pointer to the Game environment.
 * @param byte Byte received from the serial port.
 * @param read_queue Pointer to the queue for reading serial port messages.
 */
void cannon_handler(Game* game, uint8_t byte, queue_t *read_queue);

/**
 * @brief Function to handle Hook options received by serial port.
 * 
 * @param game Pointer to the Game environment.
 * @param byte Byte received from the serial port.
 */
void hook_handler(Game* game, uint8_t byte);

/**
 * @brief Function to handle Shop options received by serial port.
 * 
 * @param game Pointer to the Game environment.
 * @param byte Byte received from the serial port.
 */
void shop_handler(Game* game, uint8_t byte);

/**
 * @brief Function to handle destroy options received by serial port.
 * 
 * @param game Pointer to the Game environment.
 * @param byte Byte received from the serial port.
 * @param read_queue Pointer to the queue for reading serial port messages.
 */
void destroy_handler(Game* game, uint8_t byte, queue_t *read_queue);

#endif
