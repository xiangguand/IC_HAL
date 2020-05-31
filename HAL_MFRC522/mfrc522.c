#include "hal_mfrc522.h"
#include "spi.h"

static void mfrc522_hw_init(void);
static void mfrc522_cs_high(void);
static void mfrc522_cs_low(void);
static void mfrc522_reset_high(void);
static void mfrc522_reset_low(void);
static void mfrc522_write_byte(uint8_t reg, uint8_t txData);
static uint8_t mfrc522_read_byte(uint8_t reg);

mfrc522_dri_t mfrc522_dri = {
    .hw_init = mfrc522_hw_init,                   \
    .cs_high = mfrc522_cs_high,                   \
    .cs_low = mfrc522_cs_low,                     \
    .reset_high = mfrc522_reset_high,             \
    .reset_low = mfrc522_reset_low,               \
    .write_byte = mfrc522_write_byte,             \
    .read_byte = mfrc522_read_byte,               \
};

void mfrc522_hw_init(void) {
    init_spi();
}

void mfrc522_cs_high(void) {
    cs_high();
}

void mfrc522_cs_low(void) {
    cs_low();
}

void mfrc522_reset_high(void) {
    reset_high();
}

void mfrc522_reset_low(void) {
    reset_low();
}

void mfrc522_write_byte(uint8_t reg, uint8_t txData) {
    mfrc522_cs_low();
    spi_swap(reg);
    spi_swap(txData);
    mfrc522_cs_high();
}

uint8_t mfrc522_read_byte(uint8_t reg) {
    uint8_t temp;
    mfrc522_cs_low();
    spi_swap(reg | 0x80);
    temp = spi_swap(0x87);
    mfrc522_cs_high();
    return temp;
}
