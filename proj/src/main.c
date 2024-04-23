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

static char *i_piece_scaled_xpm[] = {
"32 100 2",  // Width, Height, Number of Colors
". 0",       // Background
"c 1",       // Cyan color
"cccccccccccccccccccccccccccccccc",
"cccccccccccccccccccccccccccccccc",
"cccccccccccccccccccccccccccccccc",
"cccccccccccccccccccccccccccccccc",
"cccccccccccccccccccccccccccccccc",
"cccccccccccccccccccccccccccccccc",
"cccccccccccccccccccccccccccccccc",
"cccccccccccccccccccccccccccccccc",
"cccccccccccccccccccccccccccccccc",
"cccccccccccccccccccccccccccccccc",
"cccccccccccccccccccccccccccccccc",
"cccccccccccccccccccccccccccccccc",
"cccccccccccccccccccccccccccccccc",
"cccccccccccccccccccccccccccccccc",
"cccccccccccccccccccccccccccccccc",
"cccccccccccccccccccccccccccccccc",
"cccccccccccccccccccccccccccccccc",
"cccccccccccccccccccccccccccccccc",
"cccccccccccccccccccccccccccccccc",
"cccccccccccccccccccccccccccccccc",
"cccccccccccccccccccccccccccccccc",
"cccccccccccccccccccccccccccccccc",
"cccccccccccccccccccccccccccccccc",
"cccccccccccccccccccccccccccccccc",
"cccccccccccccccccccccccccccccccc",
"cccccccccccccccccccccccccccccccc",
"cccccccccccccccccccccccccccccccc",
"cccccccccccccccccccccccccccccccc",
"cccccccccccccccccccccccccccccccc",
"cccccccccccccccccccccccccccccccc",
"cccccccccccccccccccccccccccccccc",
"cccccccccccccccccccccccccccccccc",
"cccccccccccccccccccccccccccccccc",
"cccccccccccccccccccccccccccccccc",
"cccccccccccccccccccccccccccccccc",
"cccccccccccccccccccccccccccccccc",
"cccccccccccccccccccccccccccccccc",
"cccccccccccccccccccccccccccccccc",
"cccccccccccccccccccccccccccccccc",
"cccccccccccccccccccccccccccccccc",
"cccccccccccccccccccccccccccccccc",
"cccccccccccccccccccccccccccccccc",
"cccccccccccccccccccccccccccccccc",
"cccccccccccccccccccccccccccccccc",
"cccccccccccccccccccccccccccccccc",
"cccccccccccccccccccccccccccccccc",
"cccccccccccccccccccccccccccccccc",
"cccccccccccccccccccccccccccccccc",
"cccccccccccccccccccccccccccccccc",
"cccccccccccccccccccccccccccccccc",
"cccccccccccccccccccccccccccccccc",
"cccccccccccccccccccccccccccccccc",
"cccccccccccccccccccccccccccccccc",
"cccccccccccccccccccccccccccccccc",
"cccccccccccccccccccccccccccccccc",
"cccccccccccccccccccccccccccccccc",
"cccccccccccccccccccccccccccccccc",
"cccccccccccccccccccccccccccccccc",
"cccccccccccccccccccccccccccccccc",
"cccccccccccccccccccccccccccccccc",
"cccccccccccccccccccccccccccccccc",
"cccccccccccccccccccccccccccccccc",
"cccccccccccccccccccccccccccccccc",
"cccccccccccccccccccccccccccccccc",
"cccccccccccccccccccccccccccccccc",
"cccccccccccccccccccccccccccccccc",
"cccccccccccccccccccccccccccccccc",
"cccccccccccccccccccccccccccccccc",
"cccccccccccccccccccccccccccccccc",
"cccccccccccccccccccccccccccccccc",
"cccccccccccccccccccccccccccccccc",
"cccccccccccccccccccccccccccccccc",
"cccccccccccccccccccccccccccccccc",
"cccccccccccccccccccccccccccccccc",
"cccccccccccccccccccccccccccccccc",
"cccccccccccccccccccccccccccccccc",
"cccccccccccccccccccccccccccccccc",
"cccccccccccccccccccccccccccccccc",
"cccccccccccccccccccccccccccccccc",
"cccccccccccccccccccccccccccccccc",
"cccccccccccccccccccccccccccccccc",
"cccccccccccccccccccccccccccccccc",
"cccccccccccccccccccccccccccccccc",
"cccccccccccccccccccccccccccccccc",
"cccccccccccccccccccccccccccccccc",
"cccccccccccccccccccccccccccccccc",
"cccccccccccccccccccccccccccccccc",
"cccccccccccccccccccccccccccccccc",
"cccccccccccccccccccccccccccccccc",
"cccccccccccccccccccccccccccccccc",
"cccccccccccccccccccccccccccccccc",
"cccccccccccccccccccccccccccccccc",
"cccccccccccccccccccccccccccccccc",
"cccccccccccccccccccccccccccccccc",
"cccccccccccccccccccccccccccccccc",
"cccccccccccccccccccccccccccccccc",
"cccccccccccccccccccccccccccccccc",
"cccccccccccccccccccccccccccccccc",
"cccccccccccccccccccccccccccccccc",
"cccccccccccccccccccccccccccccccc"
};






int main(int argc, char *argv[]) {
  lcf_set_language("EN-US");
  lcf_trace_calls("/home/lcom/labs/proj/trace.txt");
  lcf_log_output("/home/lcom/labs/proj/output.txt");
  if (lcf_start(argc, argv)) return 1;
  lcf_cleanup();
  return 0;
}

int (proj_main_loop) (int argc, char **argv) {

    if(set_frame_buffer(0x105)) return 1;
    if(set_video_mode(0x105)) return 1;

    if(timer_set_frequency(0, 30)) return 1;
    int ipc_status;
    uint8_t irq_set;
    message msg;

    if(timer_subscribe_int(&irq_set)) return 1;
    int buffer = 10, r;
    int x = 0;
    while (buffer) { 
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
                    buffer--;
                    if(draw_xpm(x, 100, (xpm_map_t) i_piece_scaled_xpm)) return 1;
                    x+=100;   
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

    if(escape_key()) return 1;
    if(vg_exit()) return 1;
    return 0;
}

