#include <lcom/lcf.h>
#include <lcom/lab2.h>

#include <stdbool.h>
#include <stdint.h>

<<<<<<< HEAD
extern int counter;
=======
#include "i8254.h"

>>>>>>> 7712c45f7814e4fc95f87d258c5a71fa805279e8

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
<<<<<<< HEAD
  lcf_trace_calls("/Users/eamachado/MINIX-LCOM/shared/g6/lab2/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/Users/eamachado/MINIX-LCOM/shared/g6/lab2/output.txt");
=======
  lcf_trace_calls("/home/lcom/labs/lab2/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab2/output.txt");
>>>>>>> 7712c45f7814e4fc95f87d258c5a71fa805279e8

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(timer_test_read_config)(uint8_t timer, enum timer_status_field field) {
<<<<<<< HEAD
  uint8_t configuration;                                              // variável que vai conter a configuração do timer
  if (timer_get_conf(timer, &configuration) != 0) return 1;           // chamar a função que preenche a configuração
  if (timer_display_conf(timer, configuration, field) != 0) return 1; // display das configurações segundo o field
=======
  uint8_t conf;
  if (timer_get_conf(timer, &conf)) return 1;
  if (timer_display_conf(timer,conf, field)) return 1;
>>>>>>> 7712c45f7814e4fc95f87d258c5a71fa805279e8
  return 0;
}

int(timer_test_time_base)(uint8_t timer, uint32_t freq) {
<<<<<<< HEAD
  if (freq < 19 || timer > 2) return 1;                 // se o timer não for válido ou a frequência menor que 19 aborta a missão
  if (timer_set_frequency(timer, freq) != 0) return 1;  // muda a frequência
  return 0;
}                                                                             

int(timer_test_int)(uint8_t time) {
  
  int ipc_status, r;
  uint8_t irq_set;
  message msg;

  if(timer_subscribe_int(&irq_set) != 0) return 1;
  
  while(time > 0) { /* time  */
     /* Get a request message. */
     if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
         printf("driver_receive failed with: %d", r);
         continue;
     }
     if (is_ipc_notify(ipc_status)) { /* received notification */
         switch (_ENDPOINT_P(msg.m_source)) {
            case HARDWARE: /* hardware interrupt notification */                
                if (msg.m_notify.interrupts & irq_set) { /* subscribed interrupt */
                   timer_int_handler(); 
                   if(counter%60==0){
                      timer_print_elapsed_time();
                      time--;
                   }
                }
              break;
            default:
              break; /* no other notifications expected: do nothing */    
         }
     } else { /* received a standard message, not a notification */
         /* no standard messages expected: do nothing */
     }
  }

  if (timer_unsubscribe_int() != 0) return 1;
=======
  if (freq > TIMER_FREQ || freq < 19 || timer < 0 || timer > 2) return 1;
  if(timer_set_frequency(timer,freq)) return 1;
  return 0;
}

extern int counter;

int(timer_test_int)(uint8_t time) {
  int ipc_status, r;
message msg;
 uint8_t irq_set;
 if(timer_subscribe_int(&irq_set)) return 1;
while (time > 0) { 
  if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
    printf("driver_receive failed with: %d", r);
    continue;
  }
  if (is_ipc_notify(ipc_status)) { /* received notification */
    switch (_ENDPOINT_P(msg.m_source)) {
      case HARDWARE: /* hardware interrupt notification */
        if (msg.m_notify.interrupts & irq_set) { /* subscribed interrupt */
          timer_int_handler();
          if (counter%60==0) {
            timer_print_elapsed_time();
            time--;
          }
        }
        break;
      default:
        break; /* no other notifications expected: do nothing */
    }
  } else { /* received a standard message, not a notification */
    /* no standard messages expected: do nothing */
  }
}
  if (timer_unsubscribe_int()) return 1;
>>>>>>> 7712c45f7814e4fc95f87d258c5a71fa805279e8
  return 0;
}
