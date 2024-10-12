#ifndef _LCOM_PS2_H_
#define _LCOM_PS2_H_

/** @file */
/** @defgroup ps2 PS2
 * @brief Mouse handling module.
 * @{
 *
 */

#define MOUSE_ENABLE_DATA_REP 0xF4 /** @brief Mouse enable data reporting command */
#define MOUSE_DISABLE_DATA_REP 0xF5 /** @brief Mouse disable data reporting command */
#define MOUSE_SET_SAMPLE_RATE 0xF3 /** @brief Mouse set sample rate command */
#define MOUSE_GET_DEVICE_ID 0xF2 /** @brief Mouse get device ID command */
#define SCROLL_SUCCESS 0x03 /** @brief Scroll success value */

#define MOUSE_Y_OV BIT(7) /** @brief Mouse Y overflow bit */
#define MOUSE_X_OV BIT(6) /** @brief Mouse X overflow bit */
#define MOUSE_Y_SIGN BIT(5) /** @brief Mouse Y sign bit */
#define MOUSE_X_SIGN BIT(4) /** @brief Mouse X sign bit */
#define MOUSE_MB BIT(2) /** @brief Mouse middle button bit */
#define MOUSE_RB BIT(1) /** @brief Mouse right button bit */
#define MOUSE_LB BIT(0) /** @brief Mouse left button bit */

#define MOUSE_ACK 0xFA /** @brief Mouse acknowledgment value */

/**
 * @brief Structure for the mouse packet.
 */
typedef struct mouse_packet {  
  bool rb; /**< @brief Right button press status */
  bool mb; /**< @brief Middle button press status */
  bool lb; /**< @brief Left button press status */
  int16_t delta_x; /**< @brief Mouse x-displacement: rightwards is positive */
  int16_t delta_y; /**< @brief Mouse y-displacement: upwards is positive */
  int8_t delta_z; /**< @brief Mouse z-displacement: upwards is positive */
  bool x_ov; /**< @brief Mouse x-displacement overflow status */
  bool y_ov; /**< @brief Mouse y-displacement overflow status */
} mouse_packet;

#endif /* _LCOM_PS2_H_ */

