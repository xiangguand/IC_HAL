/**
 * @file spi.h
 * @author Xiang-Guan Deng
 * @brief SPI hardware implementation
 * @date 2020.05.29
 *
 * MFRC522 HAL will call these function,
 * user should implement this SPI.
 * 
 */

#ifndef SPI_H
#include <stdint.h>

void init_spi(void);
void cs_high(void);
void cs_low(void);
uint8_t spi_swap(uint8_t txData);
void reset_high(void);
void reset_low(void);

#endif
