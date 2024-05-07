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

typedef struct {
    char type;                // Character representing the type of the piece ('T', 'I', 'L', etc.)
    int position[4][2];       
    int pivot;                // Index of the pivot block for rotation (if applicable)
    int colorIndex;           // Color index for drawing the piece
} TetrisPiece;



TetrisPiece create_piece(char type, int startX, int startY, int colorIndex) {
    TetrisPiece piece;
    piece.type = type;
    switch (type) {
        case 'T':  
            piece.position[0][0] = startX;     piece.position[0][1] = startY;
            piece.position[1][0] = startX + 1; piece.position[1][1] = startY;
            piece.position[2][0] = startX + 2; piece.position[2][1] = startY;
            piece.position[3][0] = startX + 1; piece.position[3][1] = startY + 1;
            piece.pivot = 1; // Central block as pivot
            piece.colorIndex = 1;

            break;
        case 'I':  
            piece.position[0][0] = startX; piece.position[0][1] = startY;
            piece.position[1][0] = startX; piece.position[1][1] = startY + 1;
            piece.position[2][0] = startX; piece.position[2][1] = startY + 2;
            piece.position[3][0] = startX; piece.position[3][1] = startY + 3;
            piece.pivot = 1; 
            piece.colorIndex = 2;
            break;

        case 'L':  
            piece.position[0][0] = startX;     piece.position[0][1] = startY;
            piece.position[1][0] = startX;     piece.position[1][1] = startY + 1;
            piece.position[2][0] = startX;     piece.position[2][1] = startY + 2;
            piece.position[3][0] = startX + 1; piece.position[3][1] = startY + 2;
            piece.pivot = 1; // Second block as pivot
            piece.colorIndex = 3;
            break;
        
        case 'Z':  
            piece.position[0][0] = startX;     piece.position[0][1] = startY;
            piece.position[1][0] = startX + 1; piece.position[1][1] = startY;
            piece.position[2][0] = startX + 1; piece.position[2][1] = startY + 1;
            piece.position[3][0] = startX + 2; piece.position[3][1] = startY + 1;
            piece.pivot = 1; // Second block as pivot
            piece.colorIndex = 4;
            break;
        
        case 'S': // square
            piece.position[0][0] = startX;     piece.position[0][1] = startY;
            piece.position[1][0] = startX + 1; piece.position[1][1] = startY;
            piece.position[2][0] = startX;     piece.position[2][1] = startY + 1;
            piece.position[3][0] = startX + 1; piece.position[3][1] = startY + 1;
            piece.pivot = 0;
            piece.colorIndex = 5;
             
            break;
    }
    return piece;
}


// check is the piece arrived at the bottom of the screen or if there is a block below it

bool is_piece_at_bottom(TetrisPiece *piece, char screen[24][32], int colorScreen[24][32]) {
    for (int i = 0; i < 4; i++) {
        if (piece->position[i][1] == 23 || screen[piece->position[i][1] + 1][piece->position[i][0]] == 'B' ||screen[piece->position[i][1] + 1][piece->position[i][0]] == 'P') {
            

            for (int i = 0; i < 4; i++) {
                colorScreen[piece->position[i][1]][piece->position[i][0]] = piece->colorIndex;
                screen[piece->position[i][1]][piece->position[i][0]] = 'P';
            }          
            
            return true;
        }

    }    
    
    return false;
}


// generate a piece with a random type on the top of the screen at a random position

TetrisPiece generate_random_piece() {
    char types[] = {'T', 'I', 'L', 'Z', 'S'};
    char type = types[rand() % 5];
    // random start position between 15 and 20
    int startX = rand() % 15 + 5;
    return create_piece(type, startX, 1, 1);
}

static void copy_piece_to_screen(TetrisPiece *piece, char screen[24][32], int colorScreen[24][32]) {
    for (int i = 0; i < 4; i++) {
        colorScreen[piece->position[i][1]][piece->position[i][0]] = piece->colorIndex;
        screen[piece->position[i][1]][piece->position[i][0]] = piece->type;
    }
}



