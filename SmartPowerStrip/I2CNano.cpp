#include <Wire.h>
#include "SmartPowerStrip.h"
#include "I2CNano.h"

#define 	CHAR_FROM_NANO	    9
#define 	FIRST_ENERGY_CHAR	2

// short ButtonVal = NO_PRESS;
// String EnergyStr = "";

short CheckButtons()
{
	short ButtonPress = NO_PRESS;
	ReadButton(&ButtonPress);
	return ButtonPress;
}

String EnergyValueStr()
{
	String EnergyStr;
	ReadEnergy(&EnergyStr);
	return EnergyStr;	
}

void ReadButton(short *ButtonVal)
{
	short ReadInfo;
	Wire.beginTransmission(ARDUINO_ADDR);
	Wire.write(BUTTON);
	Wire.endTransmission();
	delay(1);
	Wire.requestFrom(ARDUINO_ADDR, 1);
	while(Wire.available())
	{
   		ReadInfo = Wire.read();
	}
	switch (ReadInfo)
	{
		case 0:
			*ButtonVal = BUTTON_UP;
			break;
		case 1:
			*ButtonVal = BUTTON_DOWN;
			break;
		case 2:
			*ButtonVal = BUTTON_LEFT;
			break;
		case 3:
			*ButtonVal = BUTTON_SET;
			break;
		case 4:
			*ButtonVal = NO_PRESS;
			break;
		default:
			break;	
	}
}

// void ReadButton(short *ButtonVal)
// {
	// short ReadInfo;
	// Wire.requestFrom(ARDUINO_ADDR, 1);
   	// ReadInfo = Wire.read();
	// switch (ReadInfo)
	// {
		// case '0':
			// *ButtonVal = BUTTON_UP;
			// break;
		// case '1':
			// *ButtonVal = BUTTON_DOWN;
			// break;
		// case '2':
			// *ButtonVal = BUTTON_LEFT;
			// break;
		// case '3':
			// *ButtonVal = BUTTON_SET;
			// break;
		// case '4':
			// *ButtonVal = NO_PRESS;
			// break;
		// default:
			// break;	
	// }
// }

void ReadEnergy(String *EnergyStr)
{
	char ReadInfo[40];
	short TotChar = 0;
	Wire.beginTransmission(ARDUINO_ADDR);
	Wire.write(ENERGY);
	Wire.endTransmission();
	delay(1);
	// short EnergyChars = FIRST_ENERGY_CHAR;
	Wire.requestFrom(ARDUINO_ADDR, CHAR_FROM_NANO);
	while(Wire.available())
	{
   		ReadInfo[TotChar] = Wire.read();
		// if(TotChar >= 2)
		*EnergyStr += String(ReadInfo[TotChar]);
 		TotChar++;
	}
	// while(EnergyChars < TotChar)
	// {
		// *EnergyStr += String(ReadInfo[EnergyChars]);
		// EnergyChars++;
	// }	
}

// void ReadFromNano(short WhatRead)
// {
	// char ReadInfo[100];
	// short TotChar = 0;
	// short EnergyChars = FIRST_ENERGY_CHAR;
	// Wire.requestFrom(ARDUINO_ADDR, CHAR_FROM_NANO);
	// while(Wire.available())
	// {
   		// ReadInfo[TotChar] = Wire.read();
 		// TotChar++;
		// // delayms(5);
	// }
	// if(WhatRead == BUTTON)
	// {
		// switch (ReadInfo[0])
		// {
			// case '0':
				// ButtonVal = BUTTON_UP;
				// break;
			// case '1':
				// ButtonVal = BUTTON_DOWN;
				// break;
			// case '2':
				// ButtonVal = BUTTON_LEFT;
				// break;
			// case '3':
				// ButtonVal = BUTTON_SET;
				// break;
			// case '4':
				// ButtonVal = NO_PRESS;
				// break;
			// default:
				// break;
		// }
	// }
	// else
	// {		
		// while(EnergyChars < TotChar)
		// {
			// EnergyStr += String(ReadInfo[EnergyChars]);
			// EnergyChars++;
		// }
	// }
	// return;
// }
