#ifndef _UART_H_
#define _UART_H_

#define COM1 0x3F8
#define COM1_IRQ 4
#define COM2 0x2F8
#define COM2_IRQ 3

#define UART_DLL 0x00
#define UART_DLM 0x01

#define UART_RBR 0x00       //Receiver Buffer Register
#define UART_THR 0x00       //Transmitter Holding Register
#define UART_IER 0x01       //Interrupt Enable Register
#define UART_IIR 0x02       //Interrupt Identification Register
#define UART_FCR 0x02       //FIFO Control Register
#define UART_LCR 0x03       //Line Control Register
#define UART_LSR 0x05       //Line Status Register

// In Interrupt Mode
#define UART_IER_RDA BIT(0)     //Receiver Data Available Interrupt
#define UART_IER_THRE BIT(1)    //Transmitter Holding Register Empty Interrupt
#define UART_IER_RLS BIT(2)     //Receiver Line Status Interrupt
#define UART_IER_MS BIT(3)      //Modem Status Interrupt

#define UART_LCR_WORD_LENGTH (BIT(0) | BIT(1))      //Word Length
#define UART_LCR_8_WORD_LENGTH 0x03                 //Word Length of 8 bits

#define UART_LCR_BITRATE 1                          //Bitrate

#define UART_LCR_STOP_BIT BIT(2)                    //Stop Bit
#define UART_LCR_1_STOP_BIT 0x00                    //Stop Bit of 1 bit

#define UART_LCR_PARITY (BIT(3) | BIT(4) | BIT(5))  //Parity
#define UART_LCR_PARITY_ODD BIT(3)                  //Parity of type odd

#define UART_LCR_DLAB BIT(7)                        //DLAB

// In Polled Mode
#define UART_LSR_RR BIT(0)                          //Receiver Ready
#define UART_LSR_OE BIT(1)                          //Overrun Error
#define UART_LSR_PE BIT(2)                          //Parity Error
#define UART_LSR_FE BIT(3)                          //Framing Error
#define UART_LSR_THRE BIT(5)                        //Transmitter Holding Register Empty

#define UART_IIR_NOT_PENDING BIT(0)                 //Interrupt Not Pending
#define UART_IIR_IO (BIT(1) | BIT(2) | BIT(3))      //Interrupt Origin
#define UART_IIR_IO_TE BIT(1)                       //Interrupt Transmitter Empty
#define UART_IIR_IO_CTI (BIT(2) | BIT(3))           //Interrupt Character Timeout Indication
#define UART_IIR_IO_RDA BIT(2)                      //Interrupt Received Data Available
#define UART_IIR_IO_LS (BIT(1) | BIT(2))            //Interrupt Line Status
#define UART_IIR_FIFO_STATUS (BIT(6) | BIT(7))      //Interrupt FIFO Status

#define UART_FCR_ENABLE BIT(0)                      //Enable FIFO
#define UART_FCR_CLEAR_RECEIVE BIT(1)               //Clear Receive FIFO
#define UART_FCR_CLEAR_TRANSMIT BIT(2)              //Clear Transmit FIFO
#define UART_FCR_ITL_4 BIT(6)                       //Interrupt Trigger Level
#define UART_FCR_CONFIG (UART_FCR_ENABLE | UART_FCR_CLEAR_RECEIVE | UART_FCR_CLEAR_TRANSMIT | UART_FCR_ITL_4)


#define HEADER_MASK 0xF0
#define MESSAGE_MASK 0x0F

#define CONNECT_HEADER 0xF0
#define PLAYER_HEADER 0x00
#define SUB_HEADER 0x10
#define CANNON_HEADER 0x20
#define HOOK_HEADER 0x30
#define SHOP_HEADER 0x40
#define DESTROY_HEADER 0x50


#define CONNECT_PLAYER_0 0x0
#define CONNECT_PLAYER_1 0x1
#define CONNECT_PLAYER_N(n) (0x0 + n)
#define CONNECT_START_GAME 0x2
#define QUIT_GAME 0x03
#define PING_CODE 0x04
#define RECEIVED_PING 0x05
#define START_GAME 0x06
#define GAME_OVER 0x07

#define PLAYER_MOVE_UP 0x1
#define PLAYER_MOVE_DOWN 0x2
#define PLAYER_MOVE_LEFT 0x3
#define PLAYER_MOVE_RIGHT 0x4
#define PLAYER_STOPPED 0x5
#define PLAYER_CANNON_1 0x6
#define PLAYER_CANNON_2 0x7
#define PLAYER_CANNON_3 0x8
#define PLAYER_CANNON_4 0x9
#define PLAYER_CANNON_5 0xA
#define PLAYER_CANNON_N(n) (0x06 + n)
#define PLAYER_DRIVE 0xB
#define PLAYER_NONE 0xC

#define BG_UP 0x1
#define BG_DOWN 0x2
#define BG_LEFT 0x3
#define BG_RIGHT 0x4
#define BG_UP_LEFT 0x5
#define BG_UP_RIGHT 0x6
#define BG_DOWN_LEFT 0x7
#define BG_DOWN_RIGHT 0x8

#define CANNON_ROTATE 0x1
#define CANNON_FIRE 0x2

#define HOOK_CLICK 0x8

#define DESTROY_FISH 0x01
#define DESTROY_TRASH 0x02
#define KILL_FISH 0x03
#define POP_TRASH 0x04
#define CATCH_TRASH 0x05
#define KILL_ENEMY 0x06
#define DESTROY_ENEMY_BULLET 0x07

#endif
