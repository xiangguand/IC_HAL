#include "hal_mfrc522.h"
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

static void mfrc_hardware_init(void);
static void mfrc_pcd_writeRegister(uint8_t reg, uint8_t value);
static uint8_t mfrc_pcd_readRegister(uint8_t reg);
static void mfrc_pcd_setRegisterBitMask(uint8_t reg, uint8_t mask);
static void mfrc_pcd_clearRegisterBitMask(uint8_t reg, uint8_t mask);
static void mfrc_init(void);
static void mfrc_hardware_reset(void);
static void mfrc_init_power_down(uint8_t chipSelectPin, uint8_t resetPowerDownPin);
static void mfrc_pcd_reset(void);
static void mfrc_pcd_antennaOn(void);
static void mfrc_pcd_AntennaOff(void);
static uint8_t mfrc_pcd_getAntennaGain(void);
static void mfrc_pcd_setAntennaGain(uint8_t gain_0_to_7);
static uint8_t mfrc_pcd_PerformSelfTest(void);
static uint8_t mfrc_isNewCardPresent(void);
static uint8_t mfrc_picc_REQA(uint8_t *buff_atqa, uint8_t buff_sz);
static uint8_t mfrc_picc_halt(void);
static uint8_t mfrc_pcd_communicate_picc(uint8_t cmd, uint8_t rfid_irq, uint8_t *txData, uint8_t tx_sz, 
                                  uint8_t *rxData, uint8_t rx_sz, uint8_t bitFrame);
static uint8_t mfrc_pcd_antColLoop1(anti_col1_t *result);
static uint8_t mfrc_pcd_select(anti_col1_t *card_uid, uint8_t *sak);
static uint8_t mfrc_pcd_auth(anti_col1_t *card_uid, uint8_t *key);
static uint8_t mfrc_pcd_read_block(uint8_t addr, block_unit_t *block_data);
static uint8_t mfrc_pcd_write_block(uint8_t addr, block_unit_t *block_data);
static void mfrc_calculateCRC(uint8_t *data, int len, uint8_t *result);

extern mfrc522_dri_t mfrc522_dri;

hal_mfc522_t mfrc522 = {
    .dri = &mfrc522_dri,                                          \
    .hardware_init = mfrc_hardware_init,                          \
    .pcd_writeRegister = mfrc_pcd_writeRegister,                  \
    .pcd_readRegister = mfrc_pcd_readRegister,                    \
    .pcd_setRegisterBitMask = mfrc_pcd_setRegisterBitMask,        \
    .pcd_clearRegisterBitMask = mfrc_pcd_clearRegisterBitMask,    \
    .init = mfrc_init,                                            \
    .hardware_reset = mfrc_hardware_reset,                        \
    .init_power_down = mfrc_init_power_down,                      \
    .pcd_reset = mfrc_pcd_reset,                                  \
    .pcd_antennaOn = mfrc_pcd_antennaOn,                          \
    .pcd_AntennaOff = mfrc_pcd_AntennaOff,                        \
    .pcd_getAntennaGain = mfrc_pcd_getAntennaGain,                \
    .pcd_setAntennaGain = mfrc_pcd_setAntennaGain,                \
    .pcd_PerformSelfTest = mfrc_pcd_PerformSelfTest,              \
    .isNewCardPresent = mfrc_isNewCardPresent,                    \
    .picc_REQA = mfrc_picc_REQA,                                  \
    .picc_halt = mfrc_picc_halt,                                  \
    .pcd_communicate_picc = mfrc_pcd_communicate_picc,            \
    .pcd_antColLoop1= mfrc_pcd_antColLoop1,                       \
    .pcd_select = mfrc_pcd_select,                                \
    .pcd_auth = mfrc_pcd_auth,                                    \
    .pcd_read_block = mfrc_pcd_read_block,                        \
    .pcd_write_block = mfrc_pcd_write_block,                      \
    .calculateCRC = mfrc_calculateCRC,                            \
};

static void mfrc_hardware_init(void) {
    mfrc522.dri->hw_init();
}

static void mfrc_pcd_writeRegister(uint8_t reg, uint8_t value) {
    mfrc522.dri->cs_low();
    mfrc522.dri->write_byte(reg, value);
    mfrc522.dri->cs_high();
}

static uint8_t mfrc_pcd_readRegister(uint8_t reg) {
    return mfrc522.dri->read_byte(reg);
}

