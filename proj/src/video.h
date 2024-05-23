#include <lcom/lcf.h>
#include <stdint.h>
#include "vbe.h"

uint8_t* video_mem;
uint8_t* double_buffer;
vbe_mode_info_t mode_info;

int(set_video_mode)(uint16_t submode);

int get_mode_info (uint16_t mode);

unsigned int (get_frame_buffer_size)();

int set_frame_buffer(uint16_t mode);

int (set_pixel_color) (uint16_t x, uint16_t y, uint32_t color);

int (draw_rectangle) (uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);

int (draw_hline) (uint16_t x, uint16_t y, uint16_t len, uint32_t color);

int (draw_vline) (uint16_t x, uint16_t y, uint16_t len, uint32_t color);

int (draw_xpm)(uint16_t x, uint16_t y, xpm_map_t xpm);

int (clean_buffer) ();

int (swap_buffer) ();



