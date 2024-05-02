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
  //lcf_trace_calls("/home/lcom/labs/proj/trace.txt");
  //lcf_log_output("/home/lcom/labs/proj/output.txt");
  if (lcf_start(argc, argv)) return 1;
  lcf_cleanup();
  return 0;
}



static void move_left(char screen[24][32], CharColorMap *colorMap, int mapSize)
{
    for(int i = 0; i < 24; i++) {
        for(int j = 0; j < 32; j++) {
            if(screen[i][j] == 'T') {
                screen[i][j] = '-';
                screen[i][j-1] = 'T';
            }
        }
    }
    
}

static void move_right(char screen[24][32], CharColorMap *colorMap, int mapSize)
{
        for(int i = 0; i < 24; i++) {
            for(int j = 31; j >= 0; j--) { // Iterar da direita para a esquerda
                if(screen[i][j] == 'T') {
                    screen[i][j] = '-';
                    if (j < 31) screen[i][j+1] = 'T'; // Move para a direita
                }
            }
        }          
}

static void move_down(char screen[24][32], CharColorMap *colorMap, int mapSize)
{
    for(int i = 23; i >= 0; i--) { // Iterar de baixo para cima
        for(int j = 0; j < 32; j++) {
            if(screen[i][j] == 'T') {
                screen[i][j] = '-';
                if (i < 23) screen[i+1][j] = 'T'; // Mover para baixo
            }
        }
    }
}


static void draw(char screen[24][32], CharColorMap *colorMap, int mapSize)
{
    clean_buffer();
    for (int i = 0; i < 24; i++) {
        for (int j = 0; j < 32; j++) {
            int colorIndex = getColorIndex(screen[i][j], colorMap, mapSize); // 
            if (colorIndex != -1) { 
                draw_rectangle(j * 32, i * 32, 32, 32, colorIndex);
            }
        }
    }
}


int (proj_main_loop) (int argc, char **argv) {

  CharColorMap colorMap[] = {
        {'T', 1},
        {'I', 2},
        {'L', 3}
    };

int mapSize = sizeof(colorMap) / sizeof(colorMap[0]);

    // Criando e inicializando a matriz de caracteres 32x24 para representar o layout da tela
    char screen[24][32];
    for (int i = 0; i < 24; i++) {
        for (int j = 0; j < 32; j++) {
            screen[i][j] = '-';
        }
    }

    // Adicionando peças de Tetris de exemplo na tela
    // Peça 'T'
    screen[5][10] = 'T';
    screen[5][11] = 'T';
    screen[5][12] = 'T';
    screen[6][11] = 'T';
    //screen[31][23] = 'T'; // 
    
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

    

  for (int i = 0; i < 24; i++) {
        for (int j = 0; j < 32; j++) {
            int colorIndex = getColorIndex(screen[i][j], colorMap, mapSize); // Implemente essa função com base no seu mapeamento
            if (colorIndex != -1) { // Apenas desenha se houver uma cor válida (não é '-')
                if (draw_rectangle(j * 32, i * 32, 32, 32, colorIndex) != 0) return 1;
            }
        }
    }

    if(timer_set_frequency(0, 30)) return 1;
    int ipc_status;
    uint8_t irq_set_keyboard;
    uint8_t irq_set_timer;
    message msg;

    if(timer_subscribe_int(&irq_set_timer)) return 1;
    int r;

    if(keyboard_subscribe(&irq_set_keyboard)) return 1;

    while (scancode != ESC_BREAK_CODE) { 
        if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
            printf("driver_receive failed with: %d", r);
            continue;
        }
        if (is_ipc_notify(ipc_status)) { /* received notification */
            switch (_ENDPOINT_P(msg.m_source)) {
            case HARDWARE: /* hardware interrupt notification */

                if (msg.m_notify.interrupts & irq_set_timer) {
                    timer_int_handler();
                    if (counter % 30 == 0) {
                       // move_down(screen, colorMap, mapSize);
                        draw(screen, colorMap, mapSize);
                    }
                }
                if (msg.m_notify.interrupts & irq_set_keyboard) {
                    kbc_ih_keyboard();
                    switch (scancode)
                    {
                    case A_BREAK_CODE:
                        move_left(screen, colorMap, mapSize);
                        break;
                    
                    case D_BREAK_CODE:
                        move_right(screen, colorMap, mapSize);
                        break;
                    
                    case S_BREAK_CODE:
                        move_down(screen, colorMap, mapSize);
                        break;

                    default:
                      break;
                    }
                }
                break;
            default:
                break; /* no other notifications expected: do nothing */
            }
            //draw(screen, colorMap, mapSize);
        } else { 
        }
}

 

    if (timer_unsubscribe_int()) return 1;
    if (keyboard_unsubscribe()) return 1;

    if(escape_key()) return 1;
    if(vg_exit()) return 1;
    
    return 0;
}

