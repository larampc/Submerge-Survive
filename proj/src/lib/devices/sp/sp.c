#include <lcom/lcf.h>
#include "sp.h"
#include "UART.h"
#include "../../queue.h"
#include "../../../game/handler.h"

static uint16_t address = COM1;
static int sp_hook_id;

static queue_t *write_queue = NULL;
static queue_t *read_queue = NULL;
bool can_read = true;
uint16_t more_bytes = 0x0;
static bool irq_interrupts_enabled = true;

int (sp_subscribe_int)(uint8_t *bit_no) {
    if(bit_no == NULL) return 1;
    sp_hook_id = *bit_no;
    *bit_no = BIT(*bit_no);
    if (sys_irqsetpolicy(COM1_IRQ, IRQ_REENABLE|IRQ_EXCLUSIVE, &sp_hook_id) != 0){
        printf("ERROR: unable to set serial port policy inside %s\n", __func__);
        return 1;
    }
    return 0;
}

int (sp_unsubscribe_int)() {
    if (sys_irqrmpolicy(&sp_hook_id)){
        printf("ERROR: unable to remove serial port policy inside %s\n", __func__);
        return 1;
    }
    return 0;
}

int (sp_enable_irq_interrupts)() {
    if (irq_interrupts_enabled) return 0;
    irq_interrupts_enabled = true;
    return sys_irqenable(&sp_hook_id);
}

int (sp_disable_irq_interrupts)() {
    if (!irq_interrupts_enabled) return 0;
    irq_interrupts_enabled = false;
    return sys_irqdisable(&sp_hook_id);
}

int (sp_init)() {
    write_queue = new_queue(20);
    if (write_queue == NULL) {
        printf("ERROR: unable to create write queue inside %s\n", __func__);
        return 1;
    }
    read_queue = new_queue(20);
    if (read_queue == NULL) {
        printf("ERROR: unable to create read queue inside %s\n", __func__);
        return 1;
    }

    uint8_t lcr;  
    if (sp_read_lcr(&lcr)) {
        printf("ERROR: unable to read lcr inside %s\n", __func__);
        return 1;
    }

    if (!(lcr & UART_LCR_DLAB)) {
        lcr |= UART_LCR_DLAB;
        if (sp_write_lcr(lcr)) {
            printf("ERROR: unable to set DLAB in LCR inside %s\n", __func__);
            return 1;
        }
    }

    uint8_t bll;
    uint8_t blm;
    if (util_get_LSB(UART_LCR_BITRATE, &bll)) {
        printf("ERROR: unable to get bitrate less significant bits inside %s\n", __func__);
        return 1;
    }
    if (util_get_MSB(UART_LCR_BITRATE, &blm)) {
        printf("ERROR: unable to get bitrate most significant bits inside %s\n", __func__);
        return 1;
    }
    if (sys_outb(address + UART_DLL, bll)) {
        printf("ERROR: unable to set bitrate less significant bits inside %s\n", __func__);
        return 1;
    }
    if (sys_outb(address + UART_DLM, blm)) {
        printf("ERROR: unable to set bitrate most significant bits inside %s\n", __func__);
        return 1;
    }

    if (sp_read_lcr(&lcr)) {
        printf("ERROR: unable to read lcr inside %s\n", __func__);
        return 1;
    }
    lcr &= ~(UART_LCR_WORD_LENGTH | UART_LCR_STOP_BIT | UART_LCR_PARITY);
    lcr |= (UART_LCR_PARITY_ODD | UART_LCR_8_WORD_LENGTH | UART_LCR_1_STOP_BIT);
    if (sp_write_lcr(lcr)) {
        printf("ERROR: unable to set parity/stop bit/word length in LCR inside %s\n", __func__);
        return 1;
    }

    if (sys_outb(address + UART_FCR, UART_FCR_CONFIG)) {
        printf("ERROR: unable to set config in FCR inside %s\n", __func__);
        return 1;
    }


    if (sp_read_lcr(&lcr)) {
        printf("ERROR: unable to read lcr inside %s\n", __func__);
        return 1;
    }
    if (lcr & UART_LCR_DLAB) {
        lcr &= ~UART_LCR_DLAB;
        if (sp_write_lcr(lcr)) {
            printf("ERROR: unable to set DLAB in LCR inside %s\n", __func__);
            return 1;
        }
    }
    if (sys_outb(address + UART_IER, (UART_IER_RDA | UART_IER_RLS | UART_IER_THRE))) {
        printf("ERROR: unable to set enable interrupts in IER inside %s\n", __func__);
        return 1;
    }

    if (sp_read_fifo()) {
        printf("ERROR: unable to read read fifo inside %s\n", __func__);
        return 1;
    }
    uint8_t tmp;
    while (!is_empty_queue(read_queue)) {
        pop_queue(read_queue, &tmp);
    }
    return 0;
}

void (delete_sp)() {
    if (sp_read_fifo()) {
        printf("ERROR: unable to read read fifo inside %s\n", __func__);
    }
    delete_queue(write_queue);
    delete_queue(read_queue);
}

int (sp_read_lcr)(uint8_t *lcr) {
    if (lcr == NULL) {
        printf("ERROR: given lcr value of NULL inside %s\n", __func__);
        return 1;
    }
    if (util_sys_inb(address + UART_LCR, lcr)) return 1;
    return 0;
}

int(sp_write_lcr)(uint8_t lcr) {
    if (sys_outb(address + UART_LCR, lcr)) return 1;
    return 0;
}

