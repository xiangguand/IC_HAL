/*
 *  font.h
 *  i2c
 *
 *  Created by Michael Köhler on 13.09.18.
 *  Copyright 2018 Skie-Systems. All rights reserved.
 *
 */
#ifndef _font_h_
#define _font_h_

#define FONT_FIRST_ASCII   32
#define FONT_COLUMN_NUM    6
#define FONT_WORD_BYTES    6*8/8

#define FONT(ASC_CHAR) ssd1306oled_font[ASC_CHAR-FONT_FIRST_ASCII][0]

extern const char ssd1306oled_font[][6];
extern const char special_char[][2];

#endif
