#include <lcom/lcf.h>
#include <video.h>
#include <stdint.h>
#include "vbe.h"

int (set_video_mode)(uint16_t submode) {
  reg86_t reg86;

    memset(&reg86, 0, sizeof(reg86)); /* zero the structure */
    reg86.ax = AX_GRAPHICS_MODE; /* VBE call, function 02 -- set VBE mode */
    reg86.bx = BX_GRAPHICS_MODE | submode; /* set bit 14: linear framebuffer */
    reg86.intno = INTNO;

  return sys_int86(&reg86);

}
