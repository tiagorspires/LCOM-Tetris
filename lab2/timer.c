#include <lcom/lcf.h>
#include <lcom/timer.h>
<<<<<<< HEAD
#include <stdint.h>
#include "i8254.h"

int hook_id = 0;
int counter = 0;

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {

  // Verificação de input
  if (freq > TIMER_FREQ || freq < 19) return 1;

  // Consultamos a configuração atual do @timer
  uint8_t controlWord;
  if (timer_get_conf(timer, &controlWord) != 0) return 1;

  // Novo comando de configuração, ativamos os bits da zona 'LSB followed by MSB' e mantemos os restantes
  controlWord = (controlWord & 0x0F) | TIMER_LSB_MSB; 

  // Cálculo do valor inicial do contador e partes mais e menos significativas
  uint32_t initialValue = TIMER_FREQ / freq;
  uint8_t MSB, LSB;
  util_get_MSB(initialValue, &MSB);
  util_get_LSB(initialValue, &LSB);

  // Atualização da controlWord de acordo com o timer escolhido
  // @selectTimer ficará com a porta do timer escolhido (0x40, 0x41 ou 0x42)
  uint8_t selectTimer;      
  switch (timer) {  
    case 0: 
      controlWord |= TIMER_SEL0;
      selectTimer = TIMER_0;
      break;
    case 1:
      controlWord |= TIMER_SEL1;
      selectTimer = TIMER_1;
      break;
    case 2:
      controlWord |= TIMER_SEL2;
      selectTimer = TIMER_2;
      break;
    default:
      return 1;
  }

  // Avisamos o i8254 que vamos configurar o timer
  if (sys_outb(TIMER_CTRL, controlWord) != 0) return 1;

  // Injetamos o valor inicial do contador (lsb seguido de msb) diretamente no registo correspondente
  if (sys_outb(selectTimer, LSB) != 0) return 1;
  if (sys_outb(selectTimer, MSB) != 0) return 1;
=======

#include <stdint.h>

#include "i8254.h"

int hook_id = 0, counter = 0;

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  if (freq > TIMER_FREQ || freq < 19) return 1;
  uint8_t control;
  uint8_t selectedTimer;
  if (timer_get_conf(timer, &control)!= 0) return 1;
  control = control & 0x0F;
  switch (timer)
  {
  case 0:
    control |= TIMER_SEL0 | TIMER_LSB_MSB;
    selectedTimer = TIMER_0;
    break;
  case 1:
    control |= TIMER_SEL1 | TIMER_LSB_MSB;
    selectedTimer = TIMER_1;
    break;
  case 2:
    control |= TIMER_SEL2 | TIMER_LSB_MSB;
    selectedTimer = TIMER_2;
    break;  
  default:
    break;
      return 1;
  }

  uint32_t initialValue = TIMER_FREQ/freq;
  uint8_t lsb, msb;
  util_get_LSB(initialValue, &lsb);
  util_get_MSB(initialValue, &msb);

  if (sys_outb(TIMER_CTRL, control)!=0) return 1;
  if (sys_outb(selectedTimer, lsb)!=0) return 1;
  if (sys_outb(selectedTimer, msb)!=0) return 1;

>>>>>>> 7712c45f7814e4fc95f87d258c5a71fa805279e8
  return 0;
}

int (timer_subscribe_int)(uint8_t *bit_no) {
<<<<<<< HEAD
  if( bit_no == NULL) return 1; // o apontador deve ser válido
  *bit_no = BIT(hook_id);       // a função que chamou esta deve saber qual é a máscara a utilizar
  if (sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_id) != 0) return 1; // subscrição das interrupções
=======
    if (bit_no == NULL) return 1;
    *bit_no = BIT(hook_id);
    if (sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_id)!=0) return 1;
>>>>>>> 7712c45f7814e4fc95f87d258c5a71fa805279e8
  return 0;
}

int (timer_unsubscribe_int)() {
<<<<<<< HEAD
  if (sys_irqrmpolicy(&hook_id) != 0) return 1; // desligar as interrupções
=======
  if(sys_irqrmpolicy(&hook_id)!=0) return 1;
>>>>>>> 7712c45f7814e4fc95f87d258c5a71fa805279e8
  return 0;
}

void (timer_int_handler)() {
  counter++;
}

int (timer_get_conf)(uint8_t timer, uint8_t *st) {
<<<<<<< HEAD
  if (st == NULL || timer > 2 || timer < 0) return 1; // validação de input
  uint8_t RBC = (TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer)); // construção do readback command
  if (sys_outb(TIMER_CTRL, RBC) != 0) return 1;       // avisar o i8254 que vamos ler a configuração
  if (util_sys_inb(TIMER_0 + timer, st)) return 1;    // lemos a configuração diretamente do registo associado ao timer
  return 0;
}

int (timer_display_conf)(uint8_t timer, uint8_t st, enum timer_status_field field) {
  union timer_status_field_val data; // variável que vai conter o valor a mostrar
  switch (field) {
    case tsf_all:
      data.byte = st;
      break;
    case tsf_initial:
      data.in_mode = (st & TIMER_LSB_MSB) >> 4;
      break;
    case tsf_mode:
      data.count_mode = (st & (BIT(1) | BIT(2) | BIT(3))) >> 1;
      break;
    case tsf_base:
      data.bcd = (st & TIMER_BCD);
      break;
    default:
      return 1;
  }

  if (timer_print_config(timer, field, data) != 0) return 1;
=======
  if(st == NULL || timer < 0 || timer > 2) return 1;
  uint8_t rbc = TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer);
  if (sys_outb(TIMER_CTRL, rbc)!=0) return 1;
  if (util_sys_inb(TIMER_0 + timer, st)!=0) return 1;
  return 0;
}

int (timer_display_conf)(uint8_t timer, uint8_t conf,
                        enum timer_status_field field) {
  union timer_status_field_val data;
  switch (field)
  {
  case tsf_all:
    data.byte = conf;
    break;
  case tsf_initial:
    conf = conf >> 4;
    if (conf == 1) data.in_mode = LSB_only;
    else if (conf == 2) data.in_mode = MSB_only;
    else if (conf == 3) data.in_mode = MSB_after_LSB;
    else data.in_mode = INVAL_val;
    break;
  case tsf_mode:
    conf = (conf >> 1) & 0x07;

    if (conf == 6) data.count_mode = 2;
    else if (conf == 7) data.count_mode = 3;
    else data.count_mode = conf;
    break;
  case tsf_base:
    data.bcd = conf & TIMER_BCD;
    break;
  default:
    return 1;
  }
  if(timer_print_config(timer, field, data)!=0) return 1;
>>>>>>> 7712c45f7814e4fc95f87d258c5a71fa805279e8
  return 0;
}
