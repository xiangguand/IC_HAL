#include "hal_mfrc522.h"

void mfrc_pcd_writeRegister(uint8_t reg, uint8_t value);
uint8_t mfrc_pcd_readRegister(uint8_t reg);
void mfrc_pcd_readRegisters(uint8_t reg, uint8_t count, uint8_t *values, uint8_t rxAlign);
void mfrc_pcd_setRegisterBitMask(uint8_t reg, uint8_t mask);
void mfrc_pcd_clearRegisterBitMask(uint8_t reg, uint8_t mask);
void mfrc_init(void);
void mfrc_init_hw(uint8_t resetPowerDownPin);
void mfrc_init_power_down(uint8_t chipSelectPin, uint8_t resetPowerDownPin);
void mfrc_pcd_reset(void);
void mfrc_pcd_antennaOn(void);
void mfrc_pcd_AntennaOff(void);
uint8_t mfrc_pcd_getAntennaGain(void);
void mfrc_pcd_setAntennaGain(uint8_t mask);
uint8_t mfrc_pcd_PerformSelfTest(void);

extern mfrc522_dri_t mkrc522_dri;

hal_mfc522_t mfrc522 {
    .dri = &mkrc522_dri,                                          \
    .pcd_writeRegister = mfrc_pcd_writeRegister,                  \
    .pcd_readRegister = mfrc_pcd_readRegister,                  \
    .pcd_readRegisters = mfrc_pcd_readRegisters,                \
    .pcd_setRegisterBitMask = mfrc_pcd_setRegisterBitMask,      \
    .pcd_clearRegisterBitMask = mfrc_pcd_clearRegisterBitMask,  \
    .init = mfrc_init,                                            \
    .init_hw = mfrc_init_hw,                                      \
    .init_power_down = mfrc_init_power_down,                      \
    .pcd_reset = mfrc_pcd_reset,                                  \
    .pcd_antennaOn = mfrc_pcd_antennaOn,                          \
    .pcd_AntennaOff = mfrc_pcd_AntennaOff,                        \
    .pcd_getAntennaGain = mfrc_pcd_getAntennaGain,                \
    .pcd_setAntennaGain = mfrc_pcd_setAntennaGain,                \
    .pcd_PerformSelfTest =mfrc_pcd_PerformSelfTest,               \
};

void mfrc_pcd_writeRegister(uint8_t reg, uint8_t value) {
    mfrc522.dri->cs_low();
    uint8_t temp[2] = {reg, value};
    mfrc522.dri->write_bytes(temp, 2);
    mfrc522.dri->cs_high();
}

uint8_t mfrc_pcd_readRegister(uint8_t reg) {

}

void mfrc_pcd_readRegisters(uint8_t reg, uint8_t count, uint8_t *values, uint8_t rxAlign) {

}

void mfrc_pcd_setRegisterBitMask(uint8_t reg, uint8_t mask) {

}

void mfrc_pcd_clearRegisterBitMask(uint8_t reg, uint8_t mask) {

}

void mfrc_init(void) {

}

void mfrc_init_hw(uint8_t resetPowerDownPin) {

}

void mfrc_init_power_down(uint8_t chipSelectPin, uint8_t resetPowerDownPin) {

}

void mfrc_pcd_reset(void) {

}

void mfrc_pcd_antennaOn(void) {

}

void mfrc_pcd_AntennaOff(void) {

}

uint8_t mfrc_pcd_getAntennaGain(void) {

}

void mfrc_pcd_setAntennaGain(uint8_t mask) {

}

uint8_t mfrc_pcd_PerformSelfTest(void) {

}

