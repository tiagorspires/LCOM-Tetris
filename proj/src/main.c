#include <lcom/lcf.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "video.h"
#include "keyboard.h"
#include <lcom/timer.h>
#include "matrix.h"
#include "pictures.h"
#include "mouse.h"

typedef enum {
    MAIN_MENU,
    GAME,
    GAME_OVER   
}game_state_t;

extern int counter;
extern uint8_t scancode;
int screen_width = 1024;  
int screen_height = 768;  
extern struct packet pp;
extern int x, y;

int main(int argc, char *argv[]) {
    lcf_set_language("EN-US");
    if (lcf_start(argc, argv)) return 1;
    lcf_cleanup();
    return 0; 
}

typedef struct {
    char type;                
    int position[4][2];       
    int pivot;                
    int colorIndex;           
    bool isActive;
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
    piece.isActive = true;
    return piece;
}

bool is_piece_at_bottom(TetrisPiece *piece, char screen[24][32], int colorScreen[24][32]) {
    for (int i = 0; i < 4; i++) {
        if (piece->position[i][1] == 23 || screen[piece->position[i][1] + 1][piece->position[i][0]] == 'B' || screen[piece->position[i][1] + 1][piece->position[i][0]] == 'P') {
            for (int i = 0; i < 4; i++) {
                colorScreen[piece->position[i][1]][piece->position[i][0]] = piece->colorIndex;
                screen[piece->position[i][1]][piece->position[i][0]] = 'P';
            }          
            piece->isActive = false;
            return true;
        }
    }    
    return false;
}

