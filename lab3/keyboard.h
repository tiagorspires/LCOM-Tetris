#include <lcom/lcf.h>
#include "i8042.h"
#include <stdint.h>

<<<<<<< HEAD
int (keyboard_subscribe_int)(uint8_t *bit_no);
int (keyboard_unsubscribe_int)();
void (kbc_ih)();
=======
int (keyboard_subscribe)(uint8_t *bit_no);
int (keyboard_unsubscribe) ();
void (kbc_ih_keyboard)();


>>>>>>> 7712c45f7814e4fc95f87d258c5a71fa805279e8
