#include "hal_ina3221.h"
#include <stdint.h>

static uint8_t ina3221_i2c_write(uint8_t *trm_data, uint8_t bytes);
static uint8_t ina3221_i2c_read(uint8_t *rec_data, uint8_t bytes);

INA3221_I2C_t ina3221_i2c = {                 \
    .i2c_write = ina3221_i2c_write,   \
    .i2c_read = ina3221_i2c_read,     \
};

static uint8_t ina3221_i2c_write(uint8_t *trm_data, uint8_t bytes) {
    // call the I2C hardware to transmit the multi-byte data
}

static uint8_t ina3221_i2c_read(uint8_t *rec_data, uint8_t bytes) {
    // call the I2C hardware to receive the multi-byte data
}

