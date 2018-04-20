#include "LCDLib.h"
#include "TimeLib.h"
#include "SmartPowerStrip.h"
#include "EEPROM_Ard.h"
#include "Buttons.h"
#include "Menu.h"
#include "Web.h"
#include "Band.h"
#include "Rele.h"

extern RELE Rele[];
extern FLAGS Flag;
extern TIME_DATE_FORMAT PresentTime;



void TurnOffAllRele()
{
	short ReleIndx;
	for(ReleIndx = RELE_1; ReleIndx < RELE_MAX; ReleIndx++)
	{
		OFF(ReleIndx);
		Rele[ReleIndx].IsActive = false;
		delay(500);
	}
	Flag.AllReleDown = true;
}

void TurnOnAllRele()
{
	short ReleIndx;
	for(ReleIndx = RELE_1; ReleIndx < RELE_MAX; ReleIndx++)
	{
		ON(ReleIndx);
		Rele[ReleIndx].IsActive = true;
		delay(500);
	}
}

void TakeReleTime()
{
	short ReleIndx;
	for(ReleIndx = RELE_1; ReleIndx < RELE_MAX; ReleIndx++)
	{
		TakePresentTime();
		if(Rele[ReleIndx].IsActive)
		{
			Rele[ReleIndx].ActiveTime = (PresentTime.hour * SEC_IN_HOUR) + (PresentTime.minute * SEC_IN_MINUTE) + PresentTime.second;
			if(Rele[ReleIndx].ActiveTime == SEC_IN_31_GG)
				Rele[ReleIndx].ActiveTime = 0;
		}
		if(Rele[ReleIndx].HaveTimer)
		{
			Rele[ReleIndx].TimerTime -= ((PresentTime.hour * SEC_IN_HOUR) + (PresentTime.minute * SEC_IN_MINUTE));
			if(Rele[ReleIndx].TimerTime <= 0)
			{
				Rele[ReleIndx].HaveTimer = false;
				Rele[ReleIndx].IsActive = false;
				OFF(ReleIndx);
			}
		}
	}
}


bool ReleInit()
{
	short ReleIndx;
	ClearLCD();
	LCDPrintString(0, CENTER_ALIGN, "Inizializzo i rele");
	LCDPrintString(1, CENTER_ALIGN, "attendere...");
	TurnOffAllRele();
	ShowReleIcons();
	delay(1000);
	for(ReleIndx = RELE_1; ReleIndx < RELE_MAX; ReleIndx++)
	{
		ON(ReleIndx);
		Rele[ReleIndx].IsActive = true;
		ShowReleIcons();
		delay(1000);
		OFF(ReleIndx);
		Rele[ReleIndx].IsActive = false;
		ShowReleIcons();
		delay(1000);
	}
	TakePresentTime();
	ReleReStart();
}

void ReleReStart()
{
	short ReleIndx, TmpReleActive;
	ClearLCD();
	for(ReleIndx = RELE_1; ReleIndx < RELE_MAX; ReleIndx++)
	{	
		LCDPrintString(1, CENTER_ALIGN, "Sto ristabilendo");
		LCDPrintString(2, CENTER_ALIGN, "i valori dei rele");
		ReadMemory(Rele[ReleIndx].EepromAddr, 1, &TmpReleActive);
		if(TmpReleActive == 0)
		{
			Rele[ReleIndx].IsActive = false;
			OFF(ReleIndx);
		}
		else
		{
			Rele[ReleIndx].IsActive = true;
			ON(ReleIndx);
			TakeReleTime();
		}

		delay(500);
	}
	ClearLCD();
	Flag.AllReleDown = false;
	Flag.ReleRS = true;
	TakeReleTime();
}


