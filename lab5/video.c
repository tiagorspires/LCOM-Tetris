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

int get_mode_info (uint16_t mode){ // encontrar o modo de resolucao
    memset(&mode_info, 0, sizeof(vbe_mode_info_t));
    if(vbe_get_mode_info(mode ,&mode_info)!=0) return 1;
    return 0;
}



unsigned int (bytes_per_pixel)(){ // calculo do numero de bytes por pixel
    return (mode_info.BitsPerPixel + 7) >> 3;
}

unsigned int (get_frame_buffer_size)(){ // calculo do tamanho do buffer
    return mode_info.XResolution * mode_info.YResolution * bytes_per_pixel();
}

int set_frame_buffer(uint16_t mode){
    get_mode_info(mode);
    unsigned int frame_buffer_size = get_frame_buffer_size();

    struct minix_mem_range mr; // memoria fisica (memory mapping)
    mr.mr_base = mode_info.PhysBasePtr;
    mr.mr_limit = mr.mr_base + frame_buffer_size;

    // alocar a memomoria fisica necessaria
    if(sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)) return 1;

    // mapear a memoria fisica para a memoria virtual

    video_mem = vm_map_phys(SELF, (void *)mr.mr_base, frame_buffer_size);

    if(video_mem == NULL) return 1;

return 0;
}

int (set_pixel_color) (uint16_t x, uint16_t y, uint32_t color){
    
    // verificar se as coordenadas estao dentro dos limites do ecrã

    if(x > mode_info.XResolution || y > mode_info.YResolution ) return 1;

    // define where to color the pixel

    unsigned int start_index = (y * mode_info.XResolution + x) * bytes_per_pixel();

    // copiar a cor para o pixel
    if(memcpy(&video_mem[start_index], &color, bytes_per_pixel()) == NULL) return 1;

    
    return 0;

}

// desenhar uma linha horizontal

int (draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color){
    
    while (len > 0){
        if(set_pixel_color(x, y, color) != 0) return 1;
        x++;
        len--;
    }

    return 0;
}

int (draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color){
    while (height > 0){
        if(draw_hline(x, y, width, color) != 0) return 1;
        y++;
        height--;
    }
    return 0;
}

// normalizar a cor

int normalize_color(uint32_t color, uint32_t *new_color) {
    if (mode_info.BitsPerPixel == 32) {
        *new_color = color;
    } else {
        uint32_t bitmask = (1 << mode_info.BitsPerPixel) - 1;
        *new_color = color & bitmask;
    }
    return 0;
}

// modo indexado

uint32_t (indexed_mode)(uint16_t col, uint16_t row, uint8_t step, uint32_t first, uint8_t n) {
    // Calcular a posição linear baseada na linha e coluna.
    // 'row * n' calcula a progressão linear ao longo das linhas,
    // e 'col' adiciona a progressão ao longo das colunas.
    uint32_t linear_position = row * n + col;

    // Multiplicar a posição linear pelo 'step' para aumentar
    // o índice de cor de forma mais rápida ou para criar um padrão de cor.
    uint32_t index_increment = linear_position * step;

    // Adicionar o valor inicial 'first' para definir o ponto de partida do índice de cor.
    // Isso permite que a primeira cor na paleta possa ser ajustada.
    uint32_t base_index = first + index_increment;

    // Aplicar a operação de módulo para garantir que o índice de cor não exceda
    // o número máximo de cores disponíveis, que é determinado por 'BitsPerPixel'.
    uint32_t max_colors = 1 << mode_info.BitsPerPixel;
    uint32_t final_index = base_index % max_colors;

    // Retorna o índice final de cor, que será usado para buscar a cor na paleta.
    return final_index;
}


uint32_t direct_mode(uint8_t red, uint8_t green, uint8_t blue) {
    // Aplica as máscaras de bits e desloca cada cor para a sua posição correta
    uint32_t red_component = ((uint32_t)red & ((1U << mode_info.RedMaskSize) - 1U)) << mode_info.RedFieldPosition;
    uint32_t green_component = ((uint32_t)green & ((1U << mode_info.GreenMaskSize) - 1U)) << mode_info.GreenFieldPosition;
    uint32_t blue_component = ((uint32_t)blue & ((1U << mode_info.BlueMaskSize) - 1U)) << mode_info.BlueFieldPosition;

    // Retorna a combinação dos componentes de cor em um único valor de 32 bits
    return red_component | green_component | blue_component;
}


int (draw_xpm)(uint16_t x, uint16_t y, xpm_map_t xpm){

xpm_image_t image;

uint8_t *colors = xpm_load(xpm, XPM_INDEXED, &image);

if(colors == NULL) return 1;

for (int i = 0; i < image.height; i++){
    for (int j = 0; j < image.width; j++){
        uint32_t color = colors[i * image.width + j];
        if(set_pixel_color(x + j, y + i, color) != 0) return 1;
    }
}

return 0;
}