TetrisPiece generate_random_piece() {
    char types[] = {'T', 'I', 'L', 'Z', 'S'};
    char type = types[rand() % 5];
    int startX = 5 + rand() % 5;
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
        if (newX < 1 || newX > 14 || newY < 1 || newY >= 24 || screen[newY][newX] != '-') {
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

bool is_line_full(char screen[24][32], int line) {
    for (int i = 1; i < 15; i++) {
        if (screen[line][i] == '-') {
            return false;
        }
    }
    return true;
}

void check_and_clear_full_lines(char screen[24][32], int colorScreen[24][32], TetrisPiece pieces[], int num_pieces) {
    for (int i = 1; i < 23; i++) {
        if (is_line_full(screen, i)) {
            for (int j = 1; j < 15; j++) {
                screen[i][j] = '-';
                colorScreen[i][j] = 0;
            }
            for (int k = i; k > 1; k--) {
                for (int j = 1; j < 15; j++) {
                    screen[k][j] = screen[k - 1][j];
                    colorScreen[k][j] = colorScreen[k - 1][j];
                }
            }
            for (int p = 0; p < num_pieces; p++) {
                for (int b = 0; b < 4; b++) {
                    if (pieces[p].position[b][1] < i) {
                        pieces[p].position[b][1]++;
                    }
                }
            }
        }
    }
}

static void draw(char screen[24][32], int colorScreen[24][32], TetrisPiece pieces[], int num_pieces) {
    clean_buffer();
    for (int p = 0; p < num_pieces; p++) {
        if (pieces[p].isActive) {
            for (int b = 0; b < 4; b++) {
                int px = pieces[p].position[b][0];
                int py = pieces[p].position[b][1];
                screen[py][px] = pieces[p].type;
                colorScreen[py][px] = pieces[p].colorIndex;
            }
        }
    }
    for (int i = 0; i < 24; i++) {
        for (int j = 0; j < 32; j++) {
            int colorIndex = colorScreen[i][j];
            if (j < 15 && colorIndex > 0 && colorIndex < 6) {
                draw_rectangle(j * 32, i * 32, 32, 32, colorIndex);
            }
            if (screen[i][j] == 'B') {
                draw_rectangle(j * 32, i * 32, 32, 32, 7);
            }
        }
    }
}

void rotate_piece(TetrisPiece *piece, char screen[24][32], int colorScreen[24][32]) {
    if (piece->type == 'S') {
        return;
    }
    int oldPositions[4][2];
    for (int i = 0; i < 4; i++) {
        oldPositions[i][0] = piece->position[i][0];
        oldPositions[i][1] = piece->position[i][1];
        screen[piece->position[i][1]][piece->position[i][0]] = '-';
        colorScreen[piece->position[i][1]][piece->position[i][0]] = 0;
    }
    int pivotX = piece->position[piece->pivot][0];
    int pivotY = piece->position[piece->pivot][1];
    for (int i = 0; i < 4; i++) {
        int newX = pivotX + pivotY - oldPositions[i][1];
        int newY = pivotY - pivotX + oldPositions[i][0];
        if (newX < 1 || newX > 14 || newY < 1 || newY >= 24 || screen[newY][newX] != '-') {
            for (int j = 0; j < 4; j++) {
                screen[oldPositions[j][1]][oldPositions[j][0]] = piece->type;
                colorScreen[oldPositions[j][1]][oldPositions[j][0]] = piece->colorIndex;
            }
            return;
        }
    }
    for (int i = 0; i < 4; i++) {
        piece->position[i][0] = pivotX + pivotY - oldPositions[i][1];
        piece->position[i][1] = pivotY - pivotX + oldPositions[i][0];
        screen[piece->position[i][1]][piece->position[i][0]] = piece->type;
        colorScreen[piece->position[i][1]][piece->position[i][0]] = piece->colorIndex;
    }
}

bool game_over(char screen[24][32]) {
    for (int i = 1; i < 15; i++) {
        if (screen[1][i] == 'P') {
            return true;
        }
    }
    return false;
}

int draw_centered_rectangles(int screen_width, int screen_height) {
    int rect_width = 300;
    int rect_height = 100;
    int gap = 30; // Gap between the two rectangles

    // Calculate coordinates for the top rectangle
    int top_rect_x = (screen_width / 2) - (rect_width / 2);
    int top_rect_y = (screen_height / 2) - rect_height - (gap / 2);

    // Calculate coordinates for the bottom rectangle
    int bottom_rect_x = (screen_width / 2) - (rect_width / 2);
    int bottom_rect_y = (screen_height / 2) + (gap / 2);

    // Draw top rectangle
    if (draw_hline(top_rect_x, top_rect_y, rect_width, 1) != 0) return 1;
    if (draw_vline(top_rect_x, top_rect_y, rect_height, 1) != 0) return 1;
    if (draw_vline(top_rect_x + rect_width, top_rect_y, rect_height, 1) != 0) return 1;
    if (draw_hline(top_rect_x, top_rect_y + rect_height, rect_width, 1) != 0) return 1;

    // Draw bottom rectangle
    if (draw_hline(bottom_rect_x, bottom_rect_y, rect_width, 1) != 0) return 1;
    if (draw_vline(bottom_rect_x, bottom_rect_y, rect_height, 1) != 0) return 1;
    if (draw_vline(bottom_rect_x + rect_width, bottom_rect_y, rect_height, 1) != 0) return 1;
    if (draw_hline(bottom_rect_x, bottom_rect_y + rect_height, rect_width, 1) != 0) return 1;

    return 0; // Success
}

void draw_word_in_rectangle(int rect_x, int rect_y, const xpm_row_t* word[], int word_length) {
    int letter_width = 7;  // Width of each letter in the XPM
    int letter_height = 9; // Height of each letter in the XPM
    int spacing = 2;       // Spacing between letters

    int total_width = (letter_width + spacing) * word_length - spacing;
    int start_x = rect_x + (300 - total_width) / 2;
    int start_y = rect_y + (100 - letter_height) / 2;

    for (int i = 0; i < word_length; i++) {
        if (draw_xpm(start_x + i * (letter_width + spacing), start_y, word[i]) != 0) {
            printf("Failed to draw letter %d of the word.\n", i);
        }
    }
}

void drop_until_bottom(TetrisPiece *piece, char screen[24][32], int colorScreen[24][32]) {
    while (!is_piece_at_bottom(piece, screen, colorScreen)) {
        move_piece(piece, 0, 1, screen, colorScreen);
    }
}



int (proj_main_loop) (int argc, char **argv) {

    game_state_t state = MAIN_MENU;
    TetrisPiece pieces[100]; 
    char screen[24][32];
    int colorScreen[24][32];

    if(set_frame_buffer(0x105)) return 1;
    if(set_video_mode(0x105)) return 1;
    if(timer_set_frequency(0, 30)) return 1;

    int ipc_status, r;
    uint8_t irq_set_keyboard;
    uint8_t irq_set_timer;
    uint8_t irq_set_mouse;
    
    
    message msg;

    // set stream mode and data reporting
    if(change_data_report_mode(0xF4)) return 1;
    if(change_data_report_mode(0xEA)) return 1;


    if(timer_subscribe_int(&irq_set_timer)) return 1;
    if(keyboard_subscribe(&irq_set_keyboard)) return 1;
    if(mouse_subscribe_int(&irq_set_mouse)) return 1;

    int countpiece = 1;
    while (scancode != ESC_BREAK_CODE && countpiece < 100) { 
        if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
            printf("driver_receive failed with: %d", r);
            continue;
        }
        if (is_ipc_notify(ipc_status)) { /* received notification */
            switch (_ENDPOINT_P(msg.m_source)) {
            case HARDWARE: /* hardware interrupt notification */

                if (msg.m_notify.interrupts & irq_set_timer) {
                    if(state == GAME){
                        timer_int_handler();
                        if (counter % 60 == 0) {
                                if (is_piece_at_bottom(&pieces[countpiece - 1], screen, colorScreen)) {
                                check_and_clear_full_lines(screen, colorScreen, pieces, countpiece);
                                    if (countpiece < 50) {
                                        pieces[countpiece] = generate_random_piece();
                                        pieces[countpiece].isActive = true;
                                        countpiece++;
                                    }
                                } else {
                                    move_piece(&pieces[countpiece - 1], 0, 1, screen, colorScreen);
                                }
                            }
                        }
                }

                if (msg.m_notify.interrupts & irq_set_keyboard) {
                    kbc_ih_keyboard();
                    TetrisPiece* current_piece = &pieces[countpiece - 1];
                    if(state == GAME){
                        if (!game_over(screen)){
                            switch (scancode)
                            {
                            case A_BREAK_CODE:
                                move_piece(current_piece, -1, 0, screen, colorScreen);
                                break;
                            
                            case D_BREAK_CODE:
                                move_piece(current_piece, 1, 0, screen, colorScreen);
                                break;
                            
                            case S_BREAK_CODE:
                                move_piece(current_piece, 0, 1, screen, colorScreen);
                                break;
                            case W_BREAK_CODE:
                                rotate_piece(current_piece, screen, colorScreen);
                                break;
                            case SPACE_BREAK_CODE:
                                drop_until_bottom(current_piece, screen, colorScreen);
                                break;
                            default:
                                break;
                            }
                        } else {
                            state = GAME_OVER;
                        }
                    }
                    if(state == MAIN_MENU && scancode == E_BREAK_CODE){
                        
                        TetrisPiece piece = generate_random_piece();
                        pieces[0] = piece;
                            for (int i = 0; i < 24; i++) {
                                for (int j = 0; j < 32; j++) {
                                    if (j == 0 || j >=14 || i == 0 || i == 23) { 
                                        screen[i][j] = 'B';  
                                        colorScreen[i][j] = 7; 
                                    } else if (j < 15) {
                                        screen[i][j] = '-';
                                        colorScreen[i][j] = 0;
                                    } else {
                                        screen[i][j] = ' ';  
                                        colorScreen[i][j] = 0;
                                    }
                                }
                            }
                        state = GAME;
                    }

                    if (state == GAME && scancode == Q_BREAK_CODE) {
                        state = GAME_OVER;
                    }
                }
                if (msg.m_notify.interrupts & irq_set_mouse) {
                    mouse_ih();
                    mouse_event_handler(&pp);


                }
                break;
            default:
                break; /* no other notifications expected: do nothing */
            }
            if(state == MAIN_MENU){
                clean_buffer();
                // draw the mouse cursor according to his pp position
                draw_xpm(x, y, mouse_cursor);
                if (draw_centered_rectangles(screen_width, screen_height) != 0) {
                    printf("Failed to draw rectangles.\n");
                    return 1;  // Handle failure case
                }
                
                // Draw the words "PLAY" and "EXIT" in the rectangles
                xpm_row_t const *play_word[] = {P, L, A, Y};
                xpm_row_t const *exit_word[] = {E, X, I, T};

                // Calculate coordinates for the top rectangle
                int top_rect_x = (screen_width / 2) - 150;
                int top_rect_y = (screen_height / 2) - 130;
                // Calculate coordinates for the bottom rectangle
                int bottom_rect_x = (screen_width / 2) - 150;
                int bottom_rect_y = (screen_height / 2) + 30;

                draw_word_in_rectangle(top_rect_x, top_rect_y, play_word, 4);
                draw_word_in_rectangle(bottom_rect_x, bottom_rect_y, exit_word, 4);

                swap_buffer();
            } else if(state == GAME_OVER){ 
                for (int i = 0; i < 24; i++) {
                    for (int j = 0; j < 32; j++) {
                        screen[i][j] = '-';
                        colorScreen[i][j] = 63;
                    }
                }
                draw(screen, colorScreen, pieces, countpiece);
                swap_buffer();
                if (timer_unsubscribe_int()) return 1;
                if (keyboard_unsubscribe()) return 1;
                if(mouse_subscribe_int(&irq_set_mouse)) return 1;
                if(vg_exit()) return 1;
                return 0;
            } else {
                draw(screen, colorScreen, pieces, countpiece);
                swap_buffer();
            }
        } else { 
        }
    }

    if (timer_unsubscribe_int()) return 1;
    if (keyboard_unsubscribe()) return 1;
   // if(mouse_subscribe_int(&irq_set_mouse)) return 1;
    if(change_data_report_mode(0xF5)) return 1;  
    if(escape_key()) return 1;
    if(vg_exit()) return 1;
    return 0;
}
