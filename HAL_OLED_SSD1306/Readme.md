# SSD1306 IC HAL for OLED
hal_ssd1306.h and hal_ssd1306.c are the hal file.  
ssd1306.c is the file that user need to implement I2C function.
  
Example:   
```c
extern OLED_t oled;

init_hardware();   // hardware initialize  
drv8847_s.init();  // Initialize DRV8847

/* If use I2C */
if(drv8847_s.status == I2C_STATUS_TIMEOUT) {
    // timeout happen
}
/* Setting mode */
/*
 * DRV8847_MODE_TWO_PIN
 * DRV8847_MODE_FOUR_PIN
 * DRV8847_MODE_SLEEP
 * DRV8847_MODE_OPERATION
 */
drv8847_s.setMode(DRV8847_MODE_SLEEP);

/* Setting torque, full or half */
drv8847_s.setTorque(DRV8847_TRQ_FULL);

```
