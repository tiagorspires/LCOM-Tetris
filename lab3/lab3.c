#include <lcom/lcf.h>
#include <lcom/lab3.h>

#include "keyboard.h"
#include "i8042.h"
#include "i8254.h"
#include "timer.c"

#include <stdbool.h>
#include <stdint.h>

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab3/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab3/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}



extern uint8_t scancode;
extern int counter;

int(kbd_test_scan)() {

  int ipc_status, r;
  message msg;
  uint8_t irq_set;

  if (keyboard_subscribe_int(&irq_set)) return 1;

  while (scancode != ESC_BREAKCODE) { /* You may want to use a different condition */
    /* Get a request message. */
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) { /* received notification */
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE: /* hardware interrupt notification */
          if (msg.m_notify.interrupts & irq_set) { /* subscribed interrupt */
            
              kbc_ih();
              kbd_print_scancode(!(scancode & MAKE_CODE), scancode == TWO_BYTES ? 2 : 1, &scancode);
              
            }
          break;
        default:
          break;
          } /* no other notifications expected: do nothing */
      } else { /* received a standard message, not a notification */
      /* no standard messages expected: do nothing */
    }
  }

  if (keyboard_unsubscribe_int()) return 1;
  if (kbd_print_no_sysinb(counter)) return 1;
  return 0;
}



int(kbd_test_poll)() {

  while(scancode != ESC_BREAKCODE) {
  
  uint8_t status;
  int attempts = 10;

  while (attempts > 0) {

    if (util_sys_inb(STATUS_PORT, &status)) return 1;

    if ((status & OUT_BUFFER)) { 

      if ((status & ERRORS)) {
        return 1;

      } else {
        if (util_sys_inb(DATA_PORT, &scancode)) {
          return 1;

        } else {
          kbd_print_scancode(!(scancode & MAKE_CODE), scancode == TWO_BYTES ? 2 : 1, &scancode);
        }
      }
    }
    attempts--;
  }  
}
return 0;
}




extern int counter_timer;

int(kbd_test_timed_scan)(uint8_t n) {  

  int ipc_status, r;
  message msg;
  uint8_t irq_set;
  int seconds = 0;

  if (timer_subscribe_int(&irq_set)) return 1;
  if (keyboard_subscribe_int(&irq_set)) return 1;

  while (scancode != ESC_BREAKCODE && seconds < n) { /* You may want to use a different condition */
    /* Get a request message. */
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) { /* received notification */
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE: /* hardware interrupt notification */
          if (msg.m_notify.interrupts & irq_set) { /* subscribed interrupt */
            
              kbc_ih();
              kbd_print_scancode(!(scancode & MAKE_CODE), scancode == TWO_BYTES ? 2 : 1, &scancode);
              
            }

          if (msg.m_notify.interrupts & irq_set) { /* subscribed interrupt */
            timer_int_handler();
            if (counter_timer % 60 == 0) {
              seconds++;
            }
          }

          break;
        default:
          break;
          } /* no other notifications expected: do nothing */
      } else { /* received a standard message, not a notification */
      /* no standard messages expected: do nothing */
    }
  }

  if (timer_unsubscribe_int()) return 1;
  if (keyboard_unsubscribe_int()) return 1;
  if (kbd_print_no_sysinb(counter)) return 1;
  return 0;
}
