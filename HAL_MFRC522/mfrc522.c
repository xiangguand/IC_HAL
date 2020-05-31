#include "hal_mfrc522.h"

void mkrc522_cs_high(void);
void mkrc522_cs_low(void);
void mkrc522_write_bytes(uint8_t data, uint8_t sz);
void mkrc522_read_bytes(uint8_t data, uint8_t sz);

mfrc522_dri_t mkrc522_dri = {
    .cs_high = mkrc522_cs_high,          \
    .cs_low = mkrc522_cs_low,            \
    .write_bytes = mkrc522_write_bytes,  \
    .read_bytes = mkrc522_read_bytes,    \
};

void mkrc522_cs_high(void) {

}

void mkrc522_cs_low(void) {

}

void mkrc522_write_bytes(uint8_t data, uint8_t sz) {

}

void mkrc522_read_bytes(uint8_t data, uint8_t sz) {

}