int (sp_read_fifo)() {
    uint8_t lsr;
    if (sp_read_ls(&lsr)) {
        printf("ERROR: unable to read line status from LSR inside %s\n", __func__);
        return 1;
    }
    uint8_t data;
    while(lsr & UART_LSR_RR) {
        if (sp_read_data(&data)) {
            printf("ERROR: unable to read data from RBR inside %s\n", __func__);
            return 1;
        }
        if (push_queue(read_queue, data)) {
            printf("ERROR: unable to push to read queue inside %s\n", __func__);
            return 1;
        }
        if (sp_read_ls(&lsr)) {
            printf("ERROR: unable to read line status from LSR inside %s\n", __func__);
            return 1;
        }
        //printf("READ FROM FIFO: %x\n", data);
    }
    return EXIT_SUCCESS;
}

int (sp_write_fifo)() {
    sp_read_fifo();
    uint8_t lsr;
    if (sp_read_ls(&lsr)) {
        printf("ERROR: unable to read line status from LSR inside %s\n", __func__);
        return 1;
    }
    uint8_t data;
    while (lsr & UART_LSR_THRE) {
        if (is_empty_queue(write_queue)) break;
        if (pop_queue(write_queue, &data)) {
            printf("ERROR: unable to pop write queue inside %s\n", __func__);
            return 1;
        }
        if (sp_write_data(data)) {
            printf("ERROR: unable to write data in THR inside %s\n", __func__);
            return 1;
        }
        can_read = false;
        if (sp_read_ls(&lsr)) {
            printf("ERROR: unable to read line status from LSR inside %s\n", __func__);
            return 1;
        }
    }
    return 0;
}

int(sp_read_ls)(uint8_t *status) {
    if (status == NULL) {
        printf("ERROR: given status value of NULL inside %s\n", __func__);
        return 1;
    }
    if (util_sys_inb(address + UART_LSR, status)) {
        printf("ERROR: unable to read line status from LSR inside %s\n", __func__);
        return 1;
    }
    return 0;
}

int (sp_read_data)(uint8_t *data) {
    if (data == NULL) {
        printf("ERROR: given data value of NULL inside %s\n", __func__);
        return 1;
    }
    if (util_sys_inb(address + UART_RBR, data)) {
        printf("ERROR: unable to read data from RBR inside %s\n", __func__);
        return 1;
    }
    return 0;
}

int (sp_write_data)(uint8_t data) {
    if (sys_outb(address + UART_THR, data)) {
        printf("ERROR: unable to write data in THR inside %s\n", __func__);
        return 1;
    }
    return 0;
}

void (sp_ih)(Game* game) {
    uint8_t ii;
    if (util_sys_inb(address + UART_IIR, &ii)) {
        printf("ERROR: unable to read interruption identification from IIR inside %s\n", __func__);
        return;
    }
    uint8_t lsr;
    if (!(ii & UART_IIR_NOT_PENDING)) {
        switch ((ii & UART_IIR_IO)) {
            case UART_IIR_IO_RDA:                                                   //Received data
                if ((can_read || (game == NULL)) && sp_read_fifo()) {
                    printf("ERROR: unable to read fifo inside %s\n", __func__);
                    return;
                }
                break;
            case UART_IIR_IO_TE:                                                    //Transmitter is empty
            can_read = true;
                if ((game != NULL) && sp_write_fifo()) {
                    printf("ERROR: unable to write fifo inside %s\n", __func__);
                    return;
                }
                break;
            case UART_IIR_IO_CTI:                                                   //Character timedout
                if ((can_read || (game == NULL)) && sp_read_fifo()) {
                    printf("ERROR: unable to read fifo inside %s\n", __func__);
                    return;
                }
                break;
            case UART_IIR_IO_LS:                                                    //ERROR (line status)
                printf("ERROR: receive interrupt of type error\n");
                if (sp_read_ls(&lsr)) {
                    printf("ERROR: unable to read line status from LSR inside %s\n", __func__);
                    return;
                }
                if (lsr & UART_LSR_OE) {
                    printf("ERROR: overrun Error inside %s\n", __func__);
                    return;
                }
                if (lsr & UART_LSR_PE) {
                    printf("ERROR: parity Error inside %s\n", __func__);
                    return;
                }
                if (lsr & UART_LSR_FE) {
                    printf("ERROR: framing Error inside %s\n", __func__);
                    return;
                }
                return;
            default:
                printf("ERROR: receive unknown interrupt %x\n", ii);
                return;
        }
        //EXIT SUCCESS HERE
        sp_read_rq(game);
        return;
    }
}

int (sp_read_rq)(Game* game) {
    uint8_t byte;
    while (!is_empty_queue(read_queue)) {
        if (more_bytes != 0x0) {
            byte = (more_bytes >> 8);
        }
        else if (pop_queue(read_queue, &byte)) {
            printf("ERROR: unable to pop read queue inside %s\n", __func__);
            return 1;
        }
        switch (byte & HEADER_MASK) {
            case CONNECT_HEADER:
                connect_handler(game, byte & MESSAGE_MASK);
                break;
            case PLAYER_HEADER:
                player_handler(game, byte & MESSAGE_MASK);
                break;
            case SUB_HEADER:
                sub_handler(game, byte & MESSAGE_MASK);
                break;
            case CANNON_HEADER:
                cannon_handler(game, byte & MESSAGE_MASK, read_queue);
                break;
            case HOOK_HEADER:
                hook_handler(game, byte & MESSAGE_MASK);
                break;
            case SHOP_HEADER:
                shop_handler(game, byte & MESSAGE_MASK);
                break;
            case DESTROY_HEADER:
                destroy_handler(game, byte & MESSAGE_MASK, read_queue);
            default:
                break;
        }
    }
    return 0;
}

int (sp_write_wq)(uint8_t byte) {
    if (push_queue(write_queue, byte)) return 1;
    return 0;
}
