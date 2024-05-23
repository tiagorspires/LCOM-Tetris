#ifndef GAME_H
#define GAME_H

#include <lcom/lcf.h>
#include <stdbool.h>
#include "video.h"
#include "stdint.h"

typedef enum {
    MAIN_MENU,
    GAME,
    GAME_OVER   
} GameState;

typedef struct {
    char type;
    int position[4][2];
    int pivot;
    int colorIndex;
    bool isActive;
} TetrisPiece;

TetrisPiece create_piece(char type, int startX, int startY, int colorIndex);
bool is_piece_at_bottom(TetrisPiece *piece, char screen[24][32], int colorScreen[24][32]);
TetrisPiece generate_random_piece();
void move_piece(TetrisPiece *piece, int deltaX, int deltaY, char screen[24][32], int colorScreen[24][32]);
bool is_line_full(char screen[24][32], int line);
void check_and_clear_full_lines(char screen[24][32], int colorScreen[24][32], TetrisPiece pieces[], int num_pieces);
void draw(char screen[24][32], int colorScreen[24][32], TetrisPiece pieces[], int num_pieces);
void rotate_piece(TetrisPiece *piece, char screen[24][32], int colorScreen[24][32]);
bool game_over(char screen[24][32]);
int draw_centered_rectangles(int max_x, int max_y);
void draw_word_in_rectangle(int rect_x, int rect_y, const xpm_row_t* word[], int word_length);
void drop_until_bottom(TetrisPiece *piece, char screen[24][32], int colorScreen[24][32]);
void handle_game_over(GameState *state, char screen[24][32], int colorScreen[24][32], TetrisPiece pieces[], int *countpiece);
void copy_piece_to_screen(TetrisPiece *piece, char screen[24][32], int colorScreen[24][32]); // Ensure this is declared

#endif // GAME_H
