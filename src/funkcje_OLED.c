#include <funkcje_gpio.h>
#include "mcu_regs.h"
#include "type.h"
#include "string.h"
#include "funkcje_SSP.h"
#include "funkcje_OLED.h"
#include "font5x7.h"

#define OLED_CS_OFF() GPIO_SetValue( PORT0, 2, 1 )
#define OLED_CS_ON()  GPIO_SetValue( PORT0, 2, 0 )
#define OLED_DATA()   GPIO_SetValue( PORT2, 7, 1 )
#define OLED_CMD()    GPIO_SetValue( PORT2, 7, 0 )

/*
 * The display controller can handle a resolution of 132x64. The OLED
 * on the base board is 96x64.
 */
#define X_OFFSET 18

#define SHADOW_FB_SIZE (OLED_DISPLAY_WIDTH*OLED_DISPLAY_HEIGHT >> 3)

#define setAddress(page,lowerAddr,higherAddr)\
    writeCommand(page);\
    writeCommand(lowerAddr);\
    writeCommand(higherAddr);

/*
 * The SSD1305 doesn't support reading from the display memory when using
 * serial mode (only parallel mode). Since it isn't possible to write only
 * one pixel to the display (a minimum of one column, 8 pixels, is always
 * written) a shadow framebuffer is needed to keep track of the display
 * data.
 */
static uint8_t shadowFB[SHADOW_FB_SIZE];

static uint8_t const  font_mask[8] = {0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01};


/*****************************************************************************
** Function name:		writeCommand
**
** Descriptions:		Write a command to the display
**
** Parameters:			data - command to write to the display
*****************************************************************************/
void writeCommand(uint8_t data)
{
    OLED_CMD();
    OLED_CS_ON();

    SSP_Send( (uint8_t *)&data, 1 );

    OLED_CS_OFF();
}

/*****************************************************************************
** Function name:		writeData
**
** Descriptions:		Write data to the display
**
** Parameters:			data - data (color) to write to the display
*****************************************************************************/
static void writeData(uint8_t data)
{
    OLED_DATA();
    OLED_CS_ON();

    SSP_Send( (uint8_t *)&data, 1 );

    OLED_CS_OFF();
}

/*****************************************************************************
** Function name:		writeDataLen
**
** Descriptions:		Write len number of data to the display
**
** Parameters:			data - data (color) to write to the display
** 						len  - number of bytes to write
*****************************************************************************/
static void writeDataLen(unsigned char data, unsigned int len)
{
    uint8_t buf[140];

    for (int i = 0; i < len; i++)
    {
        buf[i] = data;
    }

    OLED_DATA();
    OLED_CS_ON();

    SSP_Send( (uint8_t *)buf, len );

    OLED_CS_OFF();
}

/*****************************************************************************
** Function name:		runInitSequence
**
** Descriptions:		Run display init sequence
**
** Parameters:			None
*****************************************************************************/
static void runInitSequence(void)
{
    /* Recommended Initial code according to manufacturer */

    writeCommand(0x02);/* set low column address */
    writeCommand(0x12);/* set high column address */
    writeCommand(0x40);/* (display start set) */
    writeCommand(0x2e);/* (stop horizontal scroll) */
    writeCommand(0x81);/* (set contrast control register) */
    writeCommand(0x32);/* */
    writeCommand(0x82);/* (brightness for color banks) */
    writeCommand(0x80);/* (display on) */
    writeCommand(0xa1);/* (set segment re-map) */
    writeCommand(0xa6);/* (set normal/inverse display) */
    /* writeCommand(0xa7);*/ /*(set inverse display) */
    writeCommand(0xa8);/* (set multiplex ratio) */
    writeCommand(0x3F);/* */
    writeCommand(0xd3);/* (set display offset) */
    writeCommand(0x40);/* */
    writeCommand(0xad);/* (set dc-dc on/off) */
    writeCommand(0x8E);/* */
    writeCommand(0xc8);/* (set com output scan direction) */
    writeCommand(0xd5);/* (set display clock divide ratio/oscillator/frequency) */
    writeCommand(0xf0);/* */
    writeCommand(0xd8);/* (set area color mode on/off & low power display mode ) */
    writeCommand(0x05);/* */
    writeCommand(0xd9);/* (set pre-charge period) */
    writeCommand(0xF1);/* */
    writeCommand(0xda);/* (set com pins hardware configuration) */
    writeCommand(0x12);/* */
    writeCommand(0xdb);/*(set vcom deselect level) */
    writeCommand(0x34);/* */
    writeCommand(0x91);/*(set look up table for area color) */
    writeCommand(0x3f);/* */
    writeCommand(0x3f);/* */
    writeCommand(0x3f);/* */
    writeCommand(0x3f);/* */
    writeCommand(0xaf);/* (display on) */
    writeCommand(0xa4);/* (display on) */
}

