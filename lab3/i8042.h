#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#include <lcom/lcf.h>

#define IRQ_LINE_KEYBOARD 1
#define OUT_BUF_FULL BIT(0)
#define IN_BUF_FULL BIT(1)
#define TIMEOUT_ERROR BIT(6)
#define PARITY_ERROR BIT(7)

#define DATA_PORT 0X60
#define STATUS_PORT 0X64

#define DELAY_US    20000

#define ESC_BREAK_CODE 0x81
#define TWO_BYTES 0XE0

#define BREAK_CODE BIT(7)

#endif
