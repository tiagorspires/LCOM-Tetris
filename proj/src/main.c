#include <lcom/lcf.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "video.h"
#include "keyboard.h"
#include <lcom/timer.h>
//#include "tetris.h" 

//#include "sprite.h"

extern int counter;

extern uint8_t scancode;


static char *i_piece_scaled_xpm[] = {
"40 40 1",  // Width, Height, Number of Colors
"c 1",      // Cyan color
"cccccccccccccccccccccccccccccccccccccccc",
"cccccccccccccccccccccccccccccccccccccccc",
"cccccccccccccccccccccccccccccccccccccccc",
"cccccccccccccccccccccccccccccccccccccccc",
"cccccccccccccccccccccccccccccccccccccccc",
"cccccccccccccccccccccccccccccccccccccccc",
"cccccccccccccccccccccccccccccccccccccccc",
"cccccccccccccccccccccccccccccccccccccccc",
"cccccccccccccccccccccccccccccccccccccccc",
"cccccccccccccccccccccccccccccccccccccccc",
"cccccccccccccccccccccccccccccccccccccccc",
"cccccccccccccccccccccccccccccccccccccccc",
"cccccccccccccccccccccccccccccccccccccccc",
"cccccccccccccccccccccccccccccccccccccccc",
"cccccccccccccccccccccccccccccccccccccccc",
"cccccccccccccccccccccccccccccccccccccccc",
"cccccccccccccccccccccccccccccccccccccccc",
"cccccccccccccccccccccccccccccccccccccccc",
"cccccccccccccccccccccccccccccccccccccccc",
"cccccccccccccccccccccccccccccccccccccccc",
"cccccccccccccccccccccccccccccccccccccccc",
"cccccccccccccccccccccccccccccccccccccccc",
"cccccccccccccccccccccccccccccccccccccccc",
"cccccccccccccccccccccccccccccccccccccccc",
"cccccccccccccccccccccccccccccccccccccccc",
"cccccccccccccccccccccccccccccccccccccccc",
"cccccccccccccccccccccccccccccccccccccccc",
"cccccccccccccccccccccccccccccccccccccccc",
"cccccccccccccccccccccccccccccccccccccccc",
"cccccccccccccccccccccccccccccccccccccccc",
"cccccccccccccccccccccccccccccccccccccccc",
"cccccccccccccccccccccccccccccccccccccccc",
"cccccccccccccccccccccccccccccccccccccccc",
"cccccccccccccccccccccccccccccccccccccccc",
"cccccccccccccccccccccccccccccccccccccccc",
"cccccccccccccccccccccccccccccccccccccccc",
"cccccccccccccccccccccccccccccccccccccccc",
"cccccccccccccccccccccccccccccccccccccccc",
"cccccccccccccccccccccccccccccccccccccccc",
"cccccccccccccccccccccccccccccccccccccccc"
};







int main(int argc, char *argv[]) {
  lcf_set_language("EN-US");
 // lcf_trace_calls("/home/lcom/labs/proj/trace.txt");
  //lcf_log_output("/home/lcom/labs/proj/output.txt");
  if (lcf_start(argc, argv)) return 1;
  lcf_cleanup();
  return 0;
}

int (proj_main_loop) (int argc, char **argv) {

   if(set_frame_buffer(0x105)) return 1;
   if(set_video_mode(0x105)) return 1;

    if(timer_set_frequency(0, 30)) return 1;
    int ipc_status;
    uint8_t irq_set_timer, irq_set_keyboard;
    message msg;

    if(keyboard_subscribe(&irq_set_keyboard)) return 1;
    if(timer_subscribe_int(&irq_set_timer)) return 1;
    int r;
   // int buffer = 10;
    int x = 500, y = 100;
    if(draw_xpm(x, y, (xpm_map_t) i_piece_scaled_xpm)) return 1;


       while (scancode != ESC_BREAK_CODE) { 
        if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
            printf("driver_receive failed with: %d", r);
            continue;
        }
        if (is_ipc_notify(ipc_status)) { /* received notification */
            switch (_ENDPOINT_P(msg.m_source)) {
            case HARDWARE: /* hardware interrupt notification */
                if (msg.m_notify.interrupts & irq_set_timer) { /* subscribed interrupt */
                timer_int_handler();
                if (counter%1000==0) {
                    //buffer--;
                   //if(draw_xpm(x, 100, (xpm_map_t) i_piece_scaled_xpm)) return 1;
                   //x+=100;   
                }
                }
                if (msg.m_notify.interrupts & irq_set_keyboard) {
                    kbc_ih_keyboard();
                    switch (scancode)
                    {
                    case A_BREAK_CODE:
                      x-=100;
                      //printf("%x", scancode);
                     clean_buffer();
                     if(draw_xpm(x, y, (xpm_map_t) i_piece_scaled_xpm)) return 1;
                      break;
                    case D_BREAK_CODE:
                      x+=100;
                      //printf("%x", scancode);
                      clean_buffer();
                     if(draw_xpm(x, y, (xpm_map_t) i_piece_scaled_xpm)) return 1;
                      break;
                    case S_BREAK_CODE:
                      y+=100;
                      //printf("%x", scancode);
                      clean_buffer();
                     if(draw_xpm(x, y, (xpm_map_t) i_piece_scaled_xpm)) return 1;
                      break;
                    case W_BREAK_CODE:
                      y-=100;
                      //printf("%x", scancode);
                      clean_buffer();
                     if(draw_xpm(x, y, (xpm_map_t) i_piece_scaled_xpm)) return 1;
                      break;
                    default:
                      break;
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
    printf("Exiting\n");
    printf("%x", scancode);

    if (timer_unsubscribe_int()) return 1;
    if (keyboard_unsubscribe()) return 1;

    if(escape_key()) return 1;
    if(vg_exit()) return 1;
    
    return 0;
}

