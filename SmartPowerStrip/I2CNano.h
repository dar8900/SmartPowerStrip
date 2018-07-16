
#ifndef I2C_NANO_H
#define I2C_NANO_H
#include <Arduino.h>
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
	ENERGY,
	CURRENT
}I2C_WRITE_READ;

short CheckButtons(void);
String EnergyValueStr(void);
String CurrentValueStr(void);
String ReadMeasure(short WichInfo);
void ReadButton(short *ButtonVal);
// void ReadEnergy(String *EnergyStr);
// void ReadCurrent(String *CurrentStr);
#endif