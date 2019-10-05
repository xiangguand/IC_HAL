#include "hal_lcd_pcf8574.h"

static void lcd_init(void);
static void lcd_clear(void);
static void lcd_setCursor(uint8_t col, uint8_t row);
static void lcd_send_cmd(uint8_t cmd);
static void lcd_wirte(uint8_t word);
static void lcd_wirte_multi_words(uint8_t *word);

LCD_t lcd = {             \
    lcd_init,             \
    lcd_clear,            \
    lcd_setCursor,        \
    lcd_send_cmd,         \
    lcd_wirte,            \
    lcd_wirte_multi_words,\
};

// Base driver to change GPIO
extern PCF8574_t pcf8574;

static void lcd_init(void) {
    lcd_send_cmd(0x02);  // we start in 8bit mode, try to set 4 bit mode
    lcd_send_cmd(0x28);  // enable 5x7 mode for chars 
    lcd_send_cmd(0x0E);  // Display OFF, Cursor ON
    lcd_clear();         // Clear Display
    lcd_send_cmd(0x80);  // Move the cursor to beginning of first line
}

static void lcd_clear(void) { 
    lcd_send_cmd(LCD_CLEARDISPLAY);
    pcf8574._delay_ms(100);
}

static void lcd_setCursor(uint8_t col, uint8_t row) {
    if(row == 0) {
        if(col < 16) {
            lcd_send_cmd(col + LCD_ROW0_FIRST_POS);
        }
    }
    else if(row == 1) {
        if(col < 16) {
            lcd_send_cmd(col + LCD_ROW1_FIRST_POS);
        }
    }
}

static void lcd_send_cmd(uint8_t cmd) {
    uint8_t dataH = LCD_INSTRUCTION_BIT | LCD_BACKLIGHT | (cmd&0xf0);
    uint8_t dataL = LCD_INSTRUCTION_BIT | LCD_BACKLIGHT | ((cmd<<4)&0xf0);
    
    pcf8574.write_byte(dataH);
    pcf8574.pulseEn(dataH);
    
    pcf8574.write_byte(dataL);
    pcf8574.pulseEn(dataL);
}

static void lcd_wirte(uint8_t word) {
    uint8_t dataH = LCD_DATA_BIT | LCD_BACKLIGHT | (word&0xf0);
    uint8_t dataL = LCD_DATA_BIT | LCD_BACKLIGHT | ((word<<4)&0xf0);
    
    pcf8574.write_byte(dataH);
    pcf8574.pulseEn(dataH);
    
    pcf8574.write_byte(dataL);
    pcf8574.pulseEn(dataL);
}
static void lcd_wirte_multi_words(uint8_t *words) {
    while(*words){
		lcd_wirte(*words++);
	}
}
