#include "hal_ssd1306.h"
#include "font.h"
#include <stdlib.h>
#include <string.h>

static void ssd1306_init(void);
static void ssd1306_fill(uint8_t pixel_value);
static void ssd1306_update(void);
static int ssd1306_setBlock(uint8_t pos_x, uint8_t pos_y, uint8_t width, uint8_t height);
static void ssd1306_write_words(uint8_t pos_x, uint8_t pos_y, uint8_t *words);

// Base driver to change GPIO
extern SSD1306_t ssd1306;
char ssd_buf[SSD1306_BUF_SIZE];

OLED_t oled = {                           \
    .init = ssd1306_init,                 \
    .fill = ssd1306_fill,                 \
    .update = ssd1306_update,             \
    .setBlock = ssd1306_setBlock,         \
    .write_words = ssd1306_write_words,   \
};


static void ssd1306_init(void) {
    //oled.ssd_buf = (uint8_t *)malloc(SSD1306_BUF_SIZE*sizeof(uint8_t));
    memset(ssd_buf, 0x00, SSD1306_BUF_SIZE);
    uint8_t temp2[2];
    
    ssd1306.send_command(SSD1306_DISPLAYOFF);
    
    temp2[0] = SSD1306_MEMORYMODE;
    temp2[1] = 0x10;
    ssd1306.send_multi_command(temp2, 2);
    
    ssd1306.send_command(0xB0);
    
    temp2[0] = SSD1306_SETMULTIPLEX;
    temp2[1] = 0x3F;
    ssd1306.send_multi_command(temp2, 2);
    
    temp2[0] = SSD1306_SETDISPLAYOFFSET;
    temp2[1] = 0x00;
    ssd1306.send_multi_command(temp2, 2);
    
    ssd1306.send_command(SSD1306_SETSTARTLINE);
    ssd1306.send_command(SSD1306_SEGREMAP|0x01);   // remap column address, 0->127
    ssd1306.send_command(SSD1306_COMSCANDEC);
    
    temp2[0] = SSD1306_SETCOMPINS;
    temp2[1] = 0x12;
    ssd1306.send_multi_command(temp2, 2);
    
    temp2[0] = SSD1306_SETCONTRAST;
    temp2[1] = 0x7F;
    ssd1306.send_multi_command(temp2, 2);
    
    ssd1306.send_command(SSD1306_DISPLAYALLON_RESUME);
    ssd1306.send_command(SSD1306_NORMALDISPLAY);
    
    temp2[0] = SSD1306_SETDISPLAYCLOCKDIV;
    temp2[1] = 0x80;
    ssd1306.send_multi_command(temp2, 2);
    
    temp2[0] = SSD1306_CHARGEPUMP;
    temp2[1] = 0x14;
    ssd1306.send_multi_command(temp2, 2);
    
    ssd1306.send_command(SSD1306_DISPLAYON);
    
    oled.fill(0x00);
    oled.update();
}

static void ssd1306_fill(uint8_t pixel_value) {
    memset(ssd_buf, pixel_value, SSD1306_BUF_SIZE);
}

static void ssd1306_update(void) {
    uint8_t cmd_point_ram[] = {SSD1306_PAGEADDR, 0x00, 0x07, SSD1306_COLUMNADDR, 0, 127};
    ssd1306.send_multi_command(cmd_point_ram, sizeof(cmd_point_ram));
    uint8_t send_times = SSD1306_BUF_SIZE/MAX_SSD1306_BUF;
    for(uint8_t pg=0;pg<send_times;pg++) {
        ssd1306.write_multi_pixels(&ssd_buf[pg*MAX_SSD1306_BUF], MAX_SSD1306_BUF);
    }
}

// return bytes
// pos_x: 0~127
// pos_y: 0~7
static int ssd1306_setBlock(uint8_t pos_x, uint8_t pos_y, uint8_t width, uint8_t height) {
    if(width > 127 || height > 7) return -1;
    uint8_t cmd[] = {SSD1306_PAGEADDR, pos_y, pos_y+height, SSD1306_COLUMNADDR, pos_x, pos_x+width};
    ssd1306.send_multi_command(cmd, sizeof(cmd));
    return (width+1)*(height+1);
}

static void ssd1306_write_words(uint8_t pos_x, uint8_t pos_y, uint8_t *words) {
    uint8_t temp_x = pos_x;
    while(*words) {
        int byte_num = oled.setBlock(temp_x, pos_y, FONT_COLUMN_NUM-1, 0);
        if(byte_num > 0 ) {
            ssd1306.write_multi_pixels(&FONT(*words++), FONT_WORD_BYTES);
            temp_x += 8;   // one word size is 6*8 bits
        }
    }
}
