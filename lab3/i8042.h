#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#include <lcom/lcf.h>

<<<<<<< HEAD
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
=======
#define IRQ_LINE_KEYBOARD 1
#define OUT_BUF_FULL BIT(0)
#define IN_BUF_FULL BIT(1)
#define TIMEOUT_ERROR BIT(6)
#define PARITY_ERROR BIT(7)
#define ERROR (TIMEOUT_ERROR|PARITY_ERROR)

#define DATA_PORT 0X60
#define STATUS_PORT 0X64
#define READ_COMMAND_BYTE_ALERT 0X20
#define WRITE_COMMAND_BYTE_ALERT 0X60
#define WRITE_COMMAND 0X60
#define READ_COMMAND_ADRESS 0X64


#define DELAY_US 20000

#define ESC_BREAK_CODE 0x81
#define TWO_BYTES 0XE0

#define BREAK_CODE BIT(7)

#define MOUSE BIT(5)

#define KEYBOARD_INTERRUPT_ENABLE BIT(0)

#endif
>>>>>>> 7712c45f7814e4fc95f87d258c5a71fa805279e8
