#include <lcom/lcf.h>
<<<<<<< HEAD
#include <stdint.h>

int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
  if (lsb == NULL) return 1; 
  *lsb = 0xFF & val;         

  return 0;
=======

#include <stdint.h>

int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
 if (lsb == NULL) return 1;
 *lsb = val & 0xFF;
 return 0;
>>>>>>> 7712c45f7814e4fc95f87d258c5a71fa805279e8
}

int(util_get_MSB)(uint16_t val, uint8_t *msb) {
  if (msb == NULL) return 1;
  *msb = val >> 8;
<<<<<<< HEAD

=======
>>>>>>> 7712c45f7814e4fc95f87d258c5a71fa805279e8
  return 0;
}

int (util_sys_inb)(int port, uint8_t *value) {
<<<<<<< HEAD
  if (value == NULL) return 1;    
  uint32_t temp;                   
  int ret = sys_inb(port, &temp);  

  *value = 0xFF & temp;
  return ret;
=======
  if (value == NULL) return 1;
  uint32_t val;
  if(sys_inb(port, &val)) return 1;
  *value = val & 0xFF;
  return 0;
>>>>>>> 7712c45f7814e4fc95f87d258c5a71fa805279e8
}
