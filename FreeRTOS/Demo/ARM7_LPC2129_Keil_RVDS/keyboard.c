#include "keyboard.h"
#include <LPC21xx.H>

#define BUT0_BM (1<<4)
#define BUT1_BM (1<<6)
#define BUT2_BM (1<<5)
#define BUT3_BM (1<<7)

enum ButtonState {RELEASED, PRESSED};

enum ButtonState ReadButton1(void)
{
	if ((IO0PIN&BUT0_BM) == 0)
	{
		return PRESSED;
	}
	else
	{
		return RELEASED;
	}
}

void KeyboardInit(void)
{
	IO0DIR &= ~(BUT0_BM | BUT1_BM | BUT2_BM | BUT3_BM);
}

enum KeyboardState eKeyboardRead(void)
{
	if((IO0PIN&BUT0_BM) == 0)
	{
		return BUTTON_0;
	}
	else if((IO0PIN&BUT1_BM) == 0)
	{
		return BUTTON_1;
	}
	else if((IO0PIN&BUT2_BM) == 0)
	{
		return BUTTON_2;
	}
	else if((IO0PIN&BUT3_BM) == 0)
	{
		return BUTTON_3;
	}
	else
	{
		return RELASED;
	}
}
