# INA3221 IC HAL for power management
hal_ina3221.h and hal_ina3221.c are the hal file.  
ina3221.c is the file that user need to implement I2C function.
  
Example:  
```c
extern INA3221_I2C_t ina3221_i2c;
extern INA3221_t ina3221;

init_i2c();   // I2C hardware initialize
ina3221.init();
ina3221.set_warning_current(1, 12);
ina3221.set_warning_current(2, 12);
ina3221.set_warning_current(3, 12);

ina3221.update();
printf("%.2f, %.2f, %.2f\r\n", ina3221.ch1_current, ina3221.ch2_current, ina3221.ch3_current);

/* Warning example */
uint8_t a = INA3221_WARN_CH1;
ina3221_i2c.i2c_write(&a, 1);
ina3221_i2c.i2c_read((uint8_t *)&data, 2);

a = INA3221_WARN_CH2;
ina3221_i2c.i2c_write(&a, 1);
ina3221_i2c.i2c_read((uint8_t *)&data, 2);

a = INA3221_WARN_CH3;
ina3221_i2c.i2c_write(&a, 1);
ina3221_i2c.i2c_read((uint8_t *)&data, 2);

```