static void mfrc_pcd_setRegisterBitMask(uint8_t reg, uint8_t mask) {
    mfrc522.dri->write_byte(reg, (mfrc522.pcd_readRegister(reg) | (mask)));
}

static void mfrc_pcd_clearRegisterBitMask(uint8_t reg, uint8_t mask) {
    mfrc522.dri->write_byte(reg, (mfrc522.pcd_readRegister(reg) & (~mask)));
}

static void mfrc_init(void) {
    mfrc522.dri->write_byte(TxModeReg, 0x00);
	mfrc522.dri->write_byte(RxModeReg, 0x00);
	// Reset ModWidthReg
	mfrc522.dri->write_byte(ModWidthReg, 0x26);

	// When communicating with a PICC we need a timeout if something goes wrong.
	// f_timer = 13.56 MHz / (2*TPreScaler+1) where TPreScaler = [TPrescaler_Hi:TPrescaler_Lo].
	// TPrescaler_Hi are the four low bits in TModeReg. TPrescaler_Lo is TPrescalerReg.
	mfrc522.dri->write_byte(TModeReg, 0x80);			// TAuto=1; timer starts automatically at the end of the transmission in all communication modes at all speeds
	mfrc522.dri->write_byte(TPrescalerReg, 0xA9);		// TPreScaler = TModeReg[3..0]:TPrescalerReg, ie 0x0A9 = 169 => f_timer=40kHz, ie a timer period of 25s.
	mfrc522.dri->write_byte(TReloadRegH, 0x03);		// Reload timer with 0x3E8 = 1000, ie 25ms before timeout.
	mfrc522.dri->write_byte(TReloadRegL, 0xE8);
	
	mfrc522.dri->write_byte(TxASKReg, 0x40);		// Default 0x00. Force a 100 % ASK modulation independent of the ModGsPReg register setting
	mfrc522.dri->write_byte(ModeReg, 0x3D);
    
    mfrc522.dri->write_byte(FIFOLevelReg, 0x80);		// flush the FIFO buffer
    
    mfrc522.pcd_antennaOn();
    mfrc522.pcd_setAntennaGain(4);  /* Max is 7 */
}

static void mfrc_hardware_reset(void) {
    mfrc522.dri->reset_low();
    /* Delay for a while */
    for(uint32_t i=0;i<1000000;i++) __asm("nop");
    mfrc522.dri->reset_high();
}

static void mfrc_init_power_down(uint8_t chipSelectPin, uint8_t resetPowerDownPin) {

}

static void mfrc_pcd_reset(void) {

}

static void mfrc_pcd_antennaOn(void) { 
    mfrc522.pcd_setRegisterBitMask(TxControlReg, ANTENNA_ON_OFF_MASK);
}

static void mfrc_pcd_AntennaOff(void) {
    mfrc522.pcd_clearRegisterBitMask(TxControlReg, ANTENNA_ON_OFF_MASK);
}

static uint8_t mfrc_pcd_getAntennaGain(void) {
    return (mfrc522.pcd_readRegister(RFCfgReg) & ANTENNA_GAIN_MASK) >> ANTENNA_GAIN_POS;
}

static void mfrc_pcd_setAntennaGain(uint8_t gain_0_to_7) {
    mfrc522.dri->write_byte(RFCfgReg, (mfrc522.pcd_readRegister(RFCfgReg) & (~ANTENNA_GAIN_MASK)) 
                                          | ((gain_0_to_7 << ANTENNA_GAIN_POS) & ANTENNA_GAIN_MASK));
}

static uint8_t mfrc_pcd_PerformSelfTest(void) {

    return STATUS_OK;
}

static uint8_t mfrc_isNewCardPresent(void) {
    uint8_t buff_atqa[2] = {0, 0};
    
    // Reset baud rates
	mfrc522.dri->write_byte(TxModeReg, 0x00);
	mfrc522.dri->write_byte(RxModeReg, 0x00);
	// Reset ModWidthReg
	mfrc522.dri->write_byte(ModWidthReg, 0x26);
    
    uint8_t status = mfrc522.picc_REQA(buff_atqa, sizeof(buff_atqa));
    if(buff_atqa[0] != 0 || buff_atqa[1] != 0) {
        printf("buf_atqa: %x, %x\r\n", buff_atqa[0], buff_atqa[1]);
    }
    return status;
}

