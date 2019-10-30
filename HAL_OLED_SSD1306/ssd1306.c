#include "stm32f4xx_hal.h"              // Keil::Device:STM32Cube HAL:Common
#include "hal_ssd1306.h"
#include <stdlib.h>

I2C_HandleTypeDef i2c1;
HAL_StatusTypeDef er = HAL_OK;

static void send_command(uint8_t cmd);
static void send_multi_command(uint8_t *cmd, uint16_t bytes);
static void write_multi_pixels(uint8_t *pixel, uint16_t bytes);
static void _delay_ms(uint32_t delay_ms);

SSD1306_t ssd1306 = {
    .send_command = send_command,              \
    .send_multi_command = send_multi_command,  \
    .write_multi_pixels = write_multi_pixels,  \
    ._delay_ms = _delay_ms                     \
};


static void send_command(uint8_t cmd) {
    uint8_t send_cmd[2] = {SSD1306_I2C_COMMAND_MODE, cmd};
    HAL_I2C_Master_Transmit(&i2c1, (SSD1306_ADDRESS<<1), send_cmd, 2, 100); 
}

static void send_multi_command(uint8_t *cmd, uint16_t bytes) {
    uint8_t *send_cmd = (uint8_t *)malloc((bytes+1)*sizeof(uint8_t));
    send_cmd[0] = SSD1306_I2C_COMMAND_MODE;
    for(uint8_t i=0;i<bytes;i++) send_cmd[i+1] = cmd[i];
    HAL_I2C_Master_Transmit(&i2c1, (SSD1306_ADDRESS<<1), send_cmd, bytes+1, 100);
    free(send_cmd);
}

static void write_multi_pixels(uint8_t *pixel, uint16_t bytes) {
    uint8_t *send_data = (uint8_t *)malloc((bytes+1)*sizeof(uint8_t));
    send_data[0] = SSD1306_I2C_DATA_MODE;
    for(uint16_t i=0;i<bytes;i++) send_data[i+1] = pixel[i];
    HAL_I2C_Master_Transmit(&i2c1, (SSD1306_ADDRESS<<1), send_data, bytes+1, 100);
    free(send_data);
}

static void _delay_ms(uint32_t delay_ms) {
    // User need to implement the delay method
}

