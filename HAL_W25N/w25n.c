#include "hal_w25n.h"

static inline void hw_w25n_init(void);
static inline uint8_t hw_w25n_spi_swap(uint8_t data);
static inline void hw_w25n_cs_low(void);
static inline void hw_w25n_cs_high(void);
static inline void hw_w25n_wp_low(void);
static inline void hw_w25n_wp_high(void);

hw_w25n_t hw_w25n = {
    .init = hw_w25n_init,
    .spi_swap = hw_w25n_spi_swap,
    .cs_low = hw_w25n_cs_low,
    .cs_high = hw_w25n_cs_high,
    .wp_low = hw_w25n_wp_low,
    .wp_high = hw_w25n_wp_high
};

static inline void hw_w25n_init(void) {

}

static inline uint8_t hw_w25n_spi_swap(uint8_t data) {
    return 0;
}

static inline void hw_w25n_cs_low(void) {

}

static inline void hw_w25n_cs_high(void) {

}

static inline void hw_w25n_wp_low(void) {
    __asm("nop");
}

static inline void hw_w25n_wp_high(void) {
    __asm("nop");
}
