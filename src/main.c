#include "type.h"
#include "core_cm3.h"
#include "stdio.h"
#include <stdlib.h>
#include <time.h>

#include "funkcje_gpio.h"
#include "funkcje_SSP.h"
#include "funkcje_7segled.h"
#include "funkcje_diody.h"
#include "funkcje_timer.h"
#include "funkcje_speaker_tone.h"
#include "funkcje_przelaczniki.h"
#include "funkcje_pca9532.h"
#include "funkcje_OLED.h"
#include "funkcje_light.h"
#include "funkcje_uart.h"
#include "funkcje_i2c.h"

volatile uint32_t global_timer = 0;

uint8_t poziom_trudnosci = '5';
uint8_t buf[10];
uint32_t ms = 0;
uint32_t lux = 0;
uint8_t is_night = 0;
uint8_t is_day = 1;
uint16_t macierzdiod = 0;
uint32_t rekord = 0;


static const char *songs[] =
{
   "A5+A5.A5.B5,A5.C5_D5;B/5D/5E/5F/5G/4a/5b/5D/5A/4c/3d/5e/4c/3A/4f/g/4G/4",
};

void oswiec()
{
    light_enable();
    light_setRange(LIGHT_RANGE_4000);
    lux=light_read();
}

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
        	poziom_trudnosci++;
        }
        else
        {
        	poziom_trudnosci--;
        }

        if (poziom_trudnosci > '5')
        	poziom_trudnosci = '1';
        else if (poziom_trudnosci < '1')
        	poziom_trudnosci = '5';

        led7_setChar(poziom_trudnosci);

    }
}

static void poczatek()
{
	oled_clearScreen(OLED_COLOR_WHITE);
    oled_putString(1, 1, (uint8_t*)"Witaj! ", OLED_COLOR_BLACK, OLED_COLOR_WHITE);
    oled_putString(1, 24, (uint8_t*)"Wybierz poziom! ", OLED_COLOR_BLACK, OLED_COLOR_WHITE);

    pca9532_setLeds(0x0, macierzdiod);

    while(GPIO_GetValue(1,4))
    {
        oswiec();
        intToString(lux, buf, 10,10);
        rotate_switch(rotate_read());


        if(lux<400 && is_day==1)
        {
        	writeCommand(0xa7);
        	is_day=0;
        	is_night=1;

        }

        if(lux>=400 && is_night==1)
        {
        	writeCommand(0xa6);
        	is_day=1;
        	is_night=0;
        }
    }
    oled_clearScreen(OLED_COLOR_WHITE);
}

static void ile_punktow(int ch)
{
    oswiec();

    intToString(ch, buf, 10,10);
    oled_putString(1, 1, (uint8_t*)"Ilo#$ punkt&w: ", OLED_COLOR_BLACK, OLED_COLOR_WHITE);
    oled_putString(3, 14, buf, OLED_COLOR_BLACK, OLED_COLOR_WHITE);

    if(lux<400 && is_day==1)
    {
         writeCommand(0xa7);
         is_day=0;
         is_night=1;
    }

    if(lux>=400 && is_night==1)
    {
         writeCommand(0xa6);
         is_day=1;
         is_night=0;
    }
}


static void macierz_diod(uint8_t level)
{
	if (level==1)
	{
		macierzdiod = LED4|LED5;
		pca9532_setLeds(macierzdiod, 0x0);
	}
	else if (level==2)
	{
		macierzdiod |= LED6|LED7;
		pca9532_setLeds(macierzdiod, 0x0);
	}
	else if (level==3)
	{
		macierzdiod |= LED8|LED9;
		pca9532_setLeds(macierzdiod, 0x0);
	}
	else if (level==4)
	{
		macierzdiod |= LED10|LED11;
		pca9532_setLeds(macierzdiod, 0x0);
	}





}

static void algorytm_gry()
{
	uint32_t points = 0;
	uint32_t level = 1;
	ile_punktow(points);

    while(level < 5)
    {


         if (poziom_trudnosci == '1')
         {
            delay32Ms_timer0( (5 + rand() % 10)*1000 );
         }
         else if (poziom_trudnosci == '2')
         {
            delay32Ms_timer0( (4 + rand() % 8)*1000 );
         }
         else if (poziom_trudnosci == '3')
         {
            delay32Ms_timer0( (3 + rand() % 6)*1000 );
         }
         else if (poziom_trudnosci == '4')
         {
            delay32Ms_timer0( (2 + rand() % 4)*1000 );
         }
         else
         {
            delay32Ms_timer0( (1 + rand() % 3)*1000 );
         }


         ms = global_timer;

         while(GPIO_GetValue(0,1))
         {
            rgb_diodaG ();
         }

         uint32_t temppoints = global_timer - ms;

         if ( (temppoints  < 100) )
         {
        	 points+= (100 - (temppoints) );
         }

         ile_punktow(points);

         rgb_diodaB();
         macierz_diod(level);
         level++;

    }
        if(points > rekord || rekord == 0)
        {
           rekord = points;

           for (int i = 0; i < sizeof(songs)/ sizeof(uint8_t*); i++)
           {
                playSong((uint8_t*)songs[i]);
           }

        }
        oled_clearScreen(OLED_COLOR_WHITE);
        while(GPIO_GetValue(1,4))
        {
        	oswiec();

            oled_putString(1, 1, (uint8_t*)"Gra sko%czona ", OLED_COLOR_BLACK, OLED_COLOR_WHITE);
            oled_putString(1, 14, (uint8_t*)"Ilo#$ punkt&w: ", OLED_COLOR_BLACK, OLED_COLOR_WHITE);
            oled_putString(1, 28, buf, OLED_COLOR_BLACK, OLED_COLOR_WHITE);

            if(lux<400 && is_day==1)
            {
                 writeCommand(0xa7);
                 is_day=0;
                 is_night=1;
            }

            if(lux>=400 && is_night==1)
            {
                 writeCommand(0xa6);
                 is_day=1;
                 is_night=0;
            }

        }
        UARTSendString((uint8_t*)buf);
        rgb_diodaB();

}



/*MAIN*/
int main (void)
{


    SysTick_Config(SystemCoreClock / 100);	 /* 10ms */

    GPIO_init();
    buttons_init();
    timer0_32_init(10);
    UARTInit(115200);
    UARTSendString((uint8_t*)"C11 Refleks LPC1343\r\n");
    SSP_init();
    I2CInit((uint32_t)I2CMASTER, 0);
    srand(global_timer);
    rotate_init();
    led7_init();
    rgb_init();
    speaker_init();
    oled_init();

    led7_setChar(poziom_trudnosci);
    while(1)
    {

    	poczatek();
    	UARTSendString((uint8_t*)"Gra rozpoczeta\r\n");
    	algorytm_gry();
        delay32Ms_timer0( 500 );


    }

 return (0);
}
