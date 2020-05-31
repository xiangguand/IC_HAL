# DRV8847(S) H bridge IC
hal_drv8847.h and hal_drv8847.c are the hal file.  
drv8847_s.c is the file that user need to implement I2C or GPIO function.  
  
Compiler can add flag `-DDRV8847` or `-DDRV8847S` to choose GPIO or I2C.
  
Example:  
```c
init_i2c();   // I2C hardware initialize
lcd.init();
lcd.setCursor(0, 0);
lcd.wirte_multi_words("Hello");
```


