#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

int (timer_get_conf)(uint8_t timer, uint8_t *st) {

  if (st == NULL || timer < 0  || timer> 2) return 1;

  uint8_t ReadBackCommand =  TIMER_RB_CMD |  TIMER_RB_COUNT_ | TIMER_RB_SEL(timer);

  if(sys_outb(TIMER_CTRL, ReadBackCommand)) return 1;

  if(util_sys_inb(TIMER_0+timer, st)) return 1;

  return 0;
}

int (timer_display_conf)(uint8_t timer, uint8_t st, enum timer_status_field field) {

  if (timer < 0 || timer > 2) return 1;
  union timer_status_field_val conf;

switch(field) {
  case tsf_all:
    conf.byte = st;
    break;
  case tsf_initial:
    conf.in_mode = (st & (TIMER_LSB | TIMER_MSB)) >> 4;
    if (conf.in_mode > 3) conf.in_mode = INVAL_val;
    break;
  case tsf_mode:
    conf.count_mode = (st & (BIT(3) | BIT(2) | BIT(1))) >> 1;
    if (conf.count_mode == 6 || conf.count_mode == 7) conf.count_mode -= 4;
    break;
  case tsf_base:
    conf.bcd = (st & BIT(0)) ? true : false;
    break;
    return 1;
}

timer_print_config(timer, field, conf);

return 0;
}

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  
  if (freq < TIMER_MIN_FREQ || freq > TIMER_FREQ || timer < 0 || timer > 2) return 1;

  uint8_t st;

  if (timer_get_conf(timer, &st)) return 1;
  uint8_t control_word = TIMER_LSB_MSB;

  switch(timer) {
    case 0: 
      control_word |= TIMER_SEL0;
      break;
    case 1: 
      control_word |= TIMER_SEL1;
      break;
    case 2: 
      control_word |= TIMER_SEL2;
      break;
    default: 
      return 1;
  }

  st &= (BIT(3) | BIT(2) | BIT(1));
  control_word |= st;

  if (sys_outb(TIMER_CTRL, control_word)) return 1;

  uint16_t val = TIMER_FREQ / freq;
  uint8_t lsb, msb;

  if (util_get_LSB(val, &lsb) || util_get_MSB(val, &msb)) return 1;
  if (sys_outb(TIMER_0 + timer, lsb) || sys_outb(TIMER_0 + timer, msb)) return 1;

  return 0;
}

int counter_timer = 0, timer_hook_id = 0;

int (timer_subscribe_int)(uint8_t *bit_no) {
  if (bit_no == NULL) return 1;
  *bit_no = BIT(timer_hook_id);
  if (sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &timer_hook_id)) return 1;
  return 0;
}

int (timer_unsubscribe_int)() {
  if (sys_irqrmpolicy(&timer_hook_id)) return 1;
  return 0;
}

void (timer_int_handler)() {
  counter_timer++;
}
