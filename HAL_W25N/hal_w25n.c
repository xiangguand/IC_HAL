/**
 * @file hal_w25n.c
 * @author Danny Deng
 * @brief w25n01g hardware abstract file
 * @date 2021.10.04
 *
 */

#include <assert.h>
#include "hal_w25n.h"

extern hw_w25n_t hw_w25n;

static void w25n_init(void);
static void w25n_soft_reset(void);
static uint8_t w25n_get_status(uint8_t reg_addr);
static void w25n_set_status(uint8_t reg_addr, uint8_t set_val);
static void w25n_write_enable(void);
static void w25n_write_disable(void);
static int w25n_block_erase(uint16_t page_addr);
static int w25n_check_wip(void);
static int w25n_block_wip(void);
static int w25n_load_prog_data(uint16_t page_addr, uint16_t column_addr, uint8_t *data_p, uint16_t data_len);
static int w25n_prog_exec(uint16_t page_addr);
static int w25n_read(uint16_t page_addr, uint16_t column_addr, uint8_t *data_p, uint16_t data_len);

w25n_t w25n = {
    .device_id = 0x00,
    .init = w25n_init,
    .soft_reset = w25n_soft_reset,
    .get_status = w25n_get_status,
    .set_status = w25n_set_status,
    .write_enable = w25n_write_enable,
    .write_disable = w25n_write_disable,
    .block_erase = w25n_block_erase,
    .check_wip = w25n_check_wip,
    .block_wip = w25n_block_wip,
    .load_prog_data = w25n_load_prog_data,
    .prog_exec = w25n_prog_exec,
    .read = w25n_read
};

static void w25n_init(void) {
    hw_w25n.init();
    w25n.soft_reset();
    uint8_t temp[3];
    hw_w25n.cs_low();
    hw_w25n.spi_swap(W25N_JEDEC_ID);
    hw_w25n.spi_swap(0x00);           // dummy
    temp[0] = hw_w25n.spi_swap(0x00); // WINBOND_MAN_ID (0xFE)
    temp[1] = hw_w25n.spi_swap(0x00); // W25N01GV_DEV_ID high byte
    temp[2] = hw_w25n.spi_swap(0x00); // W25N01GV_DEV_ID low byte
    hw_w25n.cs_high();

    // Disable all protect erase write
    w25n.set_status(W25N_PROT_REG, 0x00);

    w25n.device_id = (uint16_t)(temp[1] << 8) |  temp[2];
}

static void w25n_soft_reset(void) {
    hw_w25n.cs_low();
    (void)hw_w25n.spi_swap(W25N_RESET);
    hw_w25n.cs_high();
}

static uint8_t w25n_get_status(uint8_t reg_addr) {
    hw_w25n.cs_low();
    (void)hw_w25n.spi_swap(W25N_READ_STATUS_REG);
    (void)hw_w25n.spi_swap(reg_addr);
    uint8_t temp = hw_w25n.spi_swap(0x00);
    hw_w25n.cs_high();
    
    return temp;
}

static void w25n_set_status(uint8_t reg_addr, uint8_t set_val) {
    hw_w25n.cs_low();
    (void)hw_w25n.spi_swap(W25N_WRITE_STATUS_REG);
    (void)hw_w25n.spi_swap(reg_addr);
    (void)hw_w25n.spi_swap(set_val);
    hw_w25n.cs_high();
}

static void w25n_write_enable(void) {
    hw_w25n.cs_low();
    (void)hw_w25n.spi_swap(W25N_WRITE_ENABLE);
    hw_w25n.cs_high();
}

static void w25n_write_disable(void) {
    hw_w25n.cs_low();
    (void)hw_w25n.spi_swap(W25N_WRITE_DISABLE);
    hw_w25n.cs_high();
}

