#ifndef _SP_H_
#define _SP_H_

#include "UART.h"
#include "../../../game/game.h"

/** @file */
/** @defgroup sp Sp
 *
 *
 * @brief Serial port management module
 * @{
 */


/**
 * @brief Subscribes to serial port interrupts.
 * 
 * @param bit_no Pointer to the bit number to be used for interrupt notifications.
 * @return Zero on success, non-zero otherwise.
 */
int (sp_subscribe_int)(uint8_t *bit_no);
/**
 * @brief Unsubscribes from serial port interrupts.
 * 
 * @return Zero on success, non-zero otherwise.
 */
int (sp_unsubscribe_int)();
/**
 * @brief Enables IRQ interrupts for the serial port.
 * 
 * @return Zero on success, non-zero otherwise.
 */
int (sp_enable_irq_interrupts)();
/**
 * @brief Disables IRQ interrupts for the serial port.
 * 
 * @return Zero on success, non-zero otherwise.
 */
int (sp_disable_irq_interrupts)();
/**
 * @brief Initializes the serial port.
 * 
 * @return Zero on success, non-zero otherwise.
 */
int (sp_init)();
/**
 * @brief Cleans up and deletes the serial port.
 */
void (delete_sp)();
/**
 * @brief Reads the Line Control Register (LCR) of the serial port.
 * 
 * @param lcr Pointer to the variable to store the LCR value.
 * @return Zero on success, non-zero otherwise.
 */
int (sp_read_lcr)(uint8_t *lcr);
/**
 * @brief Writes to the Line Control Register (LCR) of the serial port.
 * 
 * @param lcr Value to be written to the LCR.
 * @return Zero on success, non-zero otherwise.
 */
int(sp_write_lcr)(uint8_t lcr);
/**
 * @brief Reads the data from the FIFO (First In First Out) buffer.
 * 
 * @return Zero on success, non-zero otherwise.
 */
int (sp_read_fifo)();
/**
 * @brief Writes data to the FIFO (First In First Out) buffer.
 * 
 * @return Zero on success, non-zero otherwise.
 */
int (sp_write_fifo)();
/**
 * @brief Reads the Line Status Register (LSR) of the serial port.
 * 
 * @param status Pointer to the variable to store the LSR value.
 * @return Zero on success, non-zero otherwise.
 */
int(sp_read_ls)(uint8_t *status);
/**
 * @brief Reads data from the Receiver Buffer Register (RBR) of the serial port.
 * 
 * @param data Pointer to the variable to store the read data.
 * @return Zero on success, non-zero otherwise.
 */
int (sp_read_data)(uint8_t *data);
/**
 * @brief Writes data to the Transmitter Holding Register (THR) of the serial port.
 * 
 * @param data Data to be written.
 * @return Zero on success, non-zero otherwise.
 */
int (sp_write_data)(uint8_t data);
/**
 * @brief Serial port interrupt handler.
 * 
 * @param game Pointer to the game structure.
 */
void (sp_ih)(Game* game);
/**
 * @brief Reads and processes data from the read queue.
 * 
 * @param game Pointer to the game structure.
 * @return Zero on success, non-zero otherwise.
 */
int (sp_read_rq)(Game* game);
/**
 * @brief Writes data to the write queue.
 * 
 * @param byte Data byte to be written.
 * @return Zero on success, non-zero otherwise.
 */
int (sp_write_wq)(uint8_t byte);

#endif
