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

static void init_spi(void);
static void cs_high(void);
static void cs_low(void);
static uint8_t spi_swap(uint8_t txData);
static void reset_high(void);
static void reset_low(void);

#endif
