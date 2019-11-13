#ifndef HAL_INA3221_H
#define HAL_INA3221_H
#include <stdint.h>

/*=========================================================================
I2C ADDRESS/BITS
-----------------------------------------------------------------------*/
#define INA3221_SHUNT_RESISTOR                  (1)
#define INA3221_ADDRESS                         (0x40)    // 1000000 (A0+A1=GND)
#define INA3221_READ                            (0x01)
#define INA3221_MANUFACTURER                    (0xFE)
#define INA3221_CH1_SHUNT                       (0x01)
#define INA3221_CH1_BUS                         (0x02)
#define INA3221_CH2_SHUNT                       (0x03)
#define INA3221_CH2_BUS                         (0x04)
#define INA3221_CH3_SHUNT                       (0x05)
#define INA3221_CH3_BUS                         (0x06)
#define INA3221_MANUFACTURER_ID                 (0x5449)

/*=========================================================================
CONFIG REGISTER (R/W) 
-----------------------------------------------------------------------*/
#define INA3221_REG_CONFIG                      (0x00)
/*---------------------------------------------------------------------*/
#define INA3221_CONFIG_RESET                    (0x8000)  // Reset Bit

#define INA3221_CONFIG_ENABLE_CHAN1             (0x4000)  // Enable Channel 1
#define INA3221_CONFIG_ENABLE_CHAN2             (0x2000)  // Enable Channel 2
#define INA3221_CONFIG_ENABLE_CHAN3             (0x1000)  // Enable Channel 3

#define INA3221_CONFIG_AVG2                     (0x0800)  // AVG Samples Bit 2 - See table 3 spec
#define INA3221_CONFIG_AVG1                     (0x0400)  // AVG Samples Bit 1 - See table 3 spec
#define INA3221_CONFIG_AVG0                     (0x0200)  // AVG Samples Bit 0 - See table 3 spec

#define INA3221_CONFIG_VBUS_CT2                 (0x0100)  // VBUS bit 2 Conversion time - See table 4 spec
#define INA3221_CONFIG_VBUS_CT1                 (0x0080)  // VBUS bit 1 Conversion time - See table 4 spec
#define INA3221_CONFIG_VBUS_CT0                 (0x0040)  // VBUS bit 0 Conversion time - See table 4 spec

#define INA3221_CONFIG_VSH_CT2                  (0x0020)  // Vshunt bit 2 Conversion time - See table 5 spec
#define INA3221_CONFIG_VSH_CT1                  (0x0010)  // Vshunt bit 1 Conversion time - See table 5 spec
#define INA3221_CONFIG_VSH_CT0                  (0x0008)  // Vshunt bit 0 Conversion time - See table 5 spec

#define INA3221_CONFIG_MODE_2                   (0x0004)  // Operating Mode bit 2 - See table 6 spec
#define INA3221_CONFIG_MODE_1                   (0x0002)  // Operating Mode bit 1 - See table 6 spec
#define INA3221_CONFIG_MODE_0                   (0x0001)  // Operating Mode bit 0 - See table 6 spec

/*=========================================================================
WARNING REGISTER (R/W) 
-----------------------------------------------------------------------*/
#define INA3221_WARN_CH1                         (0x08)
#define INA3221_WARN_CH2                         (0x0A)
#define INA3221_WARN_CH3                         (0x0C)
/*=========================================================================*/

typedef struct _ina3221_i2c {
    uint8_t (*i2c_write)(uint8_t *trm_data, uint8_t bytes);
    uint8_t (*i2c_read)(uint8_t *rec_data, uint8_t bytes);
}INA3221_I2C_t;

typedef struct _ina3221 {
    uint16_t config;
    int ch1_voltage;
    float ch1_current;
    int ch2_voltage;
    float ch2_current;
    int ch3_voltage;
    float ch3_current;
    uint16_t id;
    void (*init)(void);
    void (*update)(void);
    void (*update_ch1)(void);
    void (*update_ch2)(void);
    void (*update_ch3)(void);
    int (*cal_voltage)(uint8_t *raw_data);
    float (*cal_current)(uint8_t *raw_data);
    void (*set_mode)(uint8_t mode);
    void (*set_warning_current)(uint8_t channel, float amp_ma);
    void (*set_config)(uint16_t config);
}INA3221_t;

#endif  /* HAL_INA3221_H */
