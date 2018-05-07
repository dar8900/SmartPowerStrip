#include "LCDLib.h"
#include "TimeLib.h"
#include "SmartPowerStrip.h"
#include "EEPROM_Ard.h"
#include "Buttons.h"
#include "Menu.h"
#include "Web.h"
#include "Band.h"
#include "Rele.h"

extern FLAGS Flag;
extern TIME_DATE_FORMAT PresentTime;


RELE Rele[]
{
	{false, {0,0,0,0}, {0,0,0,0}, false, {0,0,0,0}, RELE_1_ADDR},  // RELE_1
	{false, {0,0,0,0}, {0,0,0,0}, false, {0,0,0,0}, RELE_2_ADDR},  // RELE_2
	{false, {0,0,0,0}, {0,0,0,0}, false, {0,0,0,0}, RELE_3_ADDR},  // RELE_3
	{false, {0,0,0,0}, {0,0,0,0}, false, {0,0,0,0}, RELE_4_ADDR},  // RELE_4
	{false, {0,0,0,0}, {0,0,0,0}, false, {0,0,0,0}, RELE_5_ADDR},  // RELE_5
	{false, {0,0,0,0}, {0,0,0,0}, false, {0,0,0,0}, RELE_6_ADDR},  // RELE_6
	{false, {0,0,0,0}, {0,0,0,0}, false, {0,0,0,0}, RELE_7_ADDR},  // RELE_7
	{false, {0,0,0,0}, {0,0,0,0}, false, {0,0,0,0}, RELE_8_ADDR},  // RELE_8
};

void TurnOffAllRele()
{
	short ReleIndx;
	for(ReleIndx = RELE_1; ReleIndx < RELE_MAX; ReleIndx++)
	{
		OFF(ReleIdx2Pin(ReleIndx));
		Rele[ReleIndx].IsActive = false;
		delay(500);
		Rele[ReleIndx].TurnOnTime = SetTimeVarRele(0,0,0,0);
		Rele[ReleIndx].ActiveTime = SetTimeVarRele(0,0,0,0);
	}
	Flag.AllReleDown = true;
	Flag.AllReleUp = false;
}

void TurnOnAllRele()
{
	short ReleIndx;
	CheckEvents();
	for(ReleIndx = RELE_1; ReleIndx < RELE_MAX; ReleIndx++)
	{
		ON(ReleIdx2Pin(ReleIndx));
		Rele[ReleIndx].IsActive = true;
		Rele[ReleIndx].TurnOnTime = SetTimeVarRele(PresentTime.hour,PresentTime.minute,PresentTime.second,PresentTime.day);
		delay(500);
	}
	Flag.AllReleDown = false;
	Flag.AllReleUp = true;
}

void TakeReleTime()
{
	short ReleIndx;
	if(!Flag.AllReleDown)
	{
		for(ReleIndx = RELE_1; ReleIndx < RELE_MAX; ReleIndx++)
		{
			TakePresentTime();
			if(Rele[ReleIndx].IsActive)
			{
				Rele[ReleIndx].ActiveTime.day = PresentTime.day - Rele[ReleIndx].TurnOnTime.day;
				Rele[ReleIndx].ActiveTime.hour = PresentTime.hour - Rele[ReleIndx].TurnOnTime.hour;
				Rele[ReleIndx].ActiveTime.minute = PresentTime.minute - Rele[ReleIndx].TurnOnTime.minute;
				Rele[ReleIndx].ActiveTime.second = PresentTime.second - Rele[ReleIndx].TurnOnTime.second;
			}
			if(Rele[ReleIndx].HaveTimer)
			{
				if(Rele[ReleIndx].TimerTime.hour == PresentTime.hour && Rele[ReleIndx].TimerTime.minute == PresentTime.minute)
				{
					Rele[ReleIndx].TimerTime = SetTimeVarRele(0,0,0,0);
					Rele[ReleIndx].HaveTimer = false;
					Rele[ReleIndx].IsActive = false;
					OFF(ReleIdx2Pin(ReleIndx));
					String ReleName;
					ReleName = "Presa " + String(ReleIndx+1) + "spenta";
					LCDShowPopUp(ReleName);
				}
			}
		}
	}
}


bool ReleInit(bool FirstGo)
{
	short ReleIndx;
	ClearLCD();
	LCDPrintString(ONE, CENTER_ALIGN, "Inizializzo i rele");
	LCDPrintString(TWO, CENTER_ALIGN, "attendere...");
	TurnOffAllRele();
	ShowReleIcons(THREE);
	delay(1000);
	Flag.AllReleDown = false;
	for(ReleIndx = RELE_1; ReleIndx < RELE_MAX; ReleIndx++)
	{
		ON(ReleIdx2Pin(ReleIndx));
		Rele[ReleIndx].IsActive = true;
		ShowReleIcons(THREE);
		delay(500);
		OFF(ReleIdx2Pin(ReleIndx));
		Rele[ReleIndx].IsActive = false;
		ShowReleIcons(THREE);
		delay(500);
	}
	Flag.AllReleDown = true;
	TakePresentTime();
	if(!FirstGo)
	{
		ReleReStart();
	}
	CheckReleStatus();
}

