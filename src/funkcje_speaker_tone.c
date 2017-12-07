#include "mcu_regs.h"
#include "type.h"
#include "funkcje_speaker_tone.h"
#include "funkcje_GPIO.h"
#include "funkcje_timer.h"

#define P1_2_HIGH() (LPC_GPIO1->DATA |= (0x1<<2))
#define P1_2_LOW()  (LPC_GPIO1->DATA &= ~(0x1<<2))

static uint32_t notes[] = {
        2272, // A - 440 Hz
        2024, // B - 494 Hz
        3816, // C - 262 Hz
        3401, // D - 294 Hz
        3030, // E - 330 Hz
        2865, // F - 349 Hz
        2551, // G - 392 Hz
        1136, // a - 880 Hz
        1012, // b - 988 Hz
        1912, // c - 523 Hz
        1703, // d - 587 Hz
        1517, // e - 659 Hz
        1432, // f - 698 Hz
        1275, // g - 784 Hz
};

void speaker_init( void )
{
    GPIO_SetDir( PORT1, 9, 1 ); //-
    GPIO_SetDir( PORT1, 10, 1 ); //-

    GPIO_SetDir( PORT3, 0, 1 );
    GPIO_SetDir( PORT3, 1, 1 );
    GPIO_SetDir( PORT3, 2, 1 );

    GPIO_SetDir( PORT1, 2, 1 );

    LPC_IOCON->JTAG_nTRST_PIO1_2 = (LPC_IOCON->JTAG_nTRST_PIO1_2 & ~0x7) | 0x01;

    GPIO_SetValue( PORT3, 0, 0 );  //LM4811-clk
    GPIO_SetValue( PORT3, 1, 0 );  //LM4811-up/dn
    GPIO_SetValue( PORT3, 2, 0 );  //LM4811-shutdn

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
        return notes[ch - 'A'];
    }

    if (ch >= 'a' && ch <= 'g')
    {
        return notes[ch - 'a' + 7];
    }

    return 0;
}

uint32_t getDuration(uint8_t ch)
{
    if (ch < '0' || ch > '9')
    {
        return 400;
    }

    /* number of ms */

    return (ch - '0') * 200;
}

uint32_t getPause(uint8_t ch)
{
    switch (ch)
    {
    case '+':
        return 0;
    case ',':
        return 5;
    case '.':
        return 20;
    case '_':
        return 30;
    default:
        return 5;
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
        delay32Ms_timer0(pause);
    }
}
