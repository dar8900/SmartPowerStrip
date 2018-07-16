#include <Wire.h>
#include "SmartPowerStrip.h"
#include "I2CNano.h"

#define 	CHAR_FROM_NANO	    9


short CheckButtons()
{
	short ButtonPress = NO_PRESS;
	ReadButton(&ButtonPress);
	return ButtonPress;
}

String EnergyValueStr()
{
	return ReadMeasure(ENERGY);	
}

String CurrentValueStr()
{
	return ReadMeasure(CURRENT);
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

String ReadMeasure(short WichInfo)
{
	String InfoStr;
	char ReadInfo[15];
	short TotChar = 0;
	Wire.beginTransmission(ARDUINO_ADDR);
	Wire.write(WichInfo);
	Wire.endTransmission();
	delay(1);
	Wire.requestFrom(ARDUINO_ADDR, CHAR_FROM_NANO);
	while(Wire.available())
	{
   		ReadInfo[TotChar] = Wire.read();
		InfoStr += String(ReadInfo[TotChar]);
 		TotChar++;
	}	
	return InfoStr;
}

// void ReadEnergy(String *EnergyStr)
// {
	// char ReadInfo[15];
	// short TotChar = 0;
	// Wire.beginTransmission(ARDUINO_ADDR);
	// Wire.write(ENERGY);
	// Wire.endTransmission();
	// delay(1);
	// Wire.requestFrom(ARDUINO_ADDR, CHAR_FROM_NANO);
	// while(Wire.available())
	// {
   		// ReadInfo[TotChar] = Wire.read();
		// *EnergyStr += String(ReadInfo[TotChar]);
 		// TotChar++;
	// }	
// }

// void ReadCurrent(String *CurrentStr)
// {
	// char ReadInfo[10];
	// short TotChar = 0;
	// Wire.beginTransmission(ARDUINO_ADDR);
	// Wire.write(CURRENT);
	// Wire.endTransmission();
	// delay(1);
	// Wire.requestFrom(ARDUINO_ADDR, 5);
	// while(Wire.available())
	// {
   		// ReadInfo[TotChar] = Wire.read();
		// *CurrentStr += String(ReadInfo[TotChar]);
 		// TotChar++;
	// }	
// }