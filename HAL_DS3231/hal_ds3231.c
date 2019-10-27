#include "hal_ds3231.h"
#include <stdint.h>
#include <time.h>

static void rtc_init(void);
static void rtc_update_time(void);
static void rtc_set_time(struct tm *time);  
static uint8_t rtc_decode(uint8_t data);
static uint8_t rtc_encode(uint8_t data);

extern DS3231_t ds3231;

RTC_t rtc = {                         \
    .time = {0},                      \
    .init = rtc_init,                 \
    .update_time = rtc_update_time,   \
    .set_time = rtc_set_time,         \
    .decode = rtc_decode,             \
    .encode = rtc_encode,             \
};

static void rtc_init(void) {
    uint8_t reg_time = DS3231_TIME_CAL_ADDR;
    ds3231.i2c_transmit((DS3231_SLA<<1), &reg_time, 1);
}

static void rtc_update_time(void) {
    uint8_t reg_time = DS3231_TIME_CAL_ADDR;
    ds3231.i2c_transmit((DS3231_SLA<<1), &reg_time, 1);
    uint8_t rec[7];
    ds3231.i2c_receive((DS3231_SLA<<1)|1, rec, sizeof(rec));
    
    // decode the rec data to time struct
    rtc.time.tm_sec = rtc.decode(rec[0]);  
    rtc.time.tm_min = rtc.decode(rec[1]); 
    rtc.time.tm_hour = rtc.decode(rec[2]); 
    rtc.time.tm_wday = rec[3];
    rtc.time.tm_mday = rtc.decode(rec[4]); 
    rtc.time.tm_mon = rtc.decode(rec[5]&0x1F) - 1;
    rtc.time.tm_year = rtc.decode(rec[6]) + 2000 - 1900;
}

static void rtc_set_time(struct tm *time) {
    uint8_t send[8];
    send[0] = DS3231_TIME_CAL_ADDR;
    send[1] = rtc.encode(time->tm_sec);
    send[2] = rtc.encode(time->tm_min);
    send[3] = rtc.encode(time->tm_hour);
    send[4] = time->tm_wday;
    send[5] = rtc.encode(time->tm_mday) | DS3231_CENTURY;
    send[6] = rtc.encode(time->tm_mon);
    send[7] = rtc.encode(time->tm_year);

    // write to DS3231 hardware
    ds3231.i2c_transmit((DS3231_SLA<<1), send, sizeof(send));
}

static uint8_t rtc_decode(uint8_t data) {
    return (data>>4)*10 + (data&0x0F);
}

static uint8_t rtc_encode(uint8_t data) {
    return ((data/10)<<4) + (data%10);
}
