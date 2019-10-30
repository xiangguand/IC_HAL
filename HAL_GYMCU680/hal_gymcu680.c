#include "hal_gymcu680.h"
#include <string.h>

static const uint8_t GYMCU680_INSTRUCTION1[4] = {0xA5, 0x55, 0x3F, 0x39};
static const uint8_t GYMCU680_INSTRUCTION2[4] = {0xA5, 0x56, 0x02, 0xFD};

extern GYMCU680_UART_t gymcu680_uart;

static void gymcu680_send_intruction(void);
static void gymcu680_update(void);


GYMCU680_t gymcu680 = {                             \
    .temperature = 0,                               \
    .humidity = 0,                                  \
    .pressure = 0,                                  \
    .air_iaq_accuaracy = 0,                         \
    .air_iaq = 0,                                   \
    .gas = 0,                                       \
    .altitude = 0,                                  \
    .send_intruction = gymcu680_send_intruction,    \
    .update = gymcu680_update,                      \
};

static void gymcu680_send_intruction(void) {
    gymcu680_uart.uart_send((uint8_t *)GYMCU680_INSTRUCTION1, sizeof(GYMCU680_INSTRUCTION1));
    for(uint32_t i=0;i<GYMCU680_DELAY_TICKS;i++);
    gymcu680_uart.uart_send((uint8_t *)GYMCU680_INSTRUCTION2, sizeof(GYMCU680_INSTRUCTION1));
    for(uint32_t i=0;i<GYMCU680_DELAY_TICKS;i++);
}

// This function is to receive BME680 data from GYMCU680
static void gymcu680_update(void) {
    // packet header 0x5A 0x5A
    static uint8_t bme680_buf[GYMCU68_PACKAGE_SIZE] = {0};
    static uint8_t bme680_i = 0;
    bme680_buf[bme680_i++] = gymcu680_uart.uart_receive();
    if((bme680_i == 1) || (bme680_i == 2)) {
        if(bme680_buf[bme680_i-1] != GYMCU68_HEADER) {
            memset(bme680_buf, 0, GYMCU68_PACKAGE_SIZE);
            bme680_i = 0;
        }
    }
    // Receive data type (temperature, humidity etc.)
    else if(bme680_i == 3) {
       if(bme680_buf[bme680_i-1] != 0x3F) {
            memset(bme680_buf, 0, GYMCU68_PACKAGE_SIZE);
            bme680_i = 0;
        } 
    }
    // Receive data size
    else if(bme680_i == 4) {
       if(bme680_buf[bme680_i-1] != 0x0F) {
            memset(bme680_buf, 0, GYMCU68_PACKAGE_SIZE);
            bme680_i = 0;
        } 
    }
    // Solve the package
    else if(bme680_i == GYMCU68_PACKAGE_SIZE){
        gymcu680.temperature = ((bme680_buf[4]<<8) | (bme680_buf[5])) / 100.0;
        gymcu680.humidity = ((bme680_buf[6]<<8) | (bme680_buf[7])) / 100.0;
        gymcu680.pressure = (bme680_buf[8]<<16) | (bme680_buf[9]<<8) | (bme680_buf[10]);
        gymcu680.air_iaq_accuaracy = (bme680_buf[11]&0xF0) >> 4;
        gymcu680.air_iaq = ((bme680_buf[11]&0x0F)<<8) | (bme680_buf[12]);
        gymcu680.gas = (bme680_buf[13]<<24) | (bme680_buf[14]<<16) | (bme680_buf[15]<<8) | (bme680_buf[16]);
        gymcu680.altitude = (bme680_buf[17]<<8) | (bme680_buf[18]);
        
        // set zero to bme680_buf
        memset(bme680_buf, 0, GYMCU68_PACKAGE_SIZE);
        bme680_i = 0;
    }
}


