#include <lcom/lcf.h>

#include <stdint.h>

<<<<<<< HEAD
int counter = 0;

int(util_get_LSB)(uint16_t val, uint8_t *lsb){
    if (lsb == NULL) return 1;
    *lsb = (uint8_t) val;
    return 0;
}

int(util_get_MSB)(uint16_t val, uint8_t *msb){
    if (msb == NULL) return 1;
    *msb = (uint8_t) (val >> 8);
    return 0;
}

int(util_sys_inb)(int port, uint8_t *value){
    if (value == NULL) return 1;
    uint32_t val;
    if (sys_inb(port, &val)) return 1;
    counter++;
    *value = (uint8_t) val;
    return 0;
}
=======
extern int counter;

int (util_sys_inb)(int port, uint8_t *value){
    if (value == NULL) return 1;
    uint32_t val;
    if (sys_inb(port,&val)) return 1;
    counter++;
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

>>>>>>> 7712c45f7814e4fc95f87d258c5a71fa805279e8
