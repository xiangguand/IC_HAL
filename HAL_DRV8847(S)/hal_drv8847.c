#include "hal_drv8847.h"
#define NOP_DELAY(void)  for(uint32_t i=0;i<STEP_PULSE_COUNT;i++) __asm("nop")

#define ADC_VALUE_TO_CURRENT      ADC_REF/ADC_RES/R_SENSE

extern drv8847_io_t drv8847_dri;

static void drv8847_init(void);
static void drv8847_setMode(uint8_t mode);
static void drv8847_setTorque(uint8_t trq);
static void drv8847_adc_trig1A1B(void);
static void drv8847_adc_trig2A2B(void);
static void drv8847_update_current(void);

drv8847_t drv8847 = {                              \
    .drv = &drv8847_dri,                           \
    .i1 = 0.0,                                     \
    .i2 = 0.0,                                     \
    .status = DRV8847_STATUS_OK,                   \
    .init = drv8847_init,                          \
    .setMode = drv8847_setMode,                    \
    .setTorque = drv8847_setTorque,                \
    .adc_trig1A1B = drv8847_adc_trig1A1B,          \
    .adc_trig2A2B = drv8847_adc_trig2A2B,          \
    .update_current = drv8847_update_current,      \

};

static void drv8847_setMode(uint8_t mode) {
    switch(mode) {
        case DRV8847_MODE_TWO_PIN:
            drv8847_dri.mode_2pin();
            break;
        case DRV8847_MODE_FOUR_PIN:
            drv8847_dri.mode_4pin();
            break;
        case DRV8847_MODE_SLEEP:
            drv8847_dri.sleep_low();
            break;
        case DRV8847_MODE_OPERATION:
            drv8847_dri.sleep_high();
            break;
    }
}

static void drv8847_init(void) {
    drv8847.setMode(DRV8847_MODE_OPERATION);
    drv8847.setMode(DRV8847_MODE_FOUR_PIN);
    drv8847.setTorque(DRV8847_TRQ_FULL);
}

static void drv8847_setTorque(uint8_t trq) {
    switch(trq) {
        case DRV8847_TRQ_HALF:
            drv8847_dri.trq_half();
            break;
        case DRV8847_TRQ_FULL:
            drv8847_dri.trq_full();
            break;
    }
}

static void drv8847_adc_trig1A1B(void) {
    drv8847.drv->mcu_trig1A1B();
}

static void drv8847_adc_trig2A2B(void) {
    drv8847.drv->mcu_trig2A2B();
}

static void drv8847_update_current(void) {
    drv8847.i1 = drv8847.drv->v_r1*ADC_VALUE_TO_CURRENT;
    drv8847.i2 = drv8847.drv->v_r2*ADC_VALUE_TO_CURRENT;
}
