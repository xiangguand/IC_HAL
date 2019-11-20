#include "hal_a4988.h"
#define NOP_DELAY(void)  for(uint32_t i=0;i<STEP_PULSE_COUNT;i++) __asm("nop")

static void a4988_init(void);
static void a4988_reset(void);
static void a4988_start_sleep_mode(void);
static void a4988_end_sleep_mode(void);
static void a4988_enable(void);
static void a4988_disable(void);
static void a4988_set_microstep_res(uint8_t microstep);
static void a4988_clockwise(void);
static void a4988_counterclockwise(void);
static void a4988_pulse(void);

extern a4988_io_t a4988_io;

a4988_t a4988 = {                                  \
    .counter = 0,                                  \
    .direction = A4988_CLOCKWISE,                  \
    .init = a4988_init,                            \
    .reset = a4988_reset,                          \
    .start_sleep_mode = a4988_start_sleep_mode,    \
    .end_sleep_mode = a4988_end_sleep_mode,        \
    .enable = a4988_enable,                        \
    .disable = a4988_disable,                      \
    .set_microstep_res = a4988_set_microstep_res,  \
    .clockwise = a4988_clockwise,                  \
    .counterclockwise = a4988_counterclockwise,    \
    .pulse = a4988_pulse,                          \
};

static void a4988_init(void) {
    a4988.reset();
    a4988.end_sleep_mode();
    a4988.enable();
    // Default mode
    a4988_set_microstep_res(MICROSTEP_SIXTEENTH_STEP);
    a4988.clockwise();
}

static void a4988_reset(void) {
    a4988_io.reset_low();
    NOP_DELAY();
    a4988_io.reset_high();
}

static void a4988_start_sleep_mode(void) {
    a4988_io.sleep_low();
}

static void a4988_end_sleep_mode(void) {
    a4988_io.sleep_high();
}

static void a4988_enable(void) {
    a4988_io.enable_low();
}

static void a4988_disable(void) {
    a4988_io.enable_high();
}

static void a4988_set_microstep_res(uint8_t microstep) {
    if(microstep & 0x01) a4988_io.ms1_high();
    else a4988_io.ms1_low();
    if(microstep & 0x02) a4988_io.ms2_high();
    else a4988_io.ms2_low();
    if(microstep & 0x04) a4988_io.ms3_high();
    else a4988_io.ms3_low();
}

static void a4988_clockwise(void) {
    a4988_io.dir_high();
    a4988.direction = A4988_CLOCKWISE;
}

static void a4988_counterclockwise(void) {
    a4988_io.dir_low();
    a4988.direction = A4988_COUNTERCLOCKWISE;
}

static void a4988_pulse(void) {
    a4988_io.step_high();
    NOP_DELAY();
    a4988_io.step_low();
    a4988.counter++;
}
