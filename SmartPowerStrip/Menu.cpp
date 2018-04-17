#include "LCDLib.h"
#include "TimeLib.h"
#include "SmartPowerStrip.h"
#include "EEPROM_Ard.h"
#include "Buttons.h"

extern TIME_DATE_FORMAT PresentTime;
extern RELE Rele[];
extern FLAGS Flag;

MENU_VOICES Menu[]
{
	
};

void SetupInterrupt()
{
	short ButtonPress = NO_PRESS;
	ButtonPress = CheckButtons();
	if(ButtonPress == BUTTON_SET)
	{
		Flag.EnterSetup = true;
	}
	else
	{
		Flag.EnterSetup = false;
	}
	delay(60);
}

void MainScreen()
{
	String Time,Date;
	short ReleIndx = 0;
	
	while(!Flag.EnterSetup)
	{
		SetupInterrupt();
		TakePresentTime();
		ClearLCD();
		// Scrittura data e ora
		if(PresentTime.hour < 10)
		{
			Time = "0" + String(PresentTime.hour);
		}
		else
		{
			Time = String(PresentTime.hour);
		}
		if(PresentTime.minute < 10)
		{
			Time += ":0" + String(PresentTime.minute);
		}
		else
		{
			Time += ":" + String(PresentTime.minute);
		}
		Date = String(PresentTime.day) + "/" + String(PresentTime.month);
		LCDPrintString(0, LEFT_ALIGN, Time);
		LCDPrintString(0, RIGHT_ALIGN, Date);
		// Stato relè
		LCDPrintString(1, LEFT_ALIGN, "Prese:");
		for(ReleIndx = RELE_1; ReleIndx < RELE_MAX; ReleIndx++)
		{
			if(Rele[ReleIndx].IsActive)
			{
				LCDMoveCursor(1, 7 + ReleIndx);
				LCDShowIcon(RELE_ON);
			}
			else
			{
				LCDMoveCursor(1, 7 + ReleIndx);
				LCDShowIcon(RELE_OFF);
			}
		}
	}
}

void MainMenu()
{
	
}

bool ManualRele()
{
	
}

bool ChangeTimeBand()
{
	
}

bool HelpInfo()
{
	
}

bool WiFiInfo()
{
	
}

bool AssignReleTimer()
{
	
}

bool SetTimer()
{
	
}

bool ReleInit()
{
	
}

void TakeReleTime(short Rele)
{
	
}

