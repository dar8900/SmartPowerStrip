#include "SmartPowerStrip.h"
#include <Wire.h>
#include "Buttons.h"

short CheckButtons()
{
	short ButtonVal = NO_PRESS;
	short ValueRead = 0;
	Wire.requestFrom(ARDUINO_ADDR, 1);
	ValueRead = Wire.read();
	switch(ValueRead)
	{
		case BUTTON_UP:
			ButtonVal = BUTTON_UP;
			break;
		case BUTTON_DOWN:
			ButtonVal = BUTTON_DOWN;
			break;
		case BUTTON_LEFT:
			ButtonVal = BUTTON_LEFT;
			break;
		case BUTTON_SET:
			ButtonVal = BUTTON_SET;
			break;
		case NO_PRESS:
		default:
			ButtonVal = NO_PRESS;
			break;	
	}
	return ButtonVal;
}