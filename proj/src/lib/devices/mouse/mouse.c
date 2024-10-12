#include "lcom/lcf.h"
#include "mouse.h"
#include "../kbc/kbc.h"
#include "../kbc/i8042.h"

static int mouse_hook_id;
int byte = 0;
uint8_t packet[4];
bool scroll = false;
static struct mouse_ev mouse_event;
uint8_t prev = 0;

int (mouse_subscribe_int)(uint8_t *bit_no) {
    if(bit_no == NULL) return 1;
    mouse_hook_id = *bit_no;
    *bit_no = BIT(*bit_no);
    if (sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE|IRQ_EXCLUSIVE, &mouse_hook_id) != 0){
        printf("in mouse_subscribe_int: failed to set mouse policy\n");
        return 1;
    }
    return 0;
}

int (mouse_unsubscribe_int)() {
    if (sys_irqrmpolicy(&mouse_hook_id) != 0){
        printf("in keyboard_unsubscribe_int: failed to remove keyboard policy\n");
    }
    return 0;
}

void (mouse_ih)() {
    uint8_t value;
    if (kbc_read_byte(&value, false) == 0) {
        if (byte != 0 || value & BIT(3)) {
            packet[byte] = value;
            byte++;
        }
    }
}

void (extend_sign)(int16_t* value) {
    if (*value & BIT(8)) {
        *value |= 0xFF00;
    }
}

void (assemble_packet)(mouse_packet* pp) {
    byte = 0;
    pp->y_ov = packet[0] & MOUSE_Y_OV;
    pp->x_ov = packet[0] & MOUSE_X_OV;
    pp->mb = packet[0] & MOUSE_MB;
    pp->rb = packet[0] & MOUSE_RB;
    pp->lb = packet[0] & MOUSE_LB;
    pp->delta_y = ((packet[0] & MOUSE_Y_SIGN) << 3) | packet[2];
    pp->delta_x = ((packet[0] & MOUSE_X_SIGN) << 4) | packet[1];
    pp->delta_z = packet[3];
    extend_sign(&pp->delta_y);
    extend_sign(&pp->delta_x);
    get_mouse_event(pp);
    prev = (pp->mb << 2) | (pp->rb << 1) | pp->lb;
}

void (get_mouse_event)(mouse_packet* pp) {
    if ((prev>>2) != pp->mb) mouse_event.type = BUTTON_EV;
    else if (((prev & MOUSE_RB)>>1) != pp->rb) {
        if (prev & MOUSE_RB) mouse_event.type = RB_RELEASED;
        else mouse_event.type = RB_PRESSED;
    }
    else if ((prev & MOUSE_LB) != pp->lb) {
        if (prev & MOUSE_LB) mouse_event.type = LB_RELEASED;
        else mouse_event.type = LB_PRESSED;
    }
    else {
        mouse_event.type = MOUSE_MOV;
        mouse_event.delta_x = pp->delta_x;
        mouse_event.delta_y = pp->delta_y;
    }
}

int (mouse_command)(uint8_t cmd) {
    for (int i = 0; i < 10; i++) {
        if (kbc_write_byte(KBC_CMD_REG, KBC_FORWARD_TO_MOUSE) != 0) return 1;
        if (kbc_write_byte(KBC_ARGS_REG, cmd) != 0) return 1;
        uint8_t value;
        if (util_sys_inb(KBC_OUT_BUF, &value) != 0) return 1;
        if (value == MOUSE_ACK) return 0;
    }
    return 1;
}

int (mouse_command_get_id)() {
    for (int i = 0; i < 10; i++) {
        if (kbc_write_byte(KBC_CMD_REG, KBC_FORWARD_TO_MOUSE) != 0) return 1;
        if (kbc_write_byte(KBC_ARGS_REG, MOUSE_GET_DEVICE_ID) != 0) return 1;
        uint8_t value;
        if (util_sys_inb(KBC_OUT_BUF, &value) != 0) return 1;
        if (value == 0x03) return 0;
    }
    return 1;
}

int (mouse_disable_reporting)() {
    return mouse_command(MOUSE_DISABLE_DATA_REP);
}

int (mouse_enable_reporting)() {
    return mouse_command(MOUSE_ENABLE_DATA_REP);
}

int (mouse_enable_scroll)() {
    if (mouse_command(MOUSE_SET_SAMPLE_RATE) != 0) return 1;
    if (mouse_command(0xc8) != 0) return 1;
    if (mouse_command(MOUSE_SET_SAMPLE_RATE) != 0) return 1;
    if (mouse_command(0x64) != 0) return 1;
    if (mouse_command(MOUSE_SET_SAMPLE_RATE) != 0) return 1;
    if (mouse_command(0x50) != 0) return 1;
    if (mouse_command_get_id() != 0) {
        if (mouse_command(MOUSE_SET_SAMPLE_RATE) != 0) return 1;
        if (mouse_command(0x28) != 0) return 1;
        printf("ERROR: Mouse with scroll not found");
        return 0;
    }
    scroll = true;
    if (mouse_command(MOUSE_SET_SAMPLE_RATE) != 0) return 1;
    if (mouse_command(0x28) != 0) return 1;
    return 0;
}
