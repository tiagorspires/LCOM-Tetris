#include <lcom/lcf.h>
#include "i8042.h"
#include <stdint.h>

int (keyboard_subscribe)(uint8_t *bit_no);
int (keyboard_unsubscribe) ();
void (kbc_ih_keyboard)();
int (escape_key) ();


