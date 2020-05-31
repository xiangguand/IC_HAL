# PCF8574 IC HAL for LCD I2C interface
hal_lcd_pcf8574.h and hal_lcd_pcf8574.c are the hal file.  
lcd_pcf8574.c is the file that user need to implement I2C function.
  
Example, setting the RTC:  
```c
init_i2c();   // I2C hardware initialize
lcd.init();
lcd.setCursor(0, 0);
lcd.wirte_multi_words("Hello");
```