/*****************************************************************************
** Function name:		oled_init
**
** Descriptions:		Initialize the OLED Display
**
** Parameters:			None
*****************************************************************************/
void oled_init (void)
{
    GPIO_SetDir( PORT0, 0, 1 );
    GPIO_SetDir( PORT1, 10, 1);
    GPIO_SetDir( PORT2, 7, 1 );
    GPIO_SetDir( PORT0, 2, 1 );

    /* make sure power is off */
    GPIO_SetValue( PORT1, 10, 0 );

    OLED_CS_OFF();

    runInitSequence();

    memset(shadowFB, 0, SHADOW_FB_SIZE);

    /* small delay before turning on power */
    for (int i = 0; i < 0xffff; i++);
    {

    }
     /* power on */
    GPIO_SetValue( PORT1, 10, 1 );
}

/*****************************************************************************
** Function name:		oled_putPixel
**
** Descriptions:		Draw one pixel on the display
**
** Parameters:			x - x position
** 						y - y position
** 						color - color of the pixel
*****************************************************************************/
void oled_putPixel(uint8_t x, uint8_t y, oled_color_t color)
{
    uint8_t page;
    uint16_t add;
    uint8_t lAddr;
    uint8_t hAddr;
    uint8_t mask;
    uint32_t shadowPos = 0;

    if (x > OLED_DISPLAY_WIDTH)
    {
        return;
    }
    if (y > OLED_DISPLAY_HEIGHT)
    {
        return;
    }

    /* page address */
    if(y < 8)
    {
        page = 0xB0;
    }
    else if(y < 16)
    {
        page = 0xB1;
    }
    else if(y < 24)
    {
        page = 0xB2;
    }
    else if(y < 32)
    {
        page = 0xB3;
    }
    else if(y < 40)
    {
        page = 0xB4;
    }
    else if(y < 48)
    {
        page = 0xB5;
    }
    else if(y < 56)
    {
        page = 0xB6;
    }
    else
    {
        page = 0xB7;
    }

    add = x + X_OFFSET;
    lAddr = 0x0F & add;             /* Low address */
    hAddr = 0x10 | (add >> 4);      /* High address */

    /* Calculate mask from rows basically do a y%8 and remainder is bit position */
    add = y>>3;                     /* Divide by 8 */
    add <<= 3;                      /* Multiply by 8 */
    add = y - add;                  /* Calculate bit position */
    mask = 1 << add;                /* Left shift 1 by bit position */

    setAddress(page, lAddr, hAddr); /* Set the address (sets the page,
                                     * lower and higher column address pointers) */

    shadowPos = (page-0xB0)*OLED_DISPLAY_WIDTH+x;

    if(color > 0)
    {
        shadowFB[shadowPos] |= mask;
    }
    else
    {
        shadowFB[shadowPos] &= ~mask;
    }

    writeData(shadowFB[shadowPos]);
}

/*****************************************************************************
** Function name:		oled_clearScreen
**
** Descriptions:		Clear the entire screen
**
** Parameters:			color to fill the screen with
*****************************************************************************/
void oled_clearScreen(oled_color_t color)
{
    uint8_t i;
    uint8_t c = 0;

    if (color == OLED_COLOR_WHITE)
    {
        c = 0xff;
    }

    for(i=0xB0; i<0xB8; i++)  /* Go through all 8 pages */
    {
        setAddress(i, 0x00, 0x10);
        writeDataLen(c, 132);
    }

    memset(shadowFB, c, SHADOW_FB_SIZE);
}

/*****************************************************************************
** Function name:		oled_putChar
**
** Descriptions:		Draw one character on the display
**						Polish diacritics available (see font5x7 for all available
**						characters)
** Parameters:			coordinate x and y, character, font color and background color
*****************************************************************************/
uint8_t oled_putChar(uint8_t x, uint8_t y, uint8_t ch, oled_color_t fb, oled_color_t bg)
{
    unsigned char data = 0;
    unsigned char i = 0, j = 0;

    oled_color_t color = OLED_COLOR_BLACK;

    if((x >= (OLED_DISPLAY_WIDTH - 8)) || (y >= (OLED_DISPLAY_HEIGHT - 8)) )
    {
        return (0);
    }

    if( (ch < 0x20) || (ch > 0x88) )
    {
        ch = 0x20;      /* unknown character will be set to blank */
    }

    ch -= 0x20;
    for(i=0; i<8; i++)
    {
        data = font5x7[ch][i];
        for(j=0; j<6; j++)
        {
            if( (data&font_mask[j])==0 )
            {
                color = bg;
            }
            else
            {
                color = fb;
            }
            oled_putPixel(x, y, color);
            x++;
        }
        y++;
        x -= 6;
    }
    return(1);
}

/*****************************************************************************
** Function name:		oled_putString
**
** Descriptions:		Put string on the display
**
** Parameters:			coordinate x and y, *string, font color and background color
*****************************************************************************/
void oled_putString(uint8_t x, uint8_t y, uint8_t *pStr, oled_color_t fb, oled_color_t bg)
{
  while(1)
  {
      if( (*pStr)=='\0' )
      {
          break;
      }
      if( oled_putChar(x, y, *pStr++, fb, bg) == 0 )
      {
          break;
      }
    x += 6;
  }
  return;
}