static uint8_t mfrc_picc_REQA(uint8_t *buff_atqa, uint8_t buff_sz) {
    uint8_t txData = PICC_CMD_REQA;
    mfrc522.pcd_clearRegisterBitMask(CollReg, 0x80);
    return mfrc522.pcd_communicate_picc(PCD_Transceive, 0x30, &txData, 1, buff_atqa, buff_sz, 0x07);
}

static uint8_t mfrc_picc_halt(void) {
    mfrc522.dri->write_byte(CollReg, 0x80);
    uint8_t txData[] = {PICC_CMD_HALT, 0};
    mfrc522.calculateCRC(txData, 2, &txData[2]);
    uint8_t dummy;
    if(mfrc522.pcd_communicate_picc(PCD_Transceive, 0x30, txData, 4, &dummy, 1, 0) == STATUS_TIMEOUT) {
        return STATUS_OK;
    }
    else {
        return STATUS_ERROR;
    }
    
}

static uint8_t mfrc_pcd_communicate_picc(uint8_t cmd, uint8_t rfid_irq, uint8_t *txData, uint8_t tx_sz, 
                                   uint8_t *rxData, uint8_t rx_sz, uint8_t bitFrame) {
    uint8_t temp;
    int32_t i;
    mfrc522.dri->write_byte(BitFramingReg, bitFrame);		// Bit adjustments
    mfrc522.dri->write_byte(CommandReg, PCD_Idle);			// Stop any active command.
	mfrc522.dri->write_byte(ComIrqReg, 0x7F);			    // Clear all seven interrupt request bits
	mfrc522.dri->write_byte(FIFOLevelReg, 0x80);		    // FlushBuffer = 1, FIFO initialization
    for(i=0;i<tx_sz;i++) {
        mfrc522.dri->write_byte(FIFODataReg, txData[i]);
    }                                                             
	mfrc522.dri->write_byte(CommandReg, cmd);				// Execute the command
    if(cmd == PCD_Transceive) {
        mfrc522.pcd_setRegisterBitMask(BitFramingReg, 0x80); // Start send 
    }
    
    for(i=10000;i>0;i--) {
        temp = mfrc522.pcd_readRegister(ComIrqReg);
        if(temp & rfid_irq) break;
        if(temp & 0x01) {
            mfrc522.pcd_clearRegisterBitMask(BitFramingReg, 0x80);
            return STATUS_TIMEOUT;  /* timeout */
        }
    }
    mfrc522.pcd_clearRegisterBitMask(BitFramingReg, 0x80);
    if(i == 0) return STATUS_TIMEOUT;
    
    uint8_t er_reg = mfrc522.pcd_readRegister(ErrorReg);
    uint8_t er_col = mfrc522.pcd_readRegister(CollReg);
    uint8_t n = mfrc522.pcd_readRegister(FIFOLevelReg);
    
    if(er_reg) {
        printf("ER: %x\r\n", er_reg);
        return STATUS_ERROR;
    }
    if(er_col&0x0f) {
        printf("COL: %x\r\n", er_col);
        return STATUS_COLLISION;
    }
    
    if(n == 0) {
        printf("No data\r\n");
        n = 1;
    }
    else if(n > rx_sz) {
        n = rx_sz;
    }
    for(i=0;i<n;i++) {
        rxData[i] = mfrc522.dri->read_byte(FIFODataReg);
    }

    return STATUS_OK;
}

static uint8_t mfrc_pcd_antColLoop1(anti_col1_t *result) {
    uint8_t txData[2] = {PICC_CMD_SEL_CL1, 0x20};
    mfrc522.dri->write_byte(CollReg, 0x80);
    uint8_t anti_col1[5] = {0 ,0 ,0 ,0 ,0};

    /* Do Anticollision L1 */
    if(mfrc522.pcd_communicate_picc(PCD_Transceive, 0x30, txData, 2, anti_col1, 5, 0) == STATUS_OK) {
        memcpy(result, anti_col1, 5);
        return STATUS_OK;
    }
    else {
        return STATUS_ERROR;
    }
}

static uint8_t mfrc_pcd_select(anti_col1_t *card_uid, uint8_t *sak) {
    uint8_t txData[9] = {PICC_CMD_SEL_CL1, 0x70};
    mfrc522.dri->write_byte(CollReg, 0x80);
    uint8_t select_check[3] = {0, 0, 0};
    txData[1] = 0x70;
    for(uint8_t i=0;i<5;i++) {
        txData[i+2] = *(&card_uid->uid[0]+i);
    }
    mfrc522.calculateCRC(txData, 7, &txData[7]);

    if(mfrc522.pcd_communicate_picc(PCD_Transceive, 0x30, txData, 9, select_check, 3, 0) == STATUS_OK) {
        sak[0] = select_check[0];
        return STATUS_OK;
    }
    else {
        return STATUS_ERROR;
    }
}

