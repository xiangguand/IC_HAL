#include "hal_ds3231.h"

static void i2c_transmit(uint8_t sla_address, uint8_t *send_data, uint8_t bytes);
static void i2c_receive(uint8_t sla_address, uint8_t *rec_data, uint8_t bytes);

I2C_HandleTypeDef i2c1;

DS3231_t ds3231 = {                \
    .i2c_transmit = i2c_transmit,  \
    .i2c_receive = i2c_receive,    \
};

static void i2c_transmit(uint8_t sla_address, uint8_t *send_data, uint8_t bytes) {
    // call the I2C hardware to transmit data
    // [start+ACK] [sla_address+W+ACK] [send_data+ACK]
}

static void i2c_receive(uint8_t sla_address, uint8_t *rec_data, uint8_t bytes) {
    // call the I2C hardware to receive data
    // [start+ACK] [sla_address+R+ACK] [receive_data+ACK]
}
