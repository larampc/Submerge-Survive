#include <lcom/lcf.h>
#include "i8042.h"
#include "kbc.h"

int (kbc_write_byte)(int port, uint8_t cmd) {
    uint8_t st;
    for (int i = 0; i < 10; i++) {
        if (util_sys_inb(KBC_STATUS_REG, &st) != 0) return 1;
        if ((st & KBC_IBF) == 0) {
            if (sys_outb(port, cmd) != 0) return 1;
            if (st & (KBC_PARITY_ERROR | KBC_TIMEOUT_ERROR) ) return 1;
            return 0;
        }
        KBC_SLEEP;
    }
    return 1;
}

int (kbc_read_byte)(uint8_t* value, bool keyboard) {
    if (value == NULL) return 1;
    for (int i = 0; i < 10; i++) {
        uint8_t st;
        if (util_sys_inb(KBC_STATUS_REG, &st) != 0) return 1;
        if (st & KBC_OBF) {
            if (util_sys_inb(KBC_OUT_BUF, value) != 0) return 1;
            if (st & (KBC_PARITY_ERROR | KBC_TIMEOUT_ERROR)) return 1;
            if ((keyboard && (st & KBC_AUX)) || (!keyboard && !(st & KBC_AUX))) return 1;
            return 0;
        }
        KBC_SLEEP;
    }
    return 1;
}