void ReleReStart()
{
	short ReleIndx, TmpReleActive;
	ClearLCD();
	for(ReleIndx = RELE_1; ReleIndx < RELE_MAX; ReleIndx++)
	{	
		LCDPrintString(TWO, CENTER_ALIGN, "Sto ristabilendo");
		LCDPrintString(THREE, CENTER_ALIGN, "i valori dei rele");
		ReadMemory(Rele[ReleIndx].EepromAddr, 1, &TmpReleActive);
		if(TmpReleActive == 0)
		{
			Rele[ReleIndx].IsActive = false;
			OFF(ReleIdx2Pin(ReleIndx));
		}
		else
		{
			Rele[ReleIndx].IsActive = true;
			ON(ReleIdx2Pin(ReleIndx));
			Flag.AllReleDown = false;
			Rele[ReleIndx].TurnOnTime = SetTimeVarRele(PresentTime.hour,PresentTime.minute,PresentTime.second,PresentTime.day);
			CheckEvents();
		}

		delay(500);
	}
	ClearLCD();
	Flag.ReleRS = true;
	CheckEvents();
}

void CheckReleStatus()
{
	short ReleIndx;
	for(ReleIndx = RELE_1; ReleIndx < RELE_MAX; ReleIndx++)
	{
		if(Rele[ReleIndx].IsActive)
		{
				Flag.AllReleDown = false;
				break;
		}
		else
			Flag.AllReleDown = true;
	}
	
}

void ShowReleIcons(short Row)
{
	short ReleIndx;
	if(Flag.AllReleDown)
	{
		for(ReleIndx = RELE_1; ReleIndx < RELE_MAX; ReleIndx++)
		{
			LCDMoveCursor(Row, 6 + ReleIndx);
			LCDShowIcon(RELE_OFF);			
		}
	}
	else
	{
		for(ReleIndx = RELE_1; ReleIndx < RELE_MAX; ReleIndx++)
		{
			if(Rele[ReleIndx].IsActive)
			{
				LCDMoveCursor(Row, 6 + ReleIndx);
				LCDShowIcon(RELE_ON);
			}
			else
			{
				LCDMoveCursor(Row, 6 + ReleIndx);
				LCDShowIcon(RELE_OFF);
			}
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
	CheckEvents();
	LCDPrintString(ONE, CENTER_ALIGN, "Imposta il timer");
	LCDPrintString(TWO, CENTER_ALIGN, "per la presa ");
	LCDPrintValue(THREE, CENTER_ALIGN, ReleNbr);
	delay(2000);
	ClearLCD();
	LCDPrintString(ONE  , CENTER_ALIGN, "Il tempo massimo");
	LCDPrintString(TWO  , CENTER_ALIGN, "è di 4h");
	LCDPrintString(THREE, CENTER_ALIGN, "Premere Ok/Set");
	LCDPrintString(FOUR , CENTER_ALIGN, "per confermare");
	delay(3000);
	ClearLCD();
	CheckEvents();
	while(!ExitSetTimer)
	{
		
		LCDPrintString(ONE, CENTER_ALIGN, "Imposta il timer");
		LCDPrintString(TWO, CENTER_ALIGN, "per la presa ");
		LCDPrintValue(THREE, CENTER_ALIGN, ReleNbr);
		LCDPrintValue(FOUR, 8, Hour);
		LCDPrintString(FOUR, 9, ":");
		LCDPrintValue(FOUR, 10, Minute_1);
		LCDPrintValue(FOUR, 11, Minute_2);
		CheckEvents();
		switch(Cursor)
		{
			case 0:
				LCDMoveCursor(FOUR, 8);
				LCDBlink();
				break;
			case 1:
				LCDMoveCursor(FOUR, 10);
				LCDBlink();
				break;
			case 2:
				LCDMoveCursor(FOUR, 11);
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
				if(SetTimer.hour == 0 && SetTimer.minute == 0)
				{
					ClearLCD();
					LCDPrintString(ONE, CENTER_ALIGN, "Timer non corretto");
					LCDPrintString(ONE, CENTER_ALIGN, "Re-Inserire i valori");
					delay(2000);
					ClearLCD();
					ExitSetTimer = false;
				}
				else if((PresentTime.hour + SetTimer.hour >= 24) || (PresentTime.minute + SetTimer.minute > 59))
				{
					ClearLCD();
					LCDPrintString(ONE, CENTER_ALIGN, "Timer non corretto");
					LCDPrintString(ONE, CENTER_ALIGN, "Re-Inserire i valori");
					delay(2000);
					ClearLCD();
					ExitSetTimer = false;					
				}
				else
				{
					Rele[ReleNbr].TimerTime.hour = PresentTime.hour + SetTimer.hour;
					Rele[ReleNbr].TimerTime.minute = PresentTime.minute + SetTimer.minute;
					ExitSetTimer = true;
				}
				break;
			default:
				break;			
		}
		delay(70);
		ButtonPress = NO_PRESS;
	}
	return ExitSetTimer;
}


RELE_TIME_FORMAT SetTimeVarRele(short Hour, short Minute, short Second, short Day)
{
	RELE_TIME_FORMAT TempVar;
	TempVar.hour = Hour;
	TempVar.hour = Minute;
	TempVar.hour = Second;
	TempVar.hour = Day;
	return TempVar;
}



short ReleIdx2Pin(short ReleIndx)
{
	short PinNum = 0;
	switch(ReleIndx)
	{
		case RELE_1:
			PinNum = RELE1; 
			break;
		case RELE_2:
			PinNum = RELE2;
			break;
		case RELE_3:
			PinNum = RELE3;
			break;
		case RELE_4:
			PinNum = RELE4;
			break;
		case RELE_5:
			PinNum = RELE5;
			break;
		case RELE_6:
			PinNum = RELE6;
			break;
		case RELE_7:
			PinNum = RELE7;
			break;
		case RELE_8: 
			PinNum = RELE8;
			break;
		default:
			PinNum = RELE1;
			break;
	}
	return PinNum;	
}