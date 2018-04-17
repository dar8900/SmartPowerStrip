#include <RTClib.h>
#include "SmartPowerStrip.h"
#include "LCDLib.h"
#include "TimeLib.h"
#include "EEPROM_Ard.h"

RTC_DS1307 RTC;
DateTime now;

TIME_DATE_FORMAT PresentTime;

void RTCInit()
{
	if (!RTC.begin()) 
	{
		while (1)
		{
			BlinkLed(LED);
		}
	}

	if (!RTC.isrunning()) 
	{
		LCDDisplayOn();
		ClearLCD();
		LCDPrintString(0, CENTER_ALIGN, "RTC NOT running!");
		delay(1000);
		// following line sets the RTC to the date & time this sketch was compiled
		RTC.adjust(DateTime(F(__DATE__), F(__TIME__)));
		ON(LED);
		ClearLCD();
		LCDPrintString(0, CENTER_ALIGN, "Setting the time");
		delay(2000);
		OFF(LED);
		LCDDisplayOff();	
	}	
	return;
}

void TakePresentTime()
{
  now = RTC.now();
  PresentTime.day = now.day();
  PresentTime.month = now.month();
  PresentTime.year = now.year();
  PresentTime.hour = now.hour();
  PresentTime.minute = now.minute();
}