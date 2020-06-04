# MFRC522 IC HAL

hal_mfrc522.h and hal_mfrc522.c are the hal file.  
mfrc522.c is the file implement basic spi function.  
spi.c is hardware dependent, user need to implement SPI swap function.
  
Example:  
```c
HAL_Init();

/* Hardware init */
init_spi();

/* Reset MFRC522 */
mfrc522.hardware_reset();

printf("Version is %x\r\n", mfrc522.dri->read_byte(VersionReg));
printf("Version is %x\r\n", mfrc522.dri->read_byte(VersionReg));

mfrc522.init();
printf("Antenna : %x \r\n", mfrc522.dri->read_byte(TxControlReg));
printf("Antenna Gain : %x \r\n", mfrc522.pcd_getAntennaGain());
printf("status1: %x\r\n", mfrc522.dri->read_byte(Status1Reg));
printf("status2: %x\r\n", mfrc522.dri->read_byte(Status2Reg));
printf("waterlevel: %x\r\n", mfrc522.dri->read_byte(WaterLevelReg));

/* Infinite loop */
printf("Start !!! \r\n");
uint16_t add = 0;
while (true) {
    anti_col1_t card_uid;

    if(mfrc522.isNewCardPresent() == STATUS_OK) {
    
    /* Do Anticollision L1 */
    if(mfrc522.pcd_antColLoop1(&card_uid) == STATUS_OK) {
        printf("UID: %x%x%x%x \r\n", card_uid.uid[0], card_uid.uid[1], card_uid.uid[2], card_uid.uid[3]);
    }
    else {
        printf(" Fail AntiCol1 !!! \r\n");
    }

    uint8_t card_sak;
    /* Do Select */
    if(mfrc522.pcd_select(&card_uid, &card_sak) == STATUS_OK) {
        printf("Card SAK: %x \r\n", card_sak);
    }
    else {
        printf(" Fail select !!! \r\n");
    }
    
    
    block_unit_t temp_block;
    for(uint8_t i=0;i<16;i++) temp_block.data[i] = 0;
    printf("\r\n ===== Read ===== \r\n");
    printf("add: %2x, %2x, %2x, %2x, %2x, %2x, %2x, %2x, %2x, %2x, %2x, %2x, %2x, %2x, %2x, %2x, \r\n", 
            0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
    printf("-------------------------------------------------------------------\r\n");

    uint8_t sector_block_num = 3;
    for(add=sector_block_num-3;add<sector_block_num+1;add++) {
        /* Do authenticate */
        if(mfrc522.pcd_auth(PICC_CMD_MF_AUTH_KEY_B, sector_block_num, &card_uid, key2) == STATUS_OK) {
            /* Do read, 16 section, each section has 4 blocks, each block has 16 bit */
            if(mfrc522.pcd_read_block(add, &temp_block) == STATUS_OK) {
                printf("%3x: ", add);
                for(uint8_t i=0;i<16;i++) {
                    printf("%2x, ", temp_block.data[i]);
                }
                printf("\r\n");
            }
            else {
                printf("Read fail !!!\r\n");
            }
        }
        else {
            printf("auth error \r\n");
        }
    }

    
    if(mfrc522.pcd_auth(PICC_CMD_MF_AUTH_KEY_B, sector_block_num, &card_uid, key2) == STATUS_OK) {
        printf("\r\n Try to write ... \r\n");
        /* Do Write */
        for(uint8_t i=0;i<16;i++) write_block.data[i] = 0xFF;
        write_block.data[6] = 0xFF;
        write_block.data[7] = 0x07;
        write_block.data[8] = 0x80;
        
        if(mfrc522.pcd_write_block(sector_block_num-2, &write_block) == STATUS_OK) {
            printf("Write OK !!! \r\n");
        }
        else {
            printf("Write fail !!!\r\n");
        }
    }
    else {
        printf("NOOOOO\r\n");
    }

    /* Do Halt */
    mfrc522.picc_halt();
    
    }
```
