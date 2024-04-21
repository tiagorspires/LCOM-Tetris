// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <lcom/lab5.h>

#include <stdint.h>
#include <stdio.h>
#include "keyboard.h"
#include "video.h"


// Any header files included below this line should have been created by you

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab5/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab5/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(video_test_init)(uint16_t mode, uint8_t delay) {
    if (set_video_mode(mode)!= 0) return 1;
    tickdelay(micros_to_ticks(delay*1000000));
    if(vg_exit()!=0) return 1;
  return 0;
}

int(video_test_rectangle)(uint16_t mode, uint16_t x, uint16_t y,
                          uint16_t width, uint16_t height, uint32_t color) {
    uint32_t new_color;
    if (set_frame_buffer(mode)!= 0) return 1;
    if (set_video_mode(mode)!= 0) return 1;
    if(normalize_color(color, &new_color)!=0) return 1;
    if(draw_rectangle(x, y, width, height, new_color)!=0) return 1;
    if(escape_key()) return 1;
    if(vg_exit()!=0) return 1;
  return 0;

}





int (video_test_pattern)(uint16_t mode, uint8_t no_rectangles, uint32_t first, uint8_t step) {
    // Initialize the mode and framebuffer
    if (set_frame_buffer(mode) != 0 || set_video_mode(mode) != 0) {
        return 1;
    }

    int vertical = mode_info.YResolution / no_rectangles;
    int horizontal = mode_info.XResolution / no_rectangles;

    for (int i = 0; i < no_rectangles; i++) {
        for (int j = 0; j < no_rectangles; j++) {
            uint32_t color;
            if (mode_info.MemoryModel == DIRECT_COLOR_MODE) {
              // Extract initial color components from 'first' by right-shifting to align with their respective field positions and applying a bitmask to filter by mask size. 
              // The extracted components are then adjusted by adding a calculated offset based on their position and a step value, with modulus to ensure values wrap within the allowed range.

                uint8_t R = (first >> mode_info.RedFieldPosition) & ((1 << mode_info.RedMaskSize) - 1);
                uint8_t G = (first >> mode_info.GreenFieldPosition) & ((1 << mode_info.GreenMaskSize) - 1);
                uint8_t B = (first >> mode_info.BlueFieldPosition) & ((1 << mode_info.BlueMaskSize) - 1);
                R = (R + j * step) % (1 << mode_info.RedMaskSize);
                G = (G + i * step) % (1 << mode_info.GreenMaskSize);
                B = (B + (i + j) * step) % (1 << mode_info.BlueMaskSize);

                color = direct_mode(R, G, B);
            } else {
                color = indexed_mode(j, i, step, first, no_rectangles);
            }

            if (draw_rectangle(j * horizontal, i * vertical, horizontal, vertical, color)) {
                return 1;
            }
        }
    }

    if(escape_key()) return 1;

    if (vg_exit() != 0) return 1;

    return 0;
}


int(video_test_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y) {
  if (set_frame_buffer(0x105) != 0 || set_video_mode(0x105) != 0) return 1;
  if (draw_xpm(x, y,xpm) != 0) return 1;
  if(escape_key()) return 1;
  if (vg_exit() != 0) return 1;
  return 0;
}

int(video_test_move)(xpm_map_t xpm, uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf,
                     int16_t speed, uint8_t fr_rate) {



  return 1;
}

int(video_test_controller)() {
  /* To be completed */
  printf("%s(): under construction\n", __func__);

  return 1;
}


