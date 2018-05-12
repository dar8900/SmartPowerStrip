#include <RTClib.h>
#include "SmartPowerStrip.h"
#include "LCDLib.h"
#include "TimeLib.h"
#include "EEPROM_Ard.h"
#include "Rele.h"
#include "Buttons.h"

RTC_DS1307 RTC;
DateTime now;

TIME_DATE_FORMAT PresentTime;

extern RELE Rele[];

static const short TabDays4Month[] = 
{
  31,
  28,
  31,
  30,
  31,
  30,
  31,
  31,
  30,
  31,
  30,
  31,
};

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
		ClearLCD();
		// following line sets the RTC to the date & time this sketch was compiled
		ChangeTime();
		ClearLCD();
	}	
	return;
}

bool ChangeTime()
{
	TakePresentTime();
	short Hour = PresentTime.hour, Minute = PresentTime.minute, Month = PresentTime.month, Day = PresentTime.day, Year = PresentTime.year, TimeSM = CHANGE_HOUR;
	short ButtonPress = NO_PRESS;
	bool ExitTimeChange = false;
	String MinuteStr;
	ClearLCD();
	while(!ExitTimeChange)
	{
		CheckEvents();
		LCDPrintString(ONE, CENTER_ALIGN, "Cambia orario");
		ButtonPress = CheckButtons();
		switch(TimeSM)
		{
			case CHANGE_HOUR:
				LCDPrintString(TWO, CENTER_ALIGN, "Ora:");
				LCDPrintValue(THREE, CENTER_ALIGN, Hour);
				switch(ButtonPress)
				{
					case BUTTON_UP:	
						if(Hour > 0)
							Hour--;
						else
							Hour = HOUR_IN_DAY;
						ClearLCD();
						break;
					case BUTTON_DOWN:
						if(Hour < HOUR_IN_DAY)
							Hour++;
						else
							Hour = 0;
						ClearLCD();
						break;
					case BUTTON_SET:
						TimeSM = CHANGE_MINUTE;
						ClearLCD();
						break;
					case BUTTON_LEFT:
					default:
						break;
				}
				break;
			case CHANGE_MINUTE:
				LCDPrintString(TWO, CENTER_ALIGN, "Minuti:");
				if(Minute < 10)
				{
					MinuteStr = "0" + String(Minute);
				}
				else
				{
					MinuteStr = String(Minute);
				}
				LCDPrintString(THREE, CENTER_ALIGN, MinuteStr);
				switch(ButtonPress)
				{
					case BUTTON_UP:	
						if(Minute > 0)
							Minute--;
						else
							Minute = MINUTE_IN_HOUR;
						ClearLCD();
						break;
					case BUTTON_DOWN:
						if(Minute < MINUTE_IN_HOUR)
							Minute++;
						else
							Minute = 0;
						ClearLCD();
						break;
					case BUTTON_SET:
						TimeSM = CHANGE_MONTH;
						ClearLCD();
						break;
					case BUTTON_LEFT:
					default:
						break;
				}
				break;
			case CHANGE_MONTH:
				LCDPrintString(TWO, CENTER_ALIGN, "Mese:");
				LCDPrintValue(THREE, CENTER_ALIGN, Month);
				switch(ButtonPress)
				{
					case BUTTON_UP:	
						if(Month > 1)
							Month--;
						else
							Month = 12;
						ClearLCD();
						break;
					case BUTTON_DOWN:
						if(Month < 12)
							Month++;
						else
							Month = 1;
						ClearLCD();
						break;
					case BUTTON_SET:
						TimeSM = CHANGE_DAY;
						ClearLCD();
						break;
					case BUTTON_LEFT:
					default:
						break;
				}
				break;
			case CHANGE_DAY:
				LCDPrintString(TWO, CENTER_ALIGN, "Giorno:");
				LCDPrintValue(THREE, CENTER_ALIGN, Day);
				switch(ButtonPress)
				{
					case BUTTON_UP:	
						if(Day > 1)
							Day--;
						else
							Day = TabDays4Month[Month - 1];
						ClearLCD();
						break;
					case BUTTON_DOWN:
						if(Day < TabDays4Month[Month - 1])
							Day++;
						else
							Day = 1;
						ClearLCD();
						break;
					case BUTTON_SET:
						TimeSM = CHANGE_YEAR;
						ClearLCD();
						break;
					case BUTTON_LEFT:
					default:
						break;
				}
				break;
			case CHANGE_YEAR:
				LCDPrintString(TWO, CENTER_ALIGN, "Anno:");
				LCDPrintValue(THREE, CENTER_ALIGN, Year);
				switch(ButtonPress)
				{
					case BUTTON_UP:	
						if(Year > 2000)
							Year--;
						else
							Year = 2099;
						ClearLCD();
						break;
					case BUTTON_DOWN:
						if(Year < 2099)
							Year++;
						else
							Year = 2000;
						ClearLCD();
						break;
					case BUTTON_SET:
						TimeSM = EXIT_CHANGE_TIME;
						ClearLCD();
						break;
					case BUTTON_LEFT:
					default:
						break;
				}
				break;
			case EXIT_CHANGE_TIME:
				ExitTimeChange = true;
				ClearLCD();
				break;
			default:
				break;
		}
		delay(60);
	}
	TimeAdjust(Hour, Minute, Day, Month, Year);
}

void TimeAdjust(short Hour, short Minute, short Day, short Month, short Year)
{
	DateTime AdjustTime(Year, Month, Day, Hour, Minute, 0 );
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