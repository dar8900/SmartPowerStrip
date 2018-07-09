#include "LCDLib.h"
#include "TimeLib.h"
#include "SmartPowerStrip.h"
#include "EEPROM_Ard.h"
#include "I2CNano.h"
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

static short TmpMinute[RELE_MAX];
static bool IsNewMinute[RELE_MAX];

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
		SaveReleStatus(ReleIndx, STATUS_OFF);
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
		Rele[ReleIndx].TurnOnTime.day = PresentTime.day;
		Rele[ReleIndx].TurnOnTime.hour = PresentTime.hour;
		Rele[ReleIndx].TurnOnTime.minute = PresentTime.minute;
		Rele[ReleIndx].ActiveTime.minute = 0;
		SaveReleStatus(ReleIndx, STATUS_ON);
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
				if((PresentTime.minute - Rele[ReleIndx].TurnOnTime.minute) <= 0)
				{
					if((PresentTime.minute - Rele[ReleIndx].TurnOnTime.minute) == 0)
					{
						TmpMinute[ReleIndx] = - Rele[ReleIndx].TurnOnTime.minute;
					}
					Rele[ReleIndx].ActiveTime.minute = (Rele[ReleIndx].TurnOnTime.minute - (Rele[ReleIndx].TurnOnTime.minute  - PresentTime.minute)) + TmpMinute[ReleIndx];
					if(Rele[ReleIndx].ActiveTime.minute == 59)
					{
						if(!IsNewMinute[ReleIndx])
						{
							Rele[ReleIndx].ActiveTime.hour += 1;
							IsNewMinute[ReleIndx] = true;
						}
					}
				}
				else
				{
					Rele[ReleIndx].ActiveTime.minute = PresentTime.minute - Rele[ReleIndx].TurnOnTime.minute;
					TmpMinute[ReleIndx] = PresentTime.minute - Rele[ReleIndx].TurnOnTime.minute;
					IsNewMinute[ReleIndx] = false;
				}
				if(Rele[ReleIndx].ActiveTime.hour > 23)
				{
					Rele[ReleIndx].ActiveTime.day += 1;
					Rele[ReleIndx].ActiveTime.hour = 0;
				}
				if(Rele[ReleIndx].ActiveTime.day > 31)
				{
					Rele[ReleIndx].ActiveTime = SetTimeVarRele(0,0,0,0);
				}
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
					ReleName = "Presa " + String(ReleIndx+1) + " spenta";
					LCDShowPopUp(ReleName);
					SaveReleStatus(ReleIndx, STATUS_OFF);
				}
			}
		}
	}
}

void SaveReleStatus(short ReleIndx, short Status)
{
	WriteMemory(Rele[ReleIndx].EepromAddr, Status);
}

