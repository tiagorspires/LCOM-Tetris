#include "rtc.h"

int rtc_hook_id = RTC_MASK;
extern real_time_info time_info;
extern uint8_t binary_mode;


void rtc_setup()
{
    binary_mode = rtc_is_binary();
    rtc_update_time();
}

int rtc_subscribe_interrupts()
{
    return sys_irqsetpolicy(RTC_IRQ, IRQ_REENABLE, &rtc_hook_id);
}

int rtc_unsubscribe_interrupts()
{
    return sys_irqrmpolicy(&rtc_hook_id);
}

int rtc_output(uint8_t command, uint8_t *output)
{
    if (sys_outb(REGISTER_INPUT, command) != 0)
        return 1;
    if (util_sys_inb(REGISTER_OUTPUT, output) != 0)
        return 1;
    return 0;
}

int rtc_is_updating()
{
    uint8_t result;
    if (rtc_output(RTC_REG_UP, &result))
        return 1;
    return result & RTC_UPDATE;
}

int rtc_is_binary()
{
    uint8_t result;
    if (rtc_output(RTC_REG_COUNT, &result))
        return 1;
    return result & RTC_BINARY;
}

int rtc_is_bcd()
{
    return !rtc_is_binary();
}

uint8_t to_binary(uint8_t bcd_number)
{
    return ((bcd_number >> 4) * 10) + (bcd_number & 0xF);
}

int rtc_update_time()
{

    if (rtc_is_updating() != 0)
        return 1;
    uint8_t output;

    if (rtc_output(RTC_SEC, &output) != 0)
        return 1;
    time_info.seconds = binary_mode ? output : to_binary(output);

    if (rtc_output(RTC_MIN, &output) != 0)
        return 1;
    time_info.minutes = binary_mode ? output : to_binary(output);

    if (rtc_output(RTC_HOUR, &output) != 0)
        return 1;
    time_info.hours = binary_mode ? output : to_binary(output);

    if (rtc_output(RTC_DAY_OF_MONTH, &output) != 0)
        return 1;
    time_info.day = binary_mode ? output : to_binary(output);

    if (rtc_output(RTC_MONTH, &output) != 0)
        return 1;
    time_info.month = binary_mode ? output : to_binary(output);

    if (rtc_output(RTC_YEAR, &output) != 0)
        return 1;
    time_info.year = binary_mode ? output : to_binary(output);

    return 0;
}