#include <RTClib.h>
#include "SmartPowerStrip.h"
#include "LCDLib.h"
#include "TimeLib.h"
#include "EEPROM_Ard.h"
#include "Rele.h"

RTC_DS1307 RTC;
DateTime now;

TIME_DATE_FORMAT PresentTime;

extern RELE Rele[];

void RTCInit()
{
	if (!RTC.begin()) 
	{
		while (1)
		{
			BlinkLed(BUTTON_LED);
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
		ON(BUTTON_LED);
		ClearLCD();
		LCDPrintString(0, CENTER_ALIGN, "Setting the time");
		delay(2000);
		OFF(BUTTON_LED);
		LCDDisplayOff();	
	}	
	return;
}

void TimeAdjust(short Hour, short Minute)
{
	DateTime AdjustTime(PresentTime.year, PresentTime.month, PresentTime.day, Hour, Minute, 0 );
	RTC.adjust(AdjustTime);
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
  PresentTime.second = now.second();
}