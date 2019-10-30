#include "hal_gymcu680.h"
#include "stm32f4xx_hal.h"              // Keil::Device:STM32Cube HAL:Common

static void stm32_uart_send(uint8_t *pdata, uint8_t bytes);
static uint8_t stm32_uart_receive(void);

extern UART_HandleTypeDef uart1;

GYMCU680_UART_t gymcu680_uart = {       \
    .uart_send = stm32_uart_send,       \
    .uart_receive = stm32_uart_receive, \
};

static void stm32_uart_send(uint8_t *pdata, uint8_t bytes) {
    HAL_UART_Transmit(&uart1, pdata, bytes, 100);
}

static uint8_t stm32_uart_receive(void) {
    uint8_t temp;
    HAL_UART_Receive(&uart1, &temp, 1, 100);
    return temp;
}


