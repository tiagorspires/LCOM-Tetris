#include <lcom/lcf.h>

#include <stdint.h>


int (util_sys_inb)(int port, uint8_t *value){
    if (value == NULL) return 1;
    uint32_t val;
    if (sys_inb(port,&val)) return 1;
    *value = val & 0xFF;
    return 0;
}

int get_lsb(uint16_t value, uint8_t *lsb){
    if (lsb == NULL) return 1;
    *lsb = value & 0xFF;
    return 0;
}

int get_msb(uint16_t value, uint8_t *msb){
    if(msb == NULL) return 1;
    *msb = value >> 8;
    return 0;
}

