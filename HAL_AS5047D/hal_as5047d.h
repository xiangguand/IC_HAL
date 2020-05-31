/**
 * @file hal_as5047d.h
 * @author Xiang-Guan Deng
 * @brief AS5047D HAL
 * @date 2020.xx.xx
 *
 * AS5047D HAL
 * Encoder resolution : 16 bits
 */

#ifndef HAL_AS5047D_H
#define HAL_AS5047D_H
#include <stdint.h>

#define CHIP_CS1          1
#define CHIP_CS2          2
#define CHIP_CS3          3

#define PROG              0x0003
#define CORDICMAG         0x3FFD
#define DIAAGC            0x3FFC
#define ANGLECOM          0x3FFF
#define ANGLE             0x3FFE
#define ERRFL             0x0001
#define ZPOSM             0x0016
#define ZPOSL             0x0017
#define SETTINGS2         0x0019
#define SETTINGS1         0x0018
#define ABIBIN            0b0000000000100000  // Select Binary
#define ABIRES            0b0000000111000000  // select resolution 2048

typedef struct __as5047d_dri {
    uint8_t CHIP_SELECT;
    void (*write)(uint16_t address, uint16_t value);
    uint16_t (*read)(uint16_t address);
    void (*cs_high)(void);
    void (*cs_low)(void);
    uint8_t (*get_parity)(uint16_t n);
}as50474_dri_t;

typedef struct __as5047d {
    uint16_t angle;
    as50474_dri_t *instance;
    void (*set_chip)(uint8_t cs_num);
    void (*init)(void);
    void (*update)(void);
}as50474_t;

#endif  /* HAL_AS5047D_H */
