#include <Wire.h>
#include "ArduinoNanoI2c.h"
#include "Measure.h"



short ButtonPress = NO_PRESS;
short TickSecond = 0;
short WichData = NO_DATA;
// uint32_t TimeExec;

extern String		EnergyStr;
extern String		CurrentStr;

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
	return Press;
}

static void WichInfo()
{
	while(Wire.available())
	{
   		WichData = Wire.read();
	}
}

static void SendInfo()
{
	switch(WichData)
	{
		case BUTTON:
			Wire.write(ButtonPress);
			ButtonPress = NO_PRESS;
			break;
		case ENERGY:
			Wire.write(EnergyStr.c_str());
			break;
		case CURRENT:
			Wire.write(CurrentStr.c_str());
			break;
		default:
			break;
	}
	WichData = NO_DATA;
}

static void BlinkLed(short pin)
{
	ON(pin);
	delay(5);
	OFF(pin);	
}

void setup() 
{
	Serial.begin(9600);
	Wire.begin(ARDUINO_ADDR); 
	pinMode(UP, INPUT);
	pinMode(DOWN, INPUT);
	pinMode(LEFT, INPUT);
	pinMode(SET, INPUT);
	pinMode(BUTTON_LED, OUTPUT);
	Wire.onReceive(WichInfo);
	Wire.onRequest(SendInfo);
}

void loop() 
{
	ChekButtons();
	CalcEnergy();
	TickSecond++;
	if(TickSecond == (1000 / MAIN_DELAY))
	{
		TickSecond = 0;
		EnergyValueSec();	
	}	
	
}
