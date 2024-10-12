#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

/** @file */
/** @defgroup i8042 i8042
 * @brief Constants for programming the i8042 KBC.
 * @{
 *
 */

#define KBD_IRQ 1 /**< @brief Keyboard IRQ line */
#define MOUSE_IRQ 12 /**< @brief Mouse IRQ line */

/* I/O port addresses */

#define KBC_OUT_BUF 0x60 /**< @brief KBC output buffer */
#define KBC_IN_BUF 0x60 /**< @brief KBC input buffer */
#define KBC_ARGS_REG 0x60 /**< @brief KBC arguments register */
#define KBC_RET_REG KBC_OUT_BUF /**< @brief KBC return register */

/* KBC control */

#define KBC_STATUS_REG 0x64 /**< @brief KBC status register */
#define KBC_CMD_REG 0x64 /**< @brief KBC command register */

#define KBC_PARITY_ERROR BIT(7) /**< @brief Parity error - invalid data */
#define KBC_TIMEOUT_ERROR BIT(6) /**< @brief Timeout error - invalid data */
#define KBC_AUX BIT(5) /**< @brief Mouse data */
#define KBC_INH BIT(4) /**< @brief Inhibit flag: 0 if keyboard is inhibited */
#define KBC_IBF BIT(1) /**< @brief Input buffer full */
#define KBC_OBF BIT(0) /**< @brief Output buffer full */

/* KBC commands */

#define KBC_READ_CMD_BYTE 0x20 /**< @brief Read Command Byte */
#define KBC_WRITE_CMD_BYTE 0x60 /**< @brief Write Command Byte */

#define KBC_ENABLE_KBD_INT BIT(0) /**< @brief Enable keyboard interrupts on OBF */

#define KBC_FORWARD_TO_MOUSE 0xD4 /**< @brief Write Byte to Mouse */

#define TWOB_SCAN 0xE0 /**< @brief First byte of a two byte scancode */
#define BREAK(n) (n | BIT(7)) 
#define ESC 0x01
#define W_KEY 0x11
#define S_KEY 0x1f
#define A_KEY 0x1e
#define D_KEY 0x20
#define UP_KEY 0x48
#define DOWN_KEY 0x50
#define LEFT_KEY 0x4b
#define RIGHT_KEY 0x4d
#define E_KEY 0x12
#define Q_KEY 0x10
#define R_KEY 0x13
#define T_KEY 0x14
#define Y_KEY 0x15
#define U_KEY 0x16
#define I_KEY 0x17
#define O_KEY 0x18
#define P_KEY 0x19
#define F_KEY 0x21
#define G_KEY 0x22
#define H_KEY 0x23
#define J_KEY 0x24
#define K_KEY 0x25
#define L_KEY 0x26
#define Z_KEY 0x2C
#define X_KEY 0x2D
#define C_KEY 0x2E
#define V_KEY 0x2F
#define B_KEY 0x30
#define N_KEY 0x31
#define M_KEY 0x32
#define ENTER_KEY 0x1C
#define BACKSPACE_KEY 0x0E

#define DELAY_US 20000
#define KBC_SLEEP tickdelay(micros_to_ticks(DELAY_US));

#endif
