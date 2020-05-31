/**
 * @file drv8847_s.h
 * @author Xiang-Guan Deng
 * @brief Hardware implement
 * @date 2020.xx.xx
 *
 */

#include "hal_drv8847.h"
#include "drv8847_s.h"

/**
* @brief DRV8847 pin map
* 1A : PTC2
* 1B : PTC1
* 2B : PTB1
* 2A : PTB0
* MODE : PTE18
* NFAULT : PTD5
* TRQ : PTE19
* NSLEEP : PTC3
* Rsense1 : PTE24
* Rsense2 : ADC0_SE23
*/

static void drv8847_sleep_low(void);
static void drv8847_sleep_high(void);
static uint8_t drv8847_get_fault(void);
static void drv8847_set_period1(uint16_t period);
static void drv8847_set_period2(uint16_t period);
static void drv8847_set_duty1(uint16_t duty);
static void drv8847_set_duty2(uint16_t duty);
static void drv8847_mcu_trig1A1B(void);
static void drv8847_mcu_trig2A2B(void);

#if DRV8847
static void drv8847_mode_4pin(void);
static void drv8847_mode_2pin(void);
static void drv8847_trq_full(void);
static void drv8847_trq_half(void);
drv8847_io_t drv8847_dri = {                       \
    .status = I2C_STATUS_OK,                       \
    .ch = ADC_CH_R_SENSE1,                         \
    .v_r1 = 0,                                     \
    .v_r2 = 0,                                     \
    .sleep_low = drv8847_sleep_low,                \
    .sleep_high = drv8847_sleep_high,              \
    .mode_4pin = drv8847_mode_4pin,                \
    .mode_2pin = drv8847_mode_2pin,                \
    .trq_full = drv8847_trq_full,                  \
    .trq_half = drv8847_trq_half,                  \
    .get_fault = drv8847_get_fault,                \
    .set_period1 = drv8847_set_period1,            \
    .set_period2 = drv8847_set_period2,            \
    .set_duty1 = drv8847_set_duty1,                \
    .set_duty2 = drv8847_set_duty2,                \
    .mcu_trig1A1B = drv8847_mcu_trig1A1B,          \
    .mcu_trig2A2B = drv8847_mcu_trig2A2B,          \
};

#else if DRV8847S
static void start_tansmission(void);
static void repeat_start_tansmission(void);
static void enter_receive_mode(void);
static void stop_tansmission(void);
static void wait_tansmission(void);
static void send_byte(uint8_t data);
static uint8_t get_byte(void);
static uint8_t is_ack(void);
void i2c_write(uint8_t regAdd, uint8_t trm_data);
uint8_t i2c_read(uint8_t regAdd);

static void drv8847s_mode_4pin(void);
static void drv8847s_mode_2pin(void);
static void drv8847s_trq_full(void);
static void drv8847s_trq_half(void);

drv8847_io_t drv8847_dri = {                       \
    .status = I2C_STATUS_OK,                       \
    .ch = ADC_CH_R_SENSE1,                         \
    .v_r1 = 0,                                     \
    .v_r2 = 0,                                     \
    .sleep_low = drv8847_sleep_low,                \
    .sleep_high = drv8847_sleep_high,              \
    .mode_4pin = drv8847s_mode_4pin,               \
    .mode_2pin = drv8847s_mode_2pin,               \
    .trq_full = drv8847s_trq_full,                 \
    .trq_half = drv8847s_trq_half,                 \
    .i2c_write = i2c_write,                        \
    .i2c_read = i2c_read,                          \
    .get_fault = drv8847_get_fault,                \
    .set_period1 = drv8847_set_period1,            \
    .set_period2 = drv8847_set_period2,            \
    .set_duty1 = drv8847_set_duty1,                \
    .set_duty2 = drv8847_set_duty2,                \
    .mcu_trig1A1B = drv8847_mcu_trig1A1B,          \
    .mcu_trig2A2B = drv8847_mcu_trig2A2B,          \
};
#endif

static void drv8847_sleep_low(void) {
    GPIO_NSLEEP->PCOR |= (1<<PIN_NSLEEP);
}

static void drv8847_sleep_high(void) {
    GPIO_NSLEEP->PSOR |= (1<<PIN_NSLEEP);
}

#if DRV8847
static void drv8847_mode_4pin(void) {
    GPIO_MODE->PCOR |= (1<<PIN_MODE);
}

static void drv8847_mode_2pin(void) {
    GPIO_MODE->PSOR |= (1<<PIN_MODE);
}

static void drv8847_trq_full(void) {
    GPIO_TRQ->PCOR |= (1<<PIN_TRQ);
}

static void drv8847_trq_half(void) {
    GPIO_TRQ->PSOR |= (1<<PIN_TRQ);
}

