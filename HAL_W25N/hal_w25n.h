/**
 * @file hal_w25n.h
 * @author Danny Deng
 * @brief w25n01g hardware abstract file
 * @date 2021.10.04
 *
 * 1 Gbit (119.2 MBytes)
 * 128 Mbit X 8 = 16 MBytes X 8
 * Each block 128 KBytes
 * Each block has 64 pages
 * Each page has 2 KBytes
 * Each page has 2048 bytes main memory(0x000~0x7FF) + 64 bytes spare are
 * Tobal erasable blocks: 1024
 */

#ifndef HAL_W25N_H
#define HAL_W25N_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define W25N_TIMEOUT_COUNTER         1000000

#define W25M_DIE_SELECT              0xC2

#define W25N_RESET                   0xFF
#define W25N_JEDEC_ID                0x9F
#define W25N_READ_STATUS_REG         0x05
#define W25N_WRITE_STATUS_REG        0x01
#define W25N_WRITE_ENABLE            0x06
#define W25N_WRITE_DISABLE           0x04
#define W25N_BB_MANAGE               0xA1
#define W25N_READ_BBM                0xA5
#define W25N_LAST_ECC_FAIL           0xA9
#define W25N_BLOCK_ERASE             0xD8
#define W25N_PROG_DATA_LOAD          0x02
#define W25N_RAND_PROG_DATA_LOAD     0x84
#define W25N_PROG_EXECUTE            0x10
#define W25N_PAGE_DATA_READ          0x13
#define W25N_READ                    0x03
#define W25N_FAST_READ               0x0B

#define W25N_PROT_REG                0xA0
#define W25N_CONFIG_REG              0xB0
#define W25N_STAT_REG                0xC0

#define WINBOND_MAN_ID               0xEF
#define W25N01GV_DEV_ID              0xAA21

#define W25N01GV_MAX_PAGE            65535
#define W25N_MAX_COLUMN              2112

/* Each page memory, total 2112 */
#define W25N_MAIN_MEMORY_START       0x0000   // 2048 Bytes
#define W25N_SPARE_AREA_START        0x0800   // 64 Bytes

typedef struct _w25n_t {
    uint16_t device_id;
    void (*init)(void);
    void (*soft_reset)(void);
    uint8_t (*get_status)(uint8_t reg_addr);
    void (*set_status)(uint8_t reg_addr, uint8_t set_val);
    void (*write_enable)(void);
    void (*write_disable)(void);
    int (*block_erase)(uint16_t page_addr);
    int (*check_wip)(void);
    int (*block_wip)(void);
    int (*load_prog_data)(uint16_t page_addr, uint16_t column_addr, uint8_t *data_p, uint16_t data_len);
    int (*prog_exec)(uint16_t page_addr);
    int (*read)(uint16_t page_addr, uint16_t column_addr, uint8_t *data_p, uint16_t data_len);
} w25n_t;

typedef struct _hw_w25n_t {
    void (*init)(void);
    uint8_t (*spi_swap)(uint8_t data);
    void (*cs_low)(void);
    void (*cs_high)(void);
    void (*wp_low)(void);
    void (*wp_high)(void);
} hw_w25n_t;

#ifdef __cplusplus
}
#endif

#endif /* HAL_W25N_H */
