#ifndef HAL_SSD1306_H
#define HAL_SSD1306_H
#include <stdint.h>

#define SSD1306_ADDRESS 0x3C
// Use OLED 128X64
#define SSD1306_128_64

#define SSD1306_BUF_SIZE            1024   // SSD1306_LCDWIDTH*SSD1306_LCDHEIGHT/8
#define MAX_SSD1306_BUF             128
#define MAX_SSD1306_PAGE            8

#define SSD1306_MEMORYMODE          0x20 ///< See datasheet
#define SSD1306_COLUMNADDR          0x21 ///< See datasheet
#define SSD1306_PAGEADDR            0x22 ///< See datasheet
#define SSD1306_SETCONTRAST         0x81 ///< See datasheet
#define SSD1306_CHARGEPUMP          0x8D ///< See datasheet
#define SSD1306_SEGREMAP            0xA0 ///< See datasheet
#define SSD1306_DISPLAYALLON_RESUME 0xA4 ///< See datasheet
#define SSD1306_DISPLAYALLON        0xA5 ///< Not currently used
#define SSD1306_NORMALDISPLAY       0xA6 ///< See datasheet
#define SSD1306_INVERTDISPLAY       0xA7 ///< See datasheet
#define SSD1306_SETMULTIPLEX        0xA8 ///< See datasheet
#define SSD1306_DISPLAYOFF          0xAE ///< See datasheet
#define SSD1306_DISPLAYON           0xAF ///< See datasheet
#define SSD1306_COMSCANINC          0xC0 ///< Not currently used
#define SSD1306_COMSCANDEC          0xC8 ///< See datasheet
#define SSD1306_SETDISPLAYOFFSET    0xD3 ///< See datasheet
#define SSD1306_SETDISPLAYCLOCKDIV  0xD5 ///< See datasheet
#define SSD1306_SETPRECHARGE        0xD9 ///< See datasheet
#define SSD1306_SETCOMPINS          0xDA ///< See datasheet
#define SSD1306_SETVCOMDETECT       0xDB ///< See datasheet

#define SSD1306_SETLOWCOLUMN        0x00 ///< Not currently used
#define SSD1306_SETHIGHCOLUMN       0x10 ///< Not currently used
#define SSD1306_SETSTARTLINE        0x40 ///< See datasheet

#define SSD1306_EXTERNALVCC         0x01 ///< External display voltage source
#define SSD1306_SWITCHCAPVCC        0x02 ///< Gen. display voltage from 3.3V

#define SSD1306_RIGHT_HORIZONTAL_SCROLL              0x26 ///< Init rt scroll
#define SSD1306_LEFT_HORIZONTAL_SCROLL               0x27 ///< Init left scroll
#define SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL 0x29 ///< Init diag scroll
#define SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL  0x2A ///< Init diag scroll
#define SSD1306_DEACTIVATE_SCROLL                    0x2E ///< Stop scroll
#define SSD1306_ACTIVATE_SCROLL                      0x2F ///< Start scroll
#define SSD1306_SET_VERTICAL_SCROLL_AREA             0xA3 ///< Set scroll range

// Deprecated size stuff for backwards compatibility with old sketches
#if defined SSD1306_128_64
 #define SSD1306_LCDWIDTH  128 ///< DEPRECATED: width w/SSD1306_128_64 defined
 #define SSD1306_LCDHEIGHT  64 ///< DEPRECATED: height w/SSD1306_128_64 defined
#endif
#if defined SSD1306_128_32
 #define SSD1306_LCDWIDTH  128 ///< DEPRECATED: width w/SSD1306_128_32 defined
 #define SSD1306_LCDHEIGHT  32 ///< DEPRECATED: height w/SSD1306_128_32 defined
#endif
#if defined SSD1306_96_16
 #define SSD1306_LCDWIDTH   96 ///< DEPRECATED: width w/SSD1306_96_16 defined
 #define SSD1306_LCDHEIGHT  16 ///< DEPRECATED: height w/SSD1306_96_16 defined
#endif

#define SSD1306_I2C_COMMAND_MODE  0x00   // D/C:L
#define SSD1306_I2C_DATA_MODE     0x40   // D/C:H

typedef struct _SSD1306{
    void (*send_command)(uint8_t cmd);       // D/C:Low, transfer command  
    void (*send_multi_command)(uint8_t *cmd, uint16_t bytes);
    void (*write_multi_pixels)(const char *pixel, uint16_t bytes);
    void (*_delay_ms)(uint32_t delay_ms);
}SSD1306_t;

typedef struct _OLED{
    void (*init)(void);
    void (*fill)(uint8_t pixel_value);
    void (*update)(void);
    int (*setBlock)(uint8_t pos_x, uint8_t pos_y, uint8_t width, uint8_t height);
    void (*write_words)(uint8_t pos_x, uint8_t pos_y, uint8_t *words);
}OLED_t;


#endif  /*HAL_SSD1306_H*/
