#include "hal_mfrc522.h"
#include "spi.h"

void mfrc522_cs_high(void);
void mfrc522_cs_low(void);
void mfrc522_reset_high(void);
void mfrc522_reset_low(void);
void mfrc522_read_write_bytes(uint8_t reg, uint8_t *txData, uint8_t *rxData, uint8_t sz);
void mfrc522_write_bytes(uint8_t reg, uint8_t *data, uint8_t sz);
void mfrc522_write_byte(uint8_t reg, uint8_t txData);
void mfrc522_read_bytes(uint8_t reg, uint8_t *data, uint8_t sz);
uint8_t mfrc522_read_byte(uint8_t reg);

mfrc522_dri_t mfrc522_dri = {
    .cs_high = mfrc522_cs_high,                   \
    .cs_low = mfrc522_cs_low,                     \
    .reset_high = mfrc522_reset_high,             \
    .reset_low = mfrc522_reset_low,               \
    .write_byte = mfrc522_write_byte,             \
    .read_byte = mfrc522_read_byte,               \
};

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

void mfrc522_read_write_bytes(uint8_t reg, uint8_t *txData, uint8_t *rxData, uint8_t sz) {
    mfrc522_cs_low();
    spi_swap(reg);
    for(uint8_t i=0;i<sz;i++) {
        rxData[i] = spi_swap(txData[i]);
    }
    mfrc522_cs_high();
}

void mfrc522_write_bytes(uint8_t reg, uint8_t *txData, uint8_t sz) {
    mfrc522_cs_low();
    spi_swap(reg);
    for(uint8_t i=0;i<sz;i++) {
        spi_swap(txData[i]);
    }
    mfrc522_cs_high();
}

void mfrc522_write_byte(uint8_t reg, uint8_t txData) {
    mfrc522_cs_low();
    spi_swap(reg);
    spi_swap(txData);
    mfrc522_cs_high();
}

void mfrc522_read_bytes(uint8_t reg, uint8_t *rxData, uint8_t sz) {
    mfrc522_cs_low();
    spi_swap(reg | 0x80);
    for(uint8_t i=0;i<sz;i++) {
        rxData[i] = spi_swap(0x87);
    }
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
