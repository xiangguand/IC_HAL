#include "hal_lcd_pcf8574.h"

static void write_byte(uint8_t data); 
static void write_bytes(uint8_t *pdata, uint8_t bytes);
static void _delay_ms(uint32_t delay_ms);
static void pulseEn(uint8_t data);

PCF8574_t pcf8574 = {  \
    write_byte,        \
    write_bytes,       \
    _delay_ms,         \
    pulseEn,           \
};

static void write_byte(uint8_t pdata) {
    // call the I2C hardware to send the one byte data
}

static void write_bytes(uint8_t *pdata, uint8_t bytes) {
    // call the I2C hardware to send the multi-byte data
}

static void _delay_ms(uint32_t delay_ms) {
    // delay ms function
}

static void pulseEn(uint8_t data) {
    // (LCD enable bit) or (data), do logic operation
    // delay 1 ms
    // ~(LCD distable bit) and (data), do logic operation
}