void move_piece(TetrisPiece *piece, int deltaX, int deltaY, char screen[24][32], int colorScreen[24][32]) {
    int oldPositions[4][2];
    for (int i = 0; i < 4; i++) {
        oldPositions[i][0] = piece->position[i][0];
        oldPositions[i][1] = piece->position[i][1];

        screen[piece->position[i][1]][piece->position[i][0]] = '-';
        colorScreen[piece->position[i][1]][piece->position[i][0]] = 0;
    }

    bool movePossible = true;
    for (int i = 0; i < 4; i++) {
        int newX = oldPositions[i][0] + deltaX;
        int newY = oldPositions[i][1] + deltaY;
        
        if (newX < 0 || newX >= 32 || newY < 0 || newY >= 24 || screen[newY][newX] != '-') {
            movePossible = false;
            break;
        }
    }

    if (!movePossible) {
        for (int i = 0; i < 4; i++) {
            screen[oldPositions[i][1]][oldPositions[i][0]] = piece->type;
            colorScreen[oldPositions[i][1]][oldPositions[i][0]] = piece->colorIndex;
        }
        return;
    }

    for (int i = 0; i < 4; i++) {
        piece->position[i][0] = oldPositions[i][0] + deltaX;
        piece->position[i][1] = oldPositions[i][1] + deltaY;

        screen[piece->position[i][1]][piece->position[i][0]] = piece->type;
        colorScreen[piece->position[i][1]][piece->position[i][0]] = piece->colorIndex;
    }


    copy_piece_to_screen(piece, screen, colorScreen);
}





static void draw(char screen[24][32],int colorScreen[24][32]) {
    clean_buffer();
    
    for (int i = 0; i < 24; i++) {
        for (int j = 0; j < 32; j++) {
            int colorIndex = colorScreen[i][j];
            if(colorIndex > 0 && colorIndex < 6) 
                draw_rectangle(j * 32, i * 32, 32, 32, colorIndex);
            if(screen[i][j] == 'B') 
                draw_rectangle(j * 32, i * 32, 32, 32, 7);
        }
    }
    
}




int (proj_main_loop) (int argc, char **argv) {

    


    // array of pieces

    TetrisPiece pieces[50]; 

    // generate the first piece

    TetrisPiece piece = generate_random_piece();
    pieces[0] = piece;


    char screen[24][32];
    int colorScreen[24][32];
    for (int i = 0; i < 24; i++) {
        for (int j = 0; j < 32; j++) {
            if (i == 0 || i == 23 || j == 0 || j == 31) {
                screen[i][j] = 'B';  // Fill the borders with 'B'
            } else {
                screen[i][j] = '-';
            }
        }
    }


        if(set_frame_buffer(0x105)) return 1;
        if(set_video_mode(0x105)) return 1;
        if(timer_set_frequency(0, 30)) return 1;

    int ipc_status, r;
    uint8_t irq_set_keyboard;
    uint8_t irq_set_timer;
    message msg;

    if(timer_subscribe_int(&irq_set_timer)) return 1;
    if(keyboard_subscribe(&irq_set_keyboard)) return 1;
    int countpiece = 1;
    while (scancode != ESC_BREAK_CODE && countpiece < 50) { 
        if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
            printf("driver_receive failed with: %d", r);
            continue;
        }
        if (is_ipc_notify(ipc_status)) { /* received notification */
            switch (_ENDPOINT_P(msg.m_source)) {
            case HARDWARE: /* hardware interrupt notification */

                if (msg.m_notify.interrupts & irq_set_timer) {
                    timer_int_handler();
                    if (counter % 60 == 0) {
                        if (is_piece_at_bottom(&piece, screen, colorScreen)) {
                            piece = generate_random_piece();
                            pieces[countpiece] = piece;
                            countpiece++;
                        } else {
                            move_piece(&piece, 0, 1, screen, colorScreen);
                        }
                       
                    }
                }

                if (msg.m_notify.interrupts & irq_set_keyboard) {
                    kbc_ih_keyboard();
                    switch (scancode)
                    {
                    case A_BREAK_CODE:
                          move_piece(&piece, -1, 0, screen, colorScreen);
                        break;
                    
                    case D_BREAK_CODE:
                       move_piece(&piece, 1, 0, screen, colorScreen);
                        break;
                    
                    case S_BREAK_CODE:
                        move_piece(&piece, 0, 1, screen, colorScreen);
                        break;

                    default:
                        break;
                    }
                }
                break;
            default:
                break; /* no other notifications expected: do nothing */
            }
            draw(screen, colorScreen);
        } else { 
        }
}

    if (timer_unsubscribe_int()) return 1;
    if (keyboard_unsubscribe()) return 1;
    if(escape_key()) return 1;
    if(vg_exit()) return 1;
    return 0;
}