#else if DRV8847S
static void start_tansmission(void) {
    /*  */
}

static void repeat_start_tansmission(void) {
    /*  */
}

static void enter_receive_mode(void) {
    /*  */
}

static void stop_tansmission(void) {
    /*  */
}

static void wait_tansmission(void) {
    /*  */
}

static void send_byte(uint8_t data) {
    /*  */
}

static uint8_t get_byte(void) {
    /*  */
}

static uint8_t is_ack(void) {
    return !((DRV8847S_I2C->S & I2C_S_RXAK_MASK) >> I2C_S_RXAK_MASK);
}

void i2c_write(uint8_t regAdd, uint8_t trm_data) {
    /* Start */
    start_tansmission();
    send_byte(((DRV8847S_ADDRESS << 1U) & 0xFE));
    if(!is_ack()) {stop_tansmission(); drv8847_dri.status = I2C_STATUS_NACK; return;}

    /* point to hardware register address */
    send_byte(regAdd);
    if(!is_ack()) {stop_tansmission(); drv8847_dri.status = I2C_STATUS_NACK; return;}

    /* write data to hardware register address */
    send_byte(trm_data);
    if(!is_ack()) {stop_tansmission(); drv8847_dri.status = I2C_STATUS_NACK; return;}

    stop_tansmission();
}

uint8_t i2c_read(uint8_t regAdd) {
    /* Start */
    start_tansmission();
    send_byte(((DRV8847S_ADDRESS << 1U) & 0xFE));
    if(!is_ack()) {stop_tansmission(); drv8847_dri.status = I2C_STATUS_NACK; return 0;}

    /* point to hardware register address */
    send_byte(regAdd);
    if(!is_ack()) {stop_tansmission(); drv8847_dri.status = I2C_STATUS_NACK; return 0;}

    /* repeat start to read hardware register */
    repeat_start_tansmission();
    send_byte(((DRV8847S_ADDRESS << 1U) | 0x01));
    if(!is_ack()) {stop_tansmission(); drv8847_dri.status = I2C_STATUS_NACK; return 0;}

    enter_receive_mode();
    uint8_t rec_data = get_byte();
    stop_tansmission();

    return rec_data;
}

static void drv8847s_mode_4pin(void) {
    uint8_t temp = drv8847_dri.i2c_read(DRV8847S_IC1_CON);
    temp &= ~(IC1_MODE_MASK);
    temp |= IC1_MODE_4PIN;
    drv8847_dri.i2c_write(DRV8847S_IC1_CON, temp);
}

static void drv8847s_mode_2pin(void) {
    uint8_t temp = drv8847_dri.i2c_read(DRV8847S_IC1_CON);
    temp &= ~(IC1_MODE_MASK);
    temp |= IC1_MODE_2PIN;
    drv8847_dri.i2c_write(DRV8847S_IC1_CON, temp);
}

static void drv8847s_trq_full(void) {
    uint8_t temp = drv8847_dri.i2c_read(DRV8847S_IC1_CON);
    temp &= ~(IC1_TRQ_MASK);
    temp |= IC1_TRQ_FULL;
    drv8847_dri.i2c_write(DRV8847S_IC1_CON, temp);
}

static void drv8847s_trq_half(void) {
    uint8_t temp = drv8847_dri.i2c_read(DRV8847S_IC1_CON);
    temp &= ~(IC1_TRQ_MASK);
    temp |= IC1_TRQ_HALF;
    drv8847_dri.i2c_write(DRV8847S_IC1_CON, temp);
}
#endif

static uint8_t drv8847_get_fault(void) {
    return (GPIO_NFAULT->PDIR & (1<<PIN_NFAULT)) >> PIN_NFAULT;
}

static void drv8847_set_period1(uint16_t period) {
    drv8847_dri.period1 = period;
    FTM_1A1B->MOD = period;
}

static void drv8847_set_period2(uint16_t period) {
    drv8847_dri.period2 = period;
}

static void drv8847_set_duty1(uint16_t duty) {
    drv8847_dri.duty1 = duty;
}

static void drv8847_set_duty2(uint16_t duty) {
    drv8847_dri.duty2 = duty;
}

static void drv8847_mcu_trig1A1B(void) {
    ADC_R_SENSE->SC1[0] =  ADC_CH_R_SENSE1;
    while(!(ADC_R_SENSE->SC1[0]&ADC_SC1_COCO_MASK));
    drv8847_dri.v_r1 = ADC_R_SENSE->R[0];
}

static void drv8847_mcu_trig2A2B(void) {
    ADC_R_SENSE->SC1[0] =  ADC_CH_R_SENSE2;
    while(!(ADC_R_SENSE->SC1[0]&ADC_SC1_COCO_MASK));
    drv8847_dri.v_r2 = ADC_R_SENSE->R[0];
}
