#include "hal_gymcu680.h"

static void stm32_uart_send(uint8_t *pdata, uint8_t bytes);
static uint8_t stm32_uart_receive(void);

extern UART_HandleTypeDef uart1;

GYMCU680_UART_t gymcu680_uart = {       \
    .uart_send = stm32_uart_send,       \
    .uart_receive = stm32_uart_receive, \
};

static void stm32_uart_send(uint8_t *pdata, uint8_t bytes) {
    // call the hardware uart, to send the data
}

static uint8_t stm32_uart_receive(void) {
    // call the hardware uart, to receive the data
    return 0;
}


