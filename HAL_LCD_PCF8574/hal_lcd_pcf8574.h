#ifndef HAL_LCD_PCF8574_H
#define HAL_LCD_PCF8574_H
#include <stdint.h>

#define PCF8574_SLA 0x27
#define DELAY_COUNTER 100
#define LCD_RS_PIN  0   // H:Data, L:Instruction
#define LCD_RW_PIN  1   // L:Write, H:Read
#define LCD_EN_PIN  2   // H:Enable, L:Disable
#define LCD_D3_PIN  3   // H:Back light ON, L:OFF
#define LCD_D4_PIN  4
#define LCD_D5_PIN  5
#define LCD_D6_PIN  6
#define LCD_D7_PIN  7

#define LCD_DATA_BIT         1<<LCD_RS_PIN
#define LCD_INSTRUCTION_BIT  0<<LCD_RS_PIN
#define LCD_WRITE_BIT        0<<LCD_RW_PIN
#define LCD_READ_BIT         1<<LCD_RW_PIN
#define LCD_EN_BIT           1<<LCD_EN_PIN
#define LCD_LIGHT_BIT        1<<LCD_D3_PIN
#define LCD_D4_BIT           1<<LCD_D4_PIN
#define LCD_D5_BIT           1<<LCD_D5_PIN
#define LCD_D6_BIT           1<<LCD_D6_PIN
#define LCD_D7_BIT           1<<LCD_D7_PIN

#define LCD_NORMAL           LCD_LIGHT_BIT

// commands
#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

// flags for display entry mode
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// flags for display on/off control
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

// flags for display/cursor shift
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE 0x00
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00

// flags for function set
#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_1LINE 0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00

// flags for backlight control
#define LCD_BACKLIGHT 0x08
#define LCD_NOBACKLIGHT 0x00

#define LCD_ROW0_FIRST_POS   0x80
#define LCD_ROW1_FIRST_POS   0xC0

typedef struct _PCF8574{
    void (*write_byte)(uint8_t data);    
    void (*write_bytes)(uint8_t *pdata, uint8_t bytes);
    void (*_delay_ms)(uint32_t delay_ms);
    void (*pulseEn)(uint8_t data);
}PCF8574_t;

typedef struct _LCD{
    void (*init)(void);
    void (*clear)(void);
    void (*setCursor)(uint8_t col, uint8_t row);
    void (*send_cmd)(uint8_t cmd);
    void (*wirte)(uint8_t word);
    void (*wirte_multi_words)(uint8_t *word);
}LCD_t;

#endif  /* HAL_LCD_PCF8574_H */
