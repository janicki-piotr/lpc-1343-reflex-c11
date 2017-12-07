#include "type.h"
#include "core_cm3.h"
#include "stdio.h"

#include "funkcje_GPIO.h"
#include "funkcje_SSP.h"
#include "funkcje_7segled.h"
#include "funkcje_ratatuj.h"
#include "funkcje_diody.h"
#include "funkcje_timer.h"
#include "funkcje_speaker_tone.h"
#include "funkcje_OLED.h"
#include "funkcje_light.h"
#include "funkcje_uart.h"


static   uint8_t ch = '0';
static   uint8_t points = '1';
volatile uint32_t global_timer = 0;
uint32_t ms;
uint32_t a;
uint32_t b;

static const char *songs[] =
{
   "e1,e1[e1[c1.e1[g1_G1<",
};

void SysTick_Handler(void)
{
	global_timer++;
}

static void rotate_switch(uint8_t rotateDir)
{
    if (rotateDir != ROTATE_WAIT)
    {

        if (rotateDir == ROTATE_RIGHT)
        {
            ch++;
        }
        else
        {
            ch--;
        }

        if (ch > '5')
            ch = '1';
        else if (ch < '1')
            ch = '5';

        led7_setChar(ch);

    }
}

static void koniec(int ch)
{
	 oled_putString(1, 1, (uint8_t*)"Gra sko%czona ", OLED_COLOR_BLACK, OLED_COLOR_WHITE);
	 oled_putString(1, 13, (uint8_t*)"Ilo#$ zdobytych pkt: ", OLED_COLOR_BLACK, OLED_COLOR_WHITE);
	 oled_putString(1, 25, (uint8_t*)"pkt: ", OLED_COLOR_BLACK, OLED_COLOR_WHITE);
	 oled_putChar(27, 25, ch, OLED_COLOR_BLACK, OLED_COLOR_WHITE);
}

static void przycisk_i_dioda(int ch)
{
	if ((a!=10) & (b!=10))
	{

		ms = global_timer;
		while (global_timer - ms < a);
		rgb_diodaB ();

		ms = global_timer;
		while (global_timer - ms < b);
		rgb_diodaG ();

	}

	else
	{
		rgb_diodaR ();
	    timer0_32_init(10);

	    for (int i = 0; i < sizeof(songs); i++)
	    {
	        playSong((uint8_t*)songs[i]);
	        rgb_diodaG ();
	        koniec(ch);

	    }

	}
}

static int punkty(int ch)
{
	oled_putChar(1, 14, ch, OLED_COLOR_BLACK, OLED_COLOR_WHITE);
	return 0;
}


/*MAIN*/
int main (void) {


    SysTick_Config(SystemCoreClock / 100);	 // 10ms

    GPIO_init();
    UART_init(115200);
    UARTSendString((uint8_t*)"C11 Refleks LPC1343\r\n");
    SSP_init();

    rotate_init();
    led7_init();
    rgb_init();
    speaker_init();
    oled_init();

    oled_clearScreen(OLED_COLOR_WHITE);

    while(LPC_GPIO0->DATA & (1<<1))
    {
    	rotate_switch(rotate_read());
    	oled_putString(1, 1, (uint8_t*)"Witaj! ", OLED_COLOR_BLACK, OLED_COLOR_WHITE);
    	oled_putString(1, 12, (uint8_t*)"Wybierz poziom! ", OLED_COLOR_BLACK, OLED_COLOR_WHITE);
    }

    a = 70 - (ch - (48 + 1) ) * 10;
    b = 60 - (ch - (48 + 1) ) * 10;

    oled_clearScreen(OLED_COLOR_WHITE);

    oled_putString(1, 1, (uint8_t*)"Ilo#$ punkt&w: ", OLED_COLOR_BLACK, OLED_COLOR_WHITE);

    while(1)
    {
    	przycisk_i_dioda(points);
    	if (!(LPC_GPIO0->DATA & (1<<1)) & (a > 10) & (b > 10) & (GPIO_GetValue( PORT1, 10)) )
    		{
    			punkty(points);
    			a-=10;
    			b-=10;
    			ch++;
    			points++;
    			led7_setChar(ch);
    		}
    }
}
