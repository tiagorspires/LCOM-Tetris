#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#include <lcom/lcf.h>

#define KEYBOARD_IRQ 1

#define DATA_PORT   0x60
#define STATUS_PORT 0x64

#define ESC_BREAKCODE 0x81
#define TWO_BYTES     0xE0

#define OUT_BUFFER BIT(0) // 0 - empty, 1 - full
#define IN_BUFFER  BIT(1) // 0 - empty, 1 - full
#define MAKE_CODE  BIT(7)
#define ERRORS     (BIT(6) | BIT(7)) // 0 - no error, 1 - error

#endif /* _LCOM_I8042_H */
