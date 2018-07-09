// #include <Arduino.h>

#define ARDUINO_ADDR	0x08


typedef enum
{
	BUTTON_UP = 0,
	BUTTON_DOWN,
	BUTTON_LEFT,
	BUTTON_SET,
	NO_PRESS
}BUTTONS;

typedef enum
{
	BUTTON = 0,
	ENERGY
}READ_ITEM;

short CheckButtons(void);
void ReadFromNano(short WhatRead);
