#include "rtc.h"

static int rtc_hook_id;

int rtc_read_register(uint32_t addr, uint32_t* data) {
    if(sys_outb(RTC_ADDR_REG, addr)) 
        return 1;

    if(sys_inb(RTC_DATA_REG, data))
        return 1;

    return 0;
}

int rtc_write_register(uint32_t addr, uint32_t data) {
    if (sys_outb(RTC_ADDR_REG, addr))
        return 1;

    if (sys_outb(RTC_DATA_REG, data))
        return 1;

    return 0;
}

time_t get_date() {
    time_t time;
    uint32_t aux;

    rtc_read_register(RTC_REG_UP, &aux);

    if (aux & RTC_WAIT_UIP)
        tickdelay(micros_to_ticks(RTC_WAIT_UIP));

    rtc_read_register(RTC_SEC, &aux);
    time.seconds = aux;

    rtc_read_register(RTC_MIN, &aux);
    time.minutes = aux;

    rtc_read_register(RTC_HOUR, &aux);
    time.hour = aux;

    return t;
}

int rtc_subscribe_int(uint32_t *bit_mask) {
    if (!bit_mask) // Check if pointer is NULL
        return 1;

    rtc_hook_id = RTC_IRQ;
    *bit_mask = BIT(RTC_IRQ);

    if (sys_irqsetpolicy(RTC_IRQ, IRQ_REENABLE, &rtc_hook_id))
        return 1;

    return 0;
}

int rtc_unsubscribe_int() {
    return sys_irqrmpolicy(&rtc_hook_id);
}

void rtc_ih() {
    int cause;
    uint32_t regA;

    sys_outb(RTC_ADDR_REG, RTC_REG_C);
    cause = sys_inb(RTC_DATA_REG, &regA);
    
    // Alarm interrupts
    if (cause & RTC_AIE)
        printf("alarm");

    // Update interrupts
    if (cause & RTC_UIE)
        printf("update");

    // Periodic interrupts
    if (cause & RTC_PIE)
        printf("periodic");
}

int rtc_enable_alarm_int() {
    uint32_t aux;
    
    if (rtc_read_register(RTC_REG_COUNT, &aux))
        return 1;

    aux |= RTC_AIE;

    if (rtc_write_register(RTC_REG_COUNT, aux))
        return 1;

    return 0;
}

int rtc_disable_alarm_int() {
    uint32_t aux;

    if (rtc_read_register(RTC_REG_COUNT, &aux))
        return 1;

    aux &= ~RTC_AIE;

    if (rtc_write_register(RTC_DATA_REG, aux))
        return 1;

    return 0;
}

void rtc_set_alarm(uint32_t period) {
    time_t time = get_date();

    time.seconds += period;
    
    while (time.seconds >= 60) {
        time.seconds -= 60;
        time.minutes++;
    }
    
    while (time.minutes >= 60) {
        time.minutes -= 60;
        time.hour++;
    }
    
    while (time.hour >= 24) {
        time.hour -= 24;
    }

    rtc_write_register(RTC_SEC_ALARM, t.seconds);
    rtc_write_register(RTC_MIN_ALARM, t.minutes); 
    rtc_write_register(RTC_HOUR_ALARM, t.hour);

    rtc_enable_alarm_int();

    //printf("Alarm set to: %02x:%02x:%02x\n", t.hour, t.minutes, t.seconds);
}