void ShowReleIcons()
{
	short ReleIndx;
	for(ReleIndx = RELE_1; ReleIndx < RELE_MAX; ReleIndx++)
	{
		if(Rele[ReleIndx].IsActive)
		{
			LCDMoveCursor(2, 6 + ReleIndx);
			LCDShowIcon(RELE_ON);
		}
		else
		{
			LCDMoveCursor(2, 6 + ReleIndx);
			LCDShowIcon(RELE_OFF);
		}
	}
}



bool SetTimerRele(short ReleNbr)
{
	short ButtonPress, Cursor = 0;
	bool ExitSetTimer = false;
	TIME_DATE_FORMAT SetTimer;
	short Hour = 0, Minute_1 = 0, Minute_2 = 0;
	String TimerStr;
	ClearLCD();
	TakeReleTime();
	LCDPrintString(0, CENTER_ALIGN, "Imposta il timer");
	LCDPrintString(1, CENTER_ALIGN, "per la presa ");
	LCDPrintValue(2, CENTER_ALIGN, ReleNbr);
	delay(2000);
	ClearLCD();
	LCDPrintString(0, CENTER_ALIGN, "Il tempo massimo");
	LCDPrintString(1, CENTER_ALIGN, "è di 4h");
	LCDPrintString(2, CENTER_ALIGN, "Premere Ok/Set");
	LCDPrintString(3, CENTER_ALIGN, "per confermare");
	delay(3000);
	ClearLCD();
	TakeReleTime();
	while(!ExitSetTimer)
	{
		TakePresentTime();
		LCDPrintString(0, CENTER_ALIGN, "Imposta il timer");
		LCDPrintString(1, CENTER_ALIGN, "per la presa ");
		LCDPrintValue(2, CENTER_ALIGN, ReleNbr);
		LCDPrintValue(3, 8, Hour);
		LCDPrintString(3, 9, ":");
		LCDPrintValue(3, 10, Minute_1);
		LCDPrintValue(3, 11, Minute_2);
		TakeReleTime();
		switch(Cursor)
		{
			case 0:
				LCDMoveCursor(3, 8);
				LCDBlink();
				break;
			case 1:
				LCDMoveCursor(3, 10);
				LCDBlink();
				break;
			case 2:
				LCDMoveCursor(3, 11);
				LCDBlink();
				break;
			default:
				break;
		}	
		ButtonPress = CheckButtons();
		switch(ButtonPress)
		{
			case BUTTON_UP:
				BlinkLed(BUTTON_LED);
				switch(Cursor)
				{
					case 0:
						if(Hour > 0)
							Hour--;
						else
							Hour = 3;
						break;
					case 1:
						if(Minute_1 > 0)
							Minute_1--;
						else
							Minute_1 = 5;
						break;
					case 2:
						if(Minute_2 > 1)
							Minute_2--;
						else
							Minute_2 = 9;
						break;
					default:
						break;
				}
				break;
			case BUTTON_DOWN:
				BlinkLed(BUTTON_LED);
				switch(Cursor)
				{
					case 0:
						if(Hour < 3)
							Hour++;
						else
							Hour = 0;
						break;
					case 1:
						if(Minute_1 < 5)
							Minute_1++;
						else
							Minute_1 = 0;
						break;
					case 2:
						if(Minute_2 < 9)
							Minute_2++;
						else
							Minute_2 = 1;
						break;
					default:
						break;
				}
				break;
			case BUTTON_LEFT:
				BlinkLed(BUTTON_LED);
				if(Cursor < 3)
					Cursor++;
				else
					Cursor = 0;
				break;
			case BUTTON_SET:
				BlinkLed(BUTTON_LED);
				SetTimer.hour = Hour;
				SetTimer.minute = (Minute_1*10) + Minute_2;
				Rele[ReleNbr].TimerTime = ((SetTimer.hour + PresentTime.hour) * SEC_IN_HOUR) + ((SetTimer.minute + PresentTime.minute) * SEC_IN_MINUTE);
				ExitSetTimer = true;
				break;
			default:
				break;			
		}
		
	}
	return ExitSetTimer;
}