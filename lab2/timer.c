#include <lcom/lcf.h>
#include <lcom/timer.h>

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

  return 0;
}

int (timer_subscribe_int)(uint8_t *bit_no) {
    if (bit_no == NULL) return 1;
    *bit_no = BIT(hook_id);
    if (sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_id)!=0) return 1;
  return 0;
}

int (timer_unsubscribe_int)() {
  if(sys_irqrmpolicy(&hook_id)!=0) return 1;
  return 0;
}

void (timer_int_handler)() {
  counter++;
}

int (timer_get_conf)(uint8_t timer, uint8_t *st) {
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
  return 0;
}
