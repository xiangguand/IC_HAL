#include "hal_ina3221.h"
#include <stdint.h>

static void ina3221_init(void);
static void ina3221_update(void);
static void ina3221_update_ch1(void);
static void ina3221_update_ch2(void);
static void ina3221_update_ch3(void);
static int ina3221_cal_voltage(uint8_t *raw_data);
static float ina3221_cal_current(uint8_t *raw_data);
static void ina3221_set_mode(uint8_t mode);
static void ina3221_set_warning_current(uint8_t channel, float amp_ma);
static void ina3221_set_config(uint16_t config);

extern INA3221_I2C_t ina3221_i2c;

INA3221_t ina3221 = {                                   \
    .config = 0,                                        \
    .ch1_voltage = 0,                                   \
    .ch1_current = 0,                                   \
    .ch2_voltage = 0,                                   \
    .ch2_current = 0,                                   \
    .ch3_voltage = 0,                                   \
    .ch3_current = 0,                                   \
    .id = 0,                                            \
    .init = ina3221_init,                               \
    .update = ina3221_update,                           \
    .update_ch1 = ina3221_update_ch1,                   \
    .update_ch2 = ina3221_update_ch2,                   \
    .update_ch3 = ina3221_update_ch3,                   \
    .cal_voltage = ina3221_cal_voltage,                 \
    .cal_current = ina3221_cal_current,                 \
    .set_mode = ina3221_set_mode,                       \
    .set_warning_current = ina3221_set_warning_current, \
    .set_config = ina3221_set_config,                   \
};

static void ina3221_init(void) {
    ina3221.config = INA3221_CONFIG_ENABLE_CHAN1 | INA3221_CONFIG_ENABLE_CHAN2 | INA3221_CONFIG_ENABLE_CHAN3 |
                     INA3221_CONFIG_AVG1 | INA3221_CONFIG_VBUS_CT2 | INA3221_CONFIG_VSH_CT2;
    ina3221.set_mode(7);   // default
    ina3221_i2c.i2c_read((uint8_t *)&ina3221.id, 2);
    ina3221_set_config(ina3221.config);
}

static void ina3221_update(void) {
    // Read the ch1 ~ ch3 voltage and current
    ina3221_update_ch1();
    ina3221_update_ch2();
    ina3221_update_ch3();
}

static void ina3221_update_ch1(void) {
    uint8_t addr;
    uint8_t temp[2];
    addr = INA3221_CH1_BUS;
    ina3221_i2c.i2c_write(&addr, 1);    // pointer address ch1 BUS
    ina3221_i2c.i2c_read((uint8_t *)&temp, 2);
    ina3221.ch1_voltage = ina3221.cal_voltage(temp);
    
    addr = INA3221_CH1_SHUNT;
    ina3221_i2c.i2c_write(&addr, 1);    // pointer address ch1 SHUNT
    ina3221_i2c.i2c_read((uint8_t *)&temp, 2);
    ina3221.ch1_current = ina3221.cal_current(temp);
}

static void ina3221_update_ch2(void) {
uint8_t addr;
    uint8_t temp[2];
    addr = INA3221_CH2_BUS;
    ina3221_i2c.i2c_write(&addr, 1);    // pointer address ch2 BUS
    ina3221_i2c.i2c_read((uint8_t *)&temp, 2);
    ina3221.ch2_voltage = ina3221.cal_voltage(temp);
    
    addr = INA3221_CH2_SHUNT;
    ina3221_i2c.i2c_write(&addr, 1);    // pointer address ch2 SHUNT
    ina3221_i2c.i2c_read((uint8_t *)&temp, 2);
    ina3221.ch2_current = ina3221.cal_current(temp);
}

static void ina3221_update_ch3(void) {
    uint8_t addr;
    uint8_t temp[2];
    addr = INA3221_CH3_BUS;
    ina3221_i2c.i2c_write(&addr, 1);    // pointer address ch3 BUS
    ina3221_i2c.i2c_read((uint8_t *)&temp, 2);
    ina3221.ch3_voltage = ina3221.cal_voltage(temp);
    
    addr = INA3221_CH3_SHUNT;
    ina3221_i2c.i2c_write(&addr, 1);    // pointer address ch3 SHUNT
    ina3221_i2c.i2c_read((uint8_t *)&temp, 2);
    ina3221.ch3_current = ina3221.cal_current(temp);
}

static int ina3221_cal_voltage(uint8_t *raw_data) {
    if(raw_data[0] & 0x80) {
        return -(0xFFFF - ((raw_data[0]<<8 | raw_data[1])) + 1);
    }
    else {
        return ((raw_data[0]<<8 | raw_data[1]));
    }
}

static float ina3221_cal_current(uint8_t *raw_data) {
    if(raw_data[0] & 0x80) {
        return -(0xFFFF - ((raw_data[0]<<8 | raw_data[1])) + 1)*0.04/INA3221_SHUNT_RESISTOR;
    }
    else {
        return ((raw_data[0]<<8 | raw_data[1]))*0.04/INA3221_SHUNT_RESISTOR;
    }
}

static void ina3221_set_mode(uint8_t mode) {
    if(mode <= 7) {
        ina3221.config &= ~0x0007;
        ina3221.config |= mode;
    }
}

static void ina3221_set_warning_current(uint8_t channel, float amp_ma) {
    uint8_t addr;
    uint16_t warn_mv = (uint16_t)(amp_ma*INA3221_SHUNT_RESISTOR*25);
    switch(channel) {
        case 1:
            addr = INA3221_WARN_CH1;
            break;
        case 2:
            addr = INA3221_WARN_CH2;
            break;
        case 3:
            addr = INA3221_WARN_CH3;
            break;
    }
    uint8_t temp[3] = {addr, (warn_mv&0xFF00)>>8, warn_mv&0xFF};
    ina3221_i2c.i2c_write(temp, 3);
}

static void ina3221_set_config(uint16_t config) {
    uint8_t addr = INA3221_REG_CONFIG;
    ina3221_i2c.i2c_write(&addr, 1);    // pointer address ch1 BUS
    ina3221_i2c.i2c_write((uint8_t *)&ina3221.config, 2);
}
