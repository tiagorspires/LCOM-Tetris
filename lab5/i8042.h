#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#include <lcom/lcf.h>

#define TIMER0_IRQ   0
#define KEYBOARD_IRQ 1
#define MOUSE_IRQ    12

#define DATA_PORT   0x60
#define STATUS_PORT 0x64

#define ESC_BREAKCODE 0x81
#define TWO_BYTES     0xE0

#define OUT_BUFFER  BIT(0) // 0 - empty, 1 - full
#define IN_BUFFER   BIT(1) // 0 - empty, 1 - full
#define BREAK_CODE  BIT(7)
#define ERRORS      (BIT(6) | BIT(7)) // 0 - no error, 1 - error

#define KBC_READ_CMD    0x20
#define KBC_WRITE_CMD   0x60

#define AUX             BIT(5)

#define LEFT_BUTTON     BIT(0)
#define RIGHT_BUTTON    BIT(1)
#define MIDDLE_BUTTON   BIT(2)
#define FIRST_BYTE      BIT(3)
#define X_SIGN          BIT(4)
#define Y_SIGN          BIT(5)
#define X_OVFL          BIT(6)
#define Y_OVFL          BIT(7)

#define SIGN_EXTENSION      0xFF00
#define BYTE_TO_MOUSE       0xD4
#define DIS_DATA_REPORTING  0xF5


#endif /* _LCOM_I8042_H */
