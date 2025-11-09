#include "led.h"
#include <LPC21xx.H>

#define LED0_BM (1<<16)
#define LED1_BM (1<<17)
#define LED2_BM (1<<18)
#define LED3_BM (1<<19)

enum Direction {LEFT, RIGHT};

void Led_Init(void)
{
	IO1DIR |= LED0_BM | LED1_BM | LED2_BM | LED3_BM;
	IO1SET |= LED0_BM;
}

void LedOn(unsigned char ucLedIndeks)
{
	IO1CLR |= LED0_BM | LED1_BM | LED2_BM | LED3_BM;
	
	switch (ucLedIndeks)
	{
		case 0:
				IO1SET |= LED0_BM;
				break;
		case 1:
				IO1SET |= LED1_BM;
				break;
		case 2:
				IO1SET |= LED2_BM;
				break;
		case 3:
				IO1SET |= LED3_BM;
				break;
		default:
				break;
	}
}

void LedStep(enum Direction Dir)
{
	static unsigned int steps;
	
	switch (Dir)
	{
		case LEFT:
			steps = (++steps) % 4;
			break;
		case RIGHT:
			steps = (--steps) % 4;
			break;
	}
	
	LedOn(steps);
}

void LedStepLeft(void)
{
	LedStep(LEFT);
}

void LedStepRight(void)
{
	LedStep(RIGHT);
}

void Led_Toggle(unsigned char ucLedIndeks)
{
    unsigned int uiLedMask;

    switch (ucLedIndeks)
    {
        case 0:
            uiLedMask = LED0_BM;
            break;
        case 1:
            uiLedMask = LED1_BM;
            break;
        case 2:
            uiLedMask = LED2_BM;
            break;
        case 3:
            uiLedMask = LED3_BM;
            break;
        default:
            return; 
    }

    if (IO1PIN & uiLedMask)
    {
        IO1CLR = uiLedMask;
    }
    else
    {
        IO1SET = uiLedMask;
    }
}
