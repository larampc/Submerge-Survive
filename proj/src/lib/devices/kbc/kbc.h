#ifndef _LCOM_KBC_H_
#define _LCOM_KBC_H_

/** @file */
/** @defgroup kbc KBC
 * @brief KBC device management module.
 * @{
 *
 */

/**
 * @brief Writes a byte to a specified KBC port.
 * 
 * @param port The port to which the byte will be written.
 * @param cmd The byte command to be written.
 * 
 * @return 0 on success, non-zero otherwise.
 */
int (kbc_write_byte)(int port, uint8_t cmd);

/**
 * @brief Reads a byte from the output buffer relative to the keyboard and mouse.
 * 
 * @param value Pointer to store the read byte value.
 * @param keyboard Boolean indicating if the read is from the keyboard (true) or mouse (false).
 * 
 * @return 0 on success, non-zero otherwise.
 */
int (kbc_read_byte)(uint8_t* value, bool keyboard);

#endif /* _LCOM_KBC_H_ */
