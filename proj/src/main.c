#include <lcom/lcf.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "video.h"
#include "keyboard.h"
#include <lcom/timer.h>
#include "matrix.h"


extern int counter;

extern uint8_t scancode;


int main(int argc, char *argv[]) {
  lcf_set_language("EN-US");
 // lcf_trace_calls("/home/lcom/labs/proj/trace.txt");
  //lcf_log_output("/home/lcom/labs/proj/output.txt");
  if (lcf_start(argc, argv)) return 1;
  lcf_cleanup();
  return 0;
}

int (proj_main_loop) (int argc, char **argv) {

  CharColorMap colorMap[] = {
        {'T', 1},
        {'I', 2},
        {'L', 3}
    };

int mapSize = sizeof(colorMap) / sizeof(colorMap[0]);

    // Criando e inicializando a matriz de caracteres 32x24 para representar o layout da tela
    char screen[32][24];
    for (int i = 0; i < 32; i++) {
        for (int j = 0; j < 24; j++) {
            screen[i][j] = '-';
        }
    }

    // Adicionando peças de Tetris de exemplo na tela
    // Peça 'T'
    screen[5][10] = 'T';
    screen[5][11] = 'T';
    screen[5][12] = 'T';
    screen[6][11] = 'T';

    // Peça 'I'
    screen[10][15] = 'I';
    screen[11][15] = 'I';
    screen[12][15] = 'I';
    screen[13][15] = 'I';

    // Peça 'L'
    screen[20][5] = 'L';
    screen[21][5] = 'L';
    screen[22][5] = 'L';
    screen[22][6] = 'L';

    // Peça 'L' invertida
    screen[15][3] = 'L';
    screen[16][3] = 'L';
    screen[17][3] = 'L';
    screen[15][4] = 'L';


   if(set_frame_buffer(0x105)) return 1;
   if(set_video_mode(0x105)) return 1;


  for (int i = 0; i < 32; i++) {
        for (int j = 0; j < 24; j++) {
            int colorIndex = getColorIndex(screen[i][j], colorMap, mapSize); // Implemente essa função com base no seu mapeamento
            if (colorIndex != -1) { // Apenas desenha se houver uma cor válida (não é '-')
                if (draw_rectangle(j * 32, i * 32, 32, 32, colorIndex) != 0) return 1;
            }
        }
    }

    //if(timer_set_frequency(0, 30)) return 1;
    int ipc_status;
    uint8_t irq_set_keyboard;
    //uint8_t irq_set_timer;
    message msg;

    //if(timer_subscribe_int(&irq_set_timer)) return 1;
    int r;

    if(keyboard_subscribe(&irq_set_keyboard)) return 1;

    while (scancode != ESC_BREAK_CODE) { 
        if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
            printf("driver_receive failed with: %d", r);
            continue;
        }
        if (is_ipc_notify(ipc_status)) { /* received notification */
            switch (_ENDPOINT_P(msg.m_source)) {
            case HARDWARE:
                if (msg.m_notify.interrupts & irq_set_keyboard) {
                    kbc_ih_keyboard();
                }
            }
        }
    }
 

    //if (timer_unsubscribe_int()) return 1;
    if (keyboard_unsubscribe()) return 1;

    if(escape_key()) return 1;
    if(vg_exit()) return 1;
    
    return 0;
}

/*
      while (scancode != ESC_BREAK_CODE) { 
        if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
            printf("driver_receive failed with: %d", r);
            continue;
        }
        if (is_ipc_notify(ipc_status)) { 
            switch (_ENDPOINT_P(msg.m_source)) {
            case HARDWARE: 
                if (msg.m_notify.interrupts & irq_set_timer) { 
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
                break; 
            }
        } else { 
            
        }
}
*/



