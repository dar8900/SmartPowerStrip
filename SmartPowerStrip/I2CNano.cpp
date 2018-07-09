#include <Wire.h>
#include "SmartPowerStrip.h"
#include "I2CNano.h"

#define 	CHAR_FROM_NANO	   11
#define 	FIRST_ENERGY_CHAR	2

short ButtonVal = NO_PRESS;
String EnergyStr = "";

short CheckButtons()
{
	short ButtonPress = NO_PRESS;
	ReadFromNano(BUTTON);
	ButtonPress = ButtonVal;
	ButtonVal = NO_PRESS;
	delayms(40);
	return ButtonPress;
}

String EnergyValueStr()
{
	String TempEnergyStr;
	ReadFromNano(ENERGY);
	TempEnergyStr = EnergyStr;
	EnergyStr = "";
	return TempEnergyStr;	
}

void ReadFromNano(short WhatRead)
{
	char ReadInfo[100];
	short TotChar = 0;
	short EnergyChars = FIRST_ENERGY_CHAR;
	Wire.requestFrom(ARDUINO_ADDR, CHAR_FROM_NANO);
	while(Wire.available())
	{
   		ReadInfo[TotChar] = Wire.read();
 		TotChar++;
		// delayms(5);
	}
	if(WhatRead == BUTTON)
	{
		switch (ReadInfo[0])
		{
			case '0':
				ButtonVal = BUTTON_UP;
				break;
			case '1':
				ButtonVal = BUTTON_DOWN;
				break;
			case '2':
				ButtonVal = BUTTON_LEFT;
				break;
			case '3':
				ButtonVal = BUTTON_SET;
				break;
			case '4':
				ButtonVal = NO_PRESS;
				break;
			default:
				break;
		}
	}
	else
	{		
		while(EnergyChars < TotChar)
		{
			EnergyStr += String(ReadInfo[EnergyChars]);
			EnergyChars++;
		}
	}
	return;
}
