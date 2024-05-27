#ifndef RTC_H
#define RTC_H

#include <lcom/lcf.h>
#include <stdint.h>

/* RTC address and data register ports */
#define RTC_ADDR_REG 0x70
#define RTC_DATA_REG 0x71

/* RTC registers */
#define RTC_SEC 0x00          // Seconds
#define RTC_SEC_ALARM 0x01    // Seconds Alarm
#define RTC_MIN 0x02          // Minutes
#define RTC_MIN_ALARM 0x03    // Minutes Alarm
#define RTC_HOUR 0x04         // Hours
#define RTC_HOUR_ALARM 0x05   // Hours Alarm
#define RTC_DAY_OF_WEEK 0x06  // Day of the Week
#define RTC_DAY_OF_MONTH 0x07 // Day of the Month
#define RTC_MONTH 0x08        // Month
#define RTC_YEAR 0x09         // Year
#define RTC_REG_UP 0x0A       // Register A
#define RTC_REG_COUNT 0x0B    // Register B
#define RTC_REG_C 0x0C        // Register C
#define RTC_REG_D 0x0D        // Register D

/* RTC register bits */
#define RTC_UPDATE BIT(7) // Update in progress

/* RTC IRQ line */
#define RTC_IRQ 8

/*Binary mode*/
#define RTC_BINARY BIT(2)

/*RTC mask*/
#define RTC_MASK BIT(RTC_IRQ) //nao tenho a certeza

/*Struct for real-time information*/
typedef struct {
    uint8_t seconds;
    uint8_t minutes;
    uint8_t hours;
    uint8_t day;
    uint8_t month;
    uint8_t year;
} real_time_info;

extern real_time_info time_info;
extern uint8_t binary_mode;


/* Function prototypes */
void rtc_setup();
int rtc_subscribe_interrupts();
int rtc_unsubscribe_interrupts();
int rtc_output(uint8_t command, uint8_t *output);
int rtc_is_updating();
int rtc_is_binary();
int rtc_is_bcd();
uint8_t to_binary(uint8_t bcd_number);
int rtc_update_time();

#endif /* RTC_H */