static uint8_t mfrc_pcd_auth(anti_col1_t *card_uid, uint8_t *key) {
    uint8_t txData[12];
    mfrc522.dri->write_byte(CollReg, 0x80);
    txData[0] = PICC_CMD_MF_AUTH_KEY_A;
    txData[1] = 0x00;
    for(uint8_t i=0;i<MF_KEY_SIZE;i++) {
        txData[i+2] = key[i];
    }
    for(uint8_t i=0;i<4;i++) {
        txData[i+8] = card_uid->uid[i];
    }
    uint8_t temp;
    if(mfrc522.pcd_communicate_picc(PCD_MFAuthent, 0x10, txData, 12, &temp, 1, 0) == STATUS_OK) {
        return STATUS_OK;
    }
    else {
        return STATUS_ERROR;
    }
}

static uint8_t mfrc_pcd_read_block(uint8_t addr, block_unit_t *block_data) {
    uint8_t txData[4];
    mfrc522.dri->write_byte(CollReg, 0x80);
    txData[0] = PICC_CMD_MF_READ;
    txData[1] = addr;
    mfrc522.calculateCRC(txData, 2, &txData[2]);

    if(mfrc522.pcd_communicate_picc(PCD_Transceive, 0x30, txData, 4, block_data->data, 16, 0) == STATUS_OK) {
        return STATUS_OK;
    }
    else {
        return STATUS_ERROR;
    }
}

static uint8_t mfrc_pcd_write_block(uint8_t addr, block_unit_t *block_data) {
    uint8_t txData[18];
    mfrc522.dri->write_byte(CollReg, 0x80);
    txData[0] = PICC_CMD_MF_WRITE;
    txData[1] = addr;
    mfrc522.calculateCRC(txData, 2, &txData[2]);

    uint8_t picc_ack = 0;
    printf("XXXXXXXXXX\r\n");
    if(mfrc522.pcd_communicate_picc(PCD_Transceive, 0x30, txData, 4, &picc_ack, 1, 0x00) != STATUS_OK) {
        return STATUS_ERROR;
    }
    printf("ACK1:%x\r\n", picc_ack);
    if(picc_ack == MF_ACK) {
        for(uint8_t i=0;i<16;i++) {
            txData[i] = block_data->data[i];
        }
        mfrc522.calculateCRC(txData, 16, &txData[16]);
        if(mfrc522.pcd_communicate_picc(PCD_Transceive, 0x30, txData, 18, &picc_ack, 1, 0) == STATUS_OK) {
            printf("ACK:%x\r\n", picc_ack);
            if(picc_ack == MF_ACK) {
                return STATUS_OK;
            }
            else {
                printf("MIFARE NACK QQ\r\n");
                return STATUS_MIFARE_NACK;
            }
        }
        else {
            return STATUS_ERROR;  
        }
    }
    else {
        printf("MIFARE NACK\r\n");
        return STATUS_MIFARE_NACK;
    }
    
}

static void mfrc_calculateCRC(uint8_t *data, int len, uint8_t *result) {
    int i;
    uint8_t n;

    mfrc522.dri->write_byte(CommandReg, PCD_Idle);
    mfrc522.pcd_clearRegisterBitMask(DivIrqReg, 0x04);   // CRCIrq = 0
    mfrc522.pcd_setRegisterBitMask(FIFOLevelReg, 0x80);  // Clear the FIFO pointer

    //Writing data to the FIFO.
    for (i = 0; i < len; i++) {
        mfrc522.dri->write_byte(FIFODataReg, data[i]);
    }
    mfrc522.dri->write_byte(CommandReg, PCD_CalcCRC);

    // Wait for the CRC calculation to complete.
    i = 0xFF;
    do {
        n = mfrc522.pcd_readRegister(DivIrqReg);
        i--;
    } while ((i != 0) && !(n & 0x04));  //CRCIrq = 1

    // Read the result from the CRC calculation.
    result[0] = mfrc522.pcd_readRegister(CRCResultRegL);
    result[1] = mfrc522.pcd_readRegister(CRCResultRegH);
}


