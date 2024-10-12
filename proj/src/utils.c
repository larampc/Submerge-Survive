#include <lcom/lcf.h>
#include <stdint.h>

int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
    if (!lsb) return 1;
    *lsb = (uint8_t) val;
    return 0;
}

int(util_get_MSB)(uint16_t val, uint8_t *msb) {
    if(!msb) return 1;
    *msb = (uint8_t)(val >> 8);
    return 0;
}

int (util_sys_inb)(int port, uint8_t *value) {
    if(!value) return 1;
    uint32_t res;
    int ret = sys_inb(port, &res);
    *value = (uint8_t)(res);
    return ret;
}
