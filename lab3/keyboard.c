<<<<<<< HEAD
#include "keyboard.h"

int keyboard_hook_id = 1;

int(keyboard_subscribe_int)(uint8_t *bit_no){
    if (bit_no == NULL) return 1;
    *bit_no = BIT(keyboard_hook_id);
    if (sys_irqsetpolicy(KEYBOARD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &keyboard_hook_id)) return 1;
    return 0;
}

int(keyboard_unsubscribe_int)(){
    if (sys_irqrmpolicy(&keyboard_hook_id)) return 1;
    return 0;
}




uint8_t scancode;

void (kbc_ih)() {

    uint8_t status;
    int attempts = 10;
    
    while (attempts > 0) {
        
        if (util_sys_inb(STATUS_PORT, &status)) return;
        
        if ((status & OUT_BUFFER)) { 

            if ((status & ERRORS)) {
                return;
            } else {
                
                if (util_sys_inb(DATA_PORT, &scancode))
                return;
            }
        }
        attempts--;
    }
    return;
}

=======
#include <lcom/lcf.h>
#include <keyboard.h>
#include <stdint.h>
#include "i8042.h"

int keyboard_hook_id = 1;
uint8_t scancode;

int (keyboard_subscribe)(uint8_t *bit_no){
    if (bit_no == NULL) return 1;
    *bit_no = BIT(keyboard_hook_id);
    if (sys_irqsetpolicy(IRQ_LINE_KEYBOARD,IRQ_REENABLE|IRQ_EXCLUSIVE,&keyboard_hook_id)) return 1;    
    return 0;
}

int (keyboard_unsubscribe) (){
    if(sys_irqrmpolicy(&keyboard_hook_id)) return 1;
    return 0;
}

void (kbc_ih_keyboard)() { // read scancode
    int tries = 10;
    uint8_t status;
    while(tries){
        if(util_sys_inb(STATUS_PORT, &status)) return;

        if(status & OUT_BUF_FULL) {
            if(status & (TIMEOUT_ERROR|PARITY_ERROR)) return;
            else{
                if (util_sys_inb(DATA_PORT,&scancode)) return;
            }
        }
    tries--;
    }
}



>>>>>>> 7712c45f7814e4fc95f87d258c5a71fa805279e8
