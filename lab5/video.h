#include <lcom/lcf.h>
#include <stdint.h>
#include "vbe.h"

uint8_t* video_mem;
vbe_mode_info_t mode_info;

int(set_video_mode)(uint16_t submode);

int get_mode_info (uint16_t mode);

unsigned int (get_frame_buffer_size)();

int set_frame_buffer(uint16_t mode);

int normalize_color(uint32_t color, uint32_t *new_color);

int (set_pixel_color) (uint16_t x, uint16_t y, uint32_t color);

int (draw_rectangle) (uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);

int (draw_hline) (uint16_t x, uint16_t y, uint16_t len, uint32_t color);

uint32_t (indexed_mode)(uint16_t col, uint16_t row, uint8_t step, uint32_t first, uint8_t n);

uint32_t direct_mode(uint8_t red, uint8_t green, uint8_t blue);

int (draw_xpm)(uint16_t x, uint16_t y, xpm_map_t xpm);





