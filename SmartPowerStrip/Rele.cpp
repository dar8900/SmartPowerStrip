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
	}
}

void CalcTimerRele()
{
	
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
			Rele[ReleIndx].IsActive = false;
		else
			Rele[ReleIndx].IsActive = true;
		if(Rele[ReleIndx].IsActive)
			ON(ReleIndx);
		else
			OFF(ReleIndx);
		delay(500);
	}
	ClearLCD();
	Flag.AllReleDown = false;
	Flag.ReleRS = true;
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
	short ButtonPress;
	bool ExitSetTimer = false;
	int SetTimer = 0;
	String TimerStr;
	ClearLCD();
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
	while(!ExitSetTimer)
	{
		LCDPrintString(0, CENTER_ALIGN, "Imposta il timer");
		LCDPrintString(1, CENTER_ALIGN, "per la presa ");
		LCDPrintValue(2, CENTER_ALIGN, ReleNbr);
		if(SetTimer <= 60)
			TimerStr = String(SetTimer) + "s";
		else if(SetTimer <= 3600 && SetTimer > 60)
			TimerStr = String(SetTimer/60) + "m";
		else if(SetTimer < MAX_TIME_TIMER && SetTimer > 3600)
			TimerStr = String(SetTimer/3600) + "h";
		LCDPrintString(3, CENTER_ALIGN, TimerStr);
		ButtonPress = CheckButtons();
		switch(ButtonPress)
		{
			case BUTTON_UP:
				BlinkLed(BUTTON_LED);
				if(SetTimer > 0)
				{
					if(SetTimer <= 60)
						SetTimer--;
					else if(SetTimer <= 3600 && SetTimer > 60)
						SetTimer -= 60;
					else if(SetTimer < MAX_TIME_TIMER && SetTimer > 3600)
						SetTimer -= 3600;
				}
				else
					SetTimer = MAX_TIME_TIMER;
				break;
			case BUTTON_DOWN:
				BlinkLed(BUTTON_LED);
				if(SetTimer < MAX_TIME_TIMER)
				{
					if(SetTimer <= 60)
						SetTimer++;
					else if(SetTimer <= 3600 && SetTimer > 60)
						SetTimer += 60;
					else if(SetTimer < MAX_TIME_TIMER && SetTimer > 3600)
						SetTimer += 3600;					
				}
				else
					SetTimer = 0;
				break;
			case BUTTON_SET:
				BlinkLed(BUTTON_LED);
				Rele[ReleNbr].TimerTime = SetTimer;
				ExitSetTimer = true;
				break;
			case BUTTON_LEFT:
			default:
				break;			
		}
		
	}
	return ExitSetTimer;
}