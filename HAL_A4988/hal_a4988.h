#ifndef HAL_A4988
#define HAL_A4988
#include <stdint.h>

#define STEP_PULSE_COUNT 5

#define A4988_CLOCKWISE           0
#define A4988_COUNTERCLOCKWISE    1

#define MICROSTEP_FULL_STEP       0
#define MICROSTEP_FALF_STEP       1
#define MICROSTEP_QUARTER_STEP    2
#define MICROSTEP_EIGHTH_STEP     3
#define MICROSTEP_SIXTEENTH_STEP  7

typedef struct _a4988_io {
    void (*reset_high)(void);
    void (*reset_low)(void);
    void (*sleep_high)(void);
    void (*sleep_low)(void);
    void (*enable_high)(void);
    void (*enable_low)(void);
    void (*ms1_high)(void);
    void (*ms1_low)(void);
    void (*ms2_high)(void);
    void (*ms2_low)(void);
    void (*ms3_high)(void);
    void (*ms3_low)(void);
    void (*dir_high)(void);
    void (*dir_low)(void);
    void (*step_high)(void);
    void (*step_low)(void);
}a4988_io_t;

typedef struct _a4988 {
    uint32_t counter;
    uint8_t direction;
    void (*init)(void);
    void (*reset)(void);
    void (*start_sleep_mode)(void);
    void (*end_sleep_mode)(void);
    void (*enable)(void);
    void (*disable)(void);
    void (*set_microstep_res)(uint8_t microstep);
    void (*clockwise)(void);
    void (*counterclockwise)(void);
    void (*pulse)(void);
}a4988_t;

#endif /* HAL_A4988 */
