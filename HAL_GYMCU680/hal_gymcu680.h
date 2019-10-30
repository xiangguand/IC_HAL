#ifndef HAL_GYMCU680_H
#define HAL_GYMCU680_H
#include <stdint.h>

#define GYMCU680_DELAY_TICKS        10000
#define GYMCU68_HEADER              0x5A
#define GYMCU68_PACKAGE_SIZE        20

/**
* IAQ:
* 0   - 50    : good
* 51  - 100   : average
* 101 - 150   : little bad
* 151 - 200   : bad
* 201 - 300   : worse
* 301 - 500   : very bad
*/


typedef struct _gymcu680_uart {
    void (*uart_send)(uint8_t *pdata, uint8_t bytes);
    uint8_t (*uart_receive)(void);
}GYMCU680_UART_t;

typedef struct _gymcu680 {
    float temperature;
    float humidity;
    uint32_t pressure;
    uint8_t air_iaq_accuaracy;
    uint8_t air_iaq;
    uint32_t gas;
    uint16_t altitude;
    void (*send_intruction)(void);
    void (*update)(void);
}GYMCU680_t;


#endif  /*HAL_GYMCU680_H*/
