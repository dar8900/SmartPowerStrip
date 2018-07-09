#include <Wire.h>
#include "ArduinoNanoI2c.h"
#include "Measure.h"



short ButtonPress = NO_PRESS;
short TickSecond;

String ButtonStr;
extern String		EnergyStr;

static bool ChekButtons()
{
	short ButtonUp = LOW, ButtonDown = LOW, ButtonLeft = LOW, Buttonset = LOW, Press = false;
	ButtonUp = digitalRead(UP);
	ButtonDown = digitalRead(DOWN);
	ButtonLeft = digitalRead(LEFT);
	Buttonset = digitalRead(SET);
	if(ButtonUp == HIGH)
	{
		ButtonPress = BUTTON_UP;	
		Press = true;
	}
	else if(ButtonDown == HIGH)
	{
		ButtonPress = BUTTON_DOWN;
		Press = true;
	}
	else if(ButtonLeft == HIGH)
	{
		ButtonPress = BUTTON_LEFT;
		Press = true;
	}
	else if(Buttonset == HIGH)
	{
		ButtonPress = BUTTON_SET;
		Press = true;
	}
	else
	{
		ButtonPress = NO_PRESS;
		Press = false;
	}
	ButtonStr = String(ButtonPress);
	return Press;
}

static void SendInfo()
{
	String InfoStr = ButtonStr + " " + EnergyStr;
	Wire.write(InfoStr.c_str());
	ButtonPress = NO_PRESS;
}

static void BlinkLed(short pin)
{
	ON(pin);
	delay(5);
	OFF(pin);	
}

void setup() 
{
	Wire.begin(ARDUINO_ADDR); 
	pinMode(UP, INPUT);
	pinMode(DOWN, INPUT);
	pinMode(LEFT, INPUT);
	pinMode(SET, INPUT);
	pinMode(BUTTON_LED, OUTPUT);
	Wire.onRequest(SendInfo);
}

void loop() 
{
	ChekButtons();
	CalcEnergy();
	TickSecond++;
	if(TickSecond == SECOND_TICK)
	{
		TickSecond = 0;
		EnergyValueSec();
	}
}
