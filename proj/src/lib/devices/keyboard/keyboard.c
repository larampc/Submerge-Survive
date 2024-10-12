#include "lcom/lcf.h"
#include "keyboard.h"
#include "../kbc/i8042.h"
#include "../kbc/kbc.h"

static int kbd_hook_id;
uint8_t scancode;

int (keyboard_subscribe_int)(uint8_t *bit_no) {
    if(bit_no == NULL) return 1;
    kbd_hook_id = *bit_no;
    *bit_no = BIT(*bit_no);
    if (sys_irqsetpolicy(KBD_IRQ, IRQ_REENABLE|IRQ_EXCLUSIVE, &kbd_hook_id) != 0){
        printf("in keyboard_subscribe_int: failed to set keyboard policy\n");
        return 1;
    }
    return 0;
}

int (keyboard_unsubscribe_int)() {
    if (sys_irqrmpolicy(&kbd_hook_id) != 0){
        printf("in keyboard_unsubscribe_int: failed to remove keyboard policy\n");
    }
    return 0;
}

void (kbd_ih)() {
    uint8_t data;
    if(kbc_read_byte(&data, true) == 0) scancode = data;
}
