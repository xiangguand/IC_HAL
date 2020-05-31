# SSD1306 IC HAL for OLED
hal_ssd1306.h and hal_ssd1306.c are the hal file.  
ssd1306.c is the file that user need to implement I2C function.
  
Example, setting the RTC:  
```c
extern OLED_t oled;

init_i2c();   // I2C hardware initialize  
oled.init();
uint8_t buf[20];
sprintf(buf, "Hello world !!!");
oled.write_words(0, 1, buf);
```
