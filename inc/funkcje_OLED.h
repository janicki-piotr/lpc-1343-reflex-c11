#ifndef __OLED_H
#define __OLED_H

#define OLED_DISPLAY_WIDTH  96
#define OLED_DISPLAY_HEIGHT 64


typedef enum
{
    OLED_COLOR_BLACK,
    OLED_COLOR_WHITE
} oled_color_t;


void oled_init (void);
void oled_putPixel(uint8_t x, uint8_t y, oled_color_t color);
void oled_clearScreen(oled_color_t color);
void oled_putString(uint8_t x, uint8_t y, uint8_t *pStr, oled_color_t fb,
        oled_color_t bg);
uint8_t oled_putChar(uint8_t x, uint8_t y, uint8_t ch, oled_color_t fb, oled_color_t bg);
void writeCommand(uint8_t data);


#endif /* end __OLED_H */
/****************************************************************************
**                            End Of File
*****************************************************************************/