bool ReleInit(bool FirstGo)
{
	short ReleIndx;
	ClearLCD();
	LCDPrintString(TWO, CENTER_ALIGN, "Effettuare test");
	LCDPrintString(THREE, CENTER_ALIGN, "diagnostico?");
	if(CheckYesNo())
	{
		ClearLCD();
		LCDPrintString(ONE, CENTER_ALIGN, "Test in corso");
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
	LCDPrintString(TWO, CENTER_ALIGN, "Sto ristabilendo");
	LCDPrintString(THREE, CENTER_ALIGN, "le prese");
	for(ReleIndx = RELE_1; ReleIndx < RELE_MAX; ReleIndx++)
	{
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
			Rele[ReleIndx].TurnOnTime.day = PresentTime.day;
			Rele[ReleIndx].TurnOnTime.hour = PresentTime.hour;
			Rele[ReleIndx].TurnOnTime.minute = PresentTime.minute;
			Rele[ReleIndx].ActiveTime.minute = 0;
			CheckEvents();
		}

		delay(500);
	}
	ClearLCD();
	Flag.ReleRS = true;
	CheckReleStatus();
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
		{
			Flag.AllReleDown = true;
		}
	}
	for(ReleIndx = RELE_1; ReleIndx < RELE_MAX; ReleIndx++)
	{
		if(!Rele[ReleIndx].IsActive)
		{
				Flag.AllReleUp = false;
				break;
		}
		else
		{
			Flag.AllReleUp = true;
		}
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
	else if(Flag.AllReleUp)
	{
		for(ReleIndx = RELE_1; ReleIndx < RELE_MAX; ReleIndx++)
		{
			LCDMoveCursor(Row, 6 + ReleIndx);
			LCDShowIcon(RELE_ON);
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
	String TimerStr, ReleNmbrStr = "per la presa: ";
	ClearLCD();
	CheckEvents();
	ReleNmbrStr += String(ReleNbr + 1);
	LCDPrintString(ONE, CENTER_ALIGN, "Imposta il timer");
	LCDPrintString(TWO, CENTER_ALIGN, ReleNmbrStr);
	delay(2000);
	ClearLCD();
	LCDPrintString(ONE  , CENTER_ALIGN, "Tempo massimo");
	LCDPrintString(TWO  , CENTER_ALIGN, "4h");
	LCDPrintString(THREE, CENTER_ALIGN, "Premere Ok/Set");
	LCDPrintString(FOUR , CENTER_ALIGN, "per confermare");
	delay(3000);
	ClearLCD();
	CheckEvents();
	LCDBlink();
	while(!ExitSetTimer)
	{
		ShowDateTime(ONE);
		LCDPrintString(TWO, CENTER_ALIGN, "Imposta il timer");
		LCDPrintString(THREE, CENTER_ALIGN, ReleNmbrStr);
		LCDPrintValue(FOUR, 8, Hour);
		LCDPrintString(FOUR, 9, ":");
		LCDPrintValue(FOUR, 10, Minute_1);
		LCDPrintValue(FOUR, 11, Minute_2);
		CheckEvents();
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
							Hour = 4;
						break;
					case 1:
						if(Hour != 4)
						{
							if(Minute_1 > 0)
								Minute_1--;
							else
								Minute_1 = 5;
						}
						else
							Minute_1 = 0;
						break;
					case 2:
						if(Hour != 4)
						{
							if(Minute_2 > 0)
								Minute_2--;
							else
								Minute_2 = 9;
						}
						else
							Minute_2 = 0;
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
						if(Hour < 4)
							Hour++;
						else
							Hour = 0;
						break;
					case 1:
						if(Hour != 4)
						{
							if(Minute_1 < 5)
								Minute_1++;
							else
								Minute_1 = 0;
						}
						else
							Minute_1 = 0;
						break;
					case 2:
						if(Hour != 4)
						{
							if(Minute_2 < 9)
								Minute_2++;
							else
								Minute_2 = 0;
						}
						else
							Minute_2 = 0;
						break;
					default:
						break;
				}
				break;
			case BUTTON_LEFT:
				BlinkLed(BUTTON_LED);
				if(Cursor < 2)
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
					if(PresentTime.hour + SetTimer.hour >= 24)
					{
						Rele[ReleNbr].TimerTime.hour = (((PresentTime.hour + SetTimer.hour) % 23) - ((PresentTime.hour + SetTimer.hour) / 23));
						Rele[ReleNbr].TimerTime.minute = PresentTime.minute + SetTimer.minute;
					}
					else if(PresentTime.minute + SetTimer.minute > 59)
					{
						Rele[ReleNbr].TimerTime.hour = PresentTime.hour + 1;
						if(PresentTime.hour + 1 == 24)
						{
							Rele[ReleNbr].TimerTime.hour = 0;
						}
						Rele[ReleNbr].TimerTime.minute = (((PresentTime.minute + SetTimer.minute) % 59) - ((PresentTime.minute + SetTimer.minute) / 59));
					}
					ExitSetTimer = true;
					LCDNoBlink();
				}
				else if((PresentTime.hour + SetTimer.hour >= 24) && (PresentTime.minute + SetTimer.minute > 59))
				{
					Rele[ReleNbr].TimerTime.hour = (((PresentTime.hour + SetTimer.hour) % 23));
					Rele[ReleNbr].TimerTime.minute = (((PresentTime.minute + SetTimer.minute) % 59) - ((PresentTime.minute + SetTimer.minute) / 59));
					ExitSetTimer = true;
					LCDNoBlink();
				}
				else
				{
					Rele[ReleNbr].TimerTime.hour = PresentTime.hour + SetTimer.hour;
					Rele[ReleNbr].TimerTime.minute = PresentTime.minute + SetTimer.minute;
					ExitSetTimer = true;
					LCDNoBlink();
				}
				break;
			default:
				break;
		}
		switch(Cursor)
		{
			case 0:
				LCDMoveCursor(FOUR, 8);
				break;
			case 1:
				LCDMoveCursor(FOUR, 10);
				break;
			case 2:
				LCDMoveCursor(FOUR, 11);
				break;
			default:
				break;
		}
		delay(WHILE_LOOP_DELAY*4); // 120ms per il blink
		ButtonPress = NO_PRESS;
	}
	ClearLCD();
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
