// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>
#include "mouse.h"

#include <stdint.h>
#include <stdio.h>

// Any header files included below this line should have been created by you

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need/ it]
  lcf_trace_calls("/home/lcom/labs/lab4/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab4/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

extern uint32_t count;
extern bool update_count;

int (mouse_test_packet)(uint32_t cnt) {
  uint8_t bit_no;
  int irq_set,ipc_status,r;
  message msg;

  mouse_enable_data_reporting();
  mouse_subscribe_int(&bit_no);

  irq_set = BIT(bit_no);

  while(count<cnt){
    if( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE: 

          if (msg.m_notify.interrupts & irq_set) {
            mouse_ih();
          }

          break;
        default:
          break; 
      }
    }

    if(update_count){
      count++;
      update_count=false;
    }

  }
  mouse_unsubscribe_int();
  return 0;

}


int (mouse_test_async)(uint8_t idle_time) {
  uint8_t irq_set_aux,irq_set_timer;
  int ipc_status,r;
  message msg;

  if(mouse_enable_data_reporting()) return 1;
  if(mouse_subscribe_int(&irq_set_aux)) return 1;;
  if(timer_subscribe_int(&irq_set_timer)) return 1;

  while(count<idle_time*60){
    if( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE: 

          if (msg.m_notify.interrupts & irq_set_aux) {
            mouse_ih();
          }

          if (msg.m_notify.interrupts & irq_set_timer) {
            timer_int_handler();
          }

          break;
        default:
          break;
      }
    }

    if(update_count){
      count=0;
      update_count=false;
    }

  }
  mouse_unsubscribe_int();
  timer_unsubscribe_int();
  return 0;
}

