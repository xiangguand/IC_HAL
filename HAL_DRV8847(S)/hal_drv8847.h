/**
 * @file hal_drv8847.h
 * @author Xiang-Guan Deng
 * @brief DRV8847(S) HAL
 * @date 2020.xx.xx
 *
 * DRV8847(S) HAL
 * ADC conversion time : 6.8 us
 */

#ifndef HAL_DRV8847_H
#define HAL_DRV8847_H
#include <stdint.h>

/* ADC parameters */
#define R_SENSE                   0.15
#define ADC_RES                   65535
#define ADC_REF                   3.3

#define DRV8847_STATUS_OK         0
#define DRV8847_STATUS_FAULT      1

#define DRV8847_MODE_TWO_PIN      0
#define DRV8847_MODE_FOUR_PIN     1
#define DRV8847_MODE_SLEEP        2
#define DRV8847_MODE_OPERATION    3

#define DRV8847_TRQ_HALF          4
#define DRV8847_TRQ_FULL          5

/* Step motor direction, default is clockwise */

enum STEP_DIR{
    DIR_CLOCKWISE,
    DIR_COUNTERCLOCKWISE
};

typedef struct _drv8847_io {
    uint8_t status;
    volatile uint8_t ch;
    volatile uint16_t v_r1;
    volatile uint16_t v_r2;
    uint16_t period1;
    uint16_t period2;
    uint16_t duty1;
    uint16_t duty2;
    void (*sleep_low)(void);
    void (*sleep_high)(void);
    void (*mode_4pin)(void);
    void (*mode_2pin)(void);
    void (*trq_full)(void);
    void (*trq_half)(void);
#ifdef DRV8847S
    void (*i2c_write)(uint8_t regAdd, uint8_t trm_data);
    uint8_t (*i2c_read)(uint8_t regAdd);
#endif
    uint8_t (*get_fault)(void);
    void (*set_period1)(uint16_t period);
    void (*set_period2)(uint16_t period);
    void (*set_duty1)(uint16_t duty);
    void (*set_duty2)(uint16_t duty);
    void (*set_channel)(uint8_t ch);
    void (*mcu_trig1A1B)(void);
    void (*mcu_trig2A2B)(void);
}drv8847_io_t;

typedef struct _a4988 {
    drv8847_io_t *drv;
    float i1;
    float i2;
    uint8_t status;
    void (*init)(void);
    void (*setMode)(uint8_t mode);
    void (*setTorque)(uint8_t trq);
    void (*adc_trig1A1B)(void);
    void (*adc_trig2A2B)(void);
    void (*update_current)(void);
}drv8847_t;

#endif /* HAL_DRV8847_H */