static int w25n_block_erase(uint16_t page_addr) {
    uint8_t pageHigh = (uint8_t)((page_addr & 0xFF00) >> 8);
    uint8_t pageLow = (uint8_t)(page_addr&0x00FF);


    if(w25n.block_wip()) {
        assert(0);
        return 1;
    }
    w25n.write_enable();
    
    hw_w25n.cs_low();
    hw_w25n.spi_swap(W25N_BLOCK_ERASE);
    hw_w25n.spi_swap(0x00);
    hw_w25n.spi_swap(pageHigh);
    hw_w25n.spi_swap(pageLow);
    hw_w25n.cs_high();

    return 0;
}

static int w25n_check_wip(void) {
    uint8_t status = w25n.get_status(W25N_STAT_REG);
    if(status & 0x01) {
        return 1;
    }
    else if(status & 0x08) {
        // Program fail
        assert(0);
        return 2;
    }

    return 0;
}

static int w25n_block_wip(void) {
    uint32_t cnt = 0;
    while(w25n.check_wip()) {
        __asm("nop");
        if(cnt++ > W25N_TIMEOUT_COUNTER) {
            return 1;
        }
    }
    return 0;
}

static int w25n_load_prog_data(uint16_t page_addr, uint16_t column_addr, uint8_t *data_p, uint16_t data_len) {
    if(column_addr > (uint32_t)W25N_MAX_COLUMN) return 1;
    if(data_len > (uint32_t)W25N_MAX_COLUMN - column_addr) return 1;

    uint8_t columnHigh = (uint8_t)(column_addr&0xFF00) >> 8;
    uint8_t columnLow = (uint8_t)(column_addr&0x00FF);

    if(w25n.block_wip()) {
        assert(0);
        return 1;
    }
    w25n.write_enable();
    
    hw_w25n.cs_low();
    (void)hw_w25n.spi_swap(W25N_PROG_DATA_LOAD);
    (void)hw_w25n.spi_swap(columnHigh);
    (void)hw_w25n.spi_swap(columnLow);
    for(uint16_t i=0;i<data_len;i++) {
        (void)hw_w25n.spi_swap(data_p[i]);
    }
    hw_w25n.cs_high();

    return 0;
}

static int w25n_prog_exec(uint16_t page_addr) {
    uint8_t pageHigh = (uint8_t)((page_addr & 0xFF00) >> 8);
    uint8_t pageLow = (uint8_t)(page_addr);

    w25n.write_enable();

    hw_w25n.cs_low();
    (void)hw_w25n.spi_swap(W25N_PROG_EXECUTE);
    (void)hw_w25n.spi_swap(0x00);
    (void)hw_w25n.spi_swap(pageHigh);
    (void)hw_w25n.spi_swap(pageLow);
    hw_w25n.cs_high();

    return 0;
}

static int w25n_read(uint16_t page_addr, uint16_t column_addr, uint8_t *data_p, uint16_t data_len) {
    /* Point to flash page data */
    uint8_t pageHigh = (uint8_t)((page_addr & 0xFF00) >> 8);
    uint8_t pageLow = (uint8_t)(page_addr & 0x00FF);

    if(w25n.block_wip()) {
        assert(0);
        return 1;
    }

    hw_w25n.cs_low();
    hw_w25n.spi_swap(W25N_PAGE_DATA_READ);
    hw_w25n.spi_swap(0x00);
    hw_w25n.spi_swap(pageHigh);
    hw_w25n.spi_swap(pageLow);
    hw_w25n.cs_high();


    /* Start to read the flash data */
    if(column_addr > (uint32_t)W25N_MAX_COLUMN) return 1;
    if(data_len > (uint32_t)W25N_MAX_COLUMN - column_addr) return 1;

    uint8_t columnHigh = (column_addr & 0xFF00) >> 8;
    uint8_t columnLow = column_addr & 0x00FF;

    if(w25n.block_wip()) {
        assert(0);
        return 1;
    }

    hw_w25n.cs_low();
    hw_w25n.spi_swap(W25N_READ);
    hw_w25n.spi_swap(columnHigh);
    hw_w25n.spi_swap(columnLow);
    hw_w25n.spi_swap(0x00);
    for(uint16_t i=0;i<data_len;i++) {
        data_p[i] = hw_w25n.spi_swap(0x00);
    }
    hw_w25n.cs_high();

    return 0;
}
