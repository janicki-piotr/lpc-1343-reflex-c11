#include <funkcje_gpio.h>
#include "mcu_regs.h"
#include "type.h"
#include "funkcje_speaker_tone.h"
#include "funkcje_timer.h"

#define P1_2_HIGH() (LPC_GPIO1->DATA |= ( 0x1 << 2 ))
#define P1_2_LOW()  (LPC_GPIO1->DATA &= ~( 0x1 << 2 ))

static uint32_t notes[] = {
        1515, /* A - 660 Hz */
        1961, /* B - 510 Hz */
        1299, /* C - 770 Hz */
        2632, /* D - 380 Hz */
        3125, /* E - 320 Hz */
        2273, /* F - 440 Hz */
        2083, /* G - 480 Hz */
        2222, /* a - 450 Hz */
        2326, /* b - 430 Hz */
        1327, /* c - 760 Hz */
        1163, /* d - 860 Hz */
        1429, /* e - 700 Hz */
        1923, /* f - 520 Hz */
        1724, /* g - 580 Hz */
};

void speaker_init( void )
{


    GPIO_SetDir( PORT3, 0, 1 );
    GPIO_SetDir( PORT3, 1, 1 );
    GPIO_SetDir( PORT3, 2, 1 );

    GPIO_SetDir( PORT1, 2, 1 );

    LPC_IOCON->JTAG_nTRST_PIO1_2 = (LPC_IOCON->JTAG_nTRST_PIO1_2 & ~0x7) | 0x01;

    GPIO_SetValue( PORT3, 0, 0 );  /* LM4811-clk */
    GPIO_SetValue( PORT3, 1, 0 );  /* LM4811-up/dn */
    GPIO_SetValue( PORT3, 2, 0 );  /* LM4811-shutdn */

  return;
}

void playNote(uint32_t note, uint32_t durationMs)
{
    uint32_t t = 0;

    if (note > 0)
    {

        while (t < (durationMs * 1000))
        {
            P1_2_HIGH();
            delay32Us_timer0(note / 2);

            P1_2_LOW();
            delay32Us_timer0(note / 2);

            t += note;
        }

    }
    else {
    	delay32Ms_timer0(durationMs);
    }
}

uint32_t getNote(uint8_t ch)
{
    if (ch >= 'A' && ch <= 'G')
    {
        return (notes[ch - 'A']);
    }

    if (ch >= 'a' && ch <= 'g')
    {
        return (notes[ch - 'a' + 7]);
    }

    return (0);
}

uint32_t getDuration(uint8_t ch)
{
    if (ch < '0' || ch > '9')
    {
        return (400);
    }

    /* number of ms */

    return ((ch - '0') * 20);
}

uint32_t getPause(uint8_t ch)
{
    switch (ch)
    {
    case '+':
        return (150);
    case ',':
        return (100);
    case '.':
        return (300);
    case '_':
        return (550);
    case ';':
    	return (575);
    case '/':
    	return (0);
    default:
        return (0);
    }
}

void playSong(uint8_t *song)
{
    uint32_t note = 0;
    uint32_t dur  = 0;
    uint32_t pause = 0;

    /*
     * A song is a collection of tones where each tone is
     * a note, duration and pause, e.g.
     *
     * "E2,F4,"
     */

    while(*song != '\0')
    {
        note = getNote(*song++);
        if (*song == '\0')
        {
            break;
        }

        dur  = getDuration(*song++);
        if (*song == '\0')
        {
            break;
        }

        pause = getPause(*song++);

        playNote(note, dur);
        if (pause!=0)
           {
               delay32Ms_timer0(pause);
           }
    }
}

