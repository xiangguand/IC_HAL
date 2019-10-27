# DS3231 RTC IC HAL
hal_ds3231.h and hal_ds3231.c are the hal file.  
ds3231.c, user need to implement I2C hardware.
  
  
Example, setting the RTC:  
```
    extern RTC_t rtc;
    
    init_i2c();
    rtc.init();

    // Set the time 2019 10/25 Friday 21:25:45
    struct tm now;
    now.tm_sec = 45;
    now.tm_min = 25;
    now.tm_hour = 21;
    now.tm_wday = 5;
    now.tm_mday = 25;
    now.tm_mon = 10;
    now.tm_year = 19;
    
    rtc.set_time(&now);
```
  
Example, Read the RTC:  
```
    extern RTC_t rtc;
    
    init_i2c();
    rtc.init();

    // update the RTC data
    rtc.update_time();
    // rtc.time is the struct tm that store the RTC data.
```
