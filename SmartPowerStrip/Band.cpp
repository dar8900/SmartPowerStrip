#include "LCDLib.h"
#include "TimeLib.h"
#include "SmartPowerStrip.h"
#include "EEPROM_Ard.h"
#include "Buttons.h"
#include "Menu.h"
#include "Web.h"
#include "Rele.h"
#include "Band.h"

extern TIME_DATE_FORMAT PresentTime;
TIME_DATE_FORMAT Band;
extern FLAGS Flag;

bool CheckBand()
{
	bool InBand = false;
	TakePresentTime();
	if(Flag.BandInvalid)
	{
		InBand = false;
		Flag.BandActive = false;
	}
	else
	{
		if(PresentTime.hour >= Band.hour && PresentTime.minute >= Band.minute)
		{
			InBand = true;
			Flag.BandActive = true;
		}
		else
		{
			InBand = false;
			Flag.BandActive = false;
		}
	}
	return InBand;
}

void SetBandInvalid()
{
	Band.hour = INVALID_BAND_VALUE;
	Band.minute = INVALID_BAND_VALUE;
	ClearLCD();
	LCDPrintString(1, CENTER_ALIGN, "La banda è");
	LCDPrintString(1, CENTER_ALIGN, "invalidata");
	delay(2000);
	ClearLCD();
	Flag.BandInvalid = true;
}

void SaveBandValues(short WichItem, short Value)
{
	if(WichItem == HOUR)
	{
		WriteMemory(BAND_HOUR_ADDR, Value);
	}
	else if(WichItem == MINUTE)
	{
		WriteMemory(BAND_MINUTE_ADDR, Value);
	}
}

void ReadBandValues(short WichItem)
{
	if(WichItem == HOUR)
	{
		ReadMemory(BAND_HOUR_ADDR, 1, &Band.hour);
	}
	else if(WichItem == MINUTE)
	{
		ReadMemory(BAND_MINUTE_ADDR, 1, &Band.minute);
	}
}


bool SetTimeBand()
{
	bool ExitSetTimeBand = false, ValidSet = false;
	short ButtonPress, TimeVar = HOUR;
	short Hour = PresentTime.hour, Minute = PresentTime.minute;
	short OldHour = PresentTime.hour, OldMinute = PresentTime.minute ;
	String MinuteStr;
	ClearLCD();
	LCDPrintString(0, CENTER_ALIGN, "Cambiare l'orario");
	LCDPrintString(1, CENTER_ALIGN, "della banda");
	LCDPrintString(2, CENTER_ALIGN, "con Up e Down");
	delay(2000);
	ClearLCD();
	LCDPrintString(0, CENTER_ALIGN, "Premere Ok/Set");
	LCDPrintString(1, CENTER_ALIGN, "per confermare");
	LCDPrintString(0, CENTER_ALIGN, "Premere Left/Back");
	LCDPrintString(1, CENTER_ALIGN, "per uscire");
	delay(2000);
	ClearLCD();
	while(!ExitSetTimeBand)
	{
		ClearLCD();
		TakePresentTime();
		LCDPrintString(0, CENTER_ALIGN, "Cambiare l'orario");
		LCDPrintString(1, CENTER_ALIGN, "della banda");
		switch(TimeVar)
		{
			case HOUR:
				ButtonPress = CheckButtons();
				LCDPrintString(2, CENTER_ALIGN, "Ore:");
				LCDPrintValue(3, CENTER_ALIGN, Hour);
				switch(ButtonPress)
				{
					case BUTTON_UP:
						BlinkLed(BUTTON_LED);
						if(Hour > 0)
							Hour--;
						else
							Hour = 23;
						break;
					case BUTTON_DOWN:
						BlinkLed(BUTTON_LED);
						if(Hour < 23)
							Hour++;
						else
							Hour = 0;
						break;
					case BUTTON_SET:
						BlinkLed(BUTTON_LED);
						if(Hour != OldHour && Hour > OldHour)
						{
							ClearLCD();
							LCDPrintString(1, CENTER_ALIGN, "Valore Salvato");
							SaveBandValues(HOUR, Hour);
							Band.hour = Hour;
							ValidSet = true;
							TimeVar = MINUTE;
						}
						else if(Hour != OldHour && Hour < OldHour)
						{
							ClearLCD();
							LCDPrintString(1, CENTER_ALIGN, "Valore non valido");
							ValidSet = false;
							TimeVar = EXIT;							
						}
						else if(Hour == OldHour)
						{
							ClearLCD();
							LCDPrintString(1, CENTER_ALIGN, "Valore uguale");
							ValidSet = true;
							TimeVar = MINUTE;
						}
						break;
					case BUTTON_LEFT:
						BlinkLed(BUTTON_LED);
						TimeVar = EXIT;
						ValidSet = true;
					default:
						break;
				}
				break;
			case MINUTE:
				ButtonPress = CheckButtons();
				MinuteStr = "0";
				LCDPrintString(2, CENTER_ALIGN, "Minuti:");	
				if(Minute < 10)
				{
					MinuteStr += String(Minute);
				}
				else
				{
					MinuteStr = String(Minute);					
				}
				LCDPrintString(3, CENTER_ALIGN, MinuteStr);
				switch(ButtonPress)
				{
					case BUTTON_UP:
						BlinkLed(BUTTON_LED);
						if(Minute > 0)
							Minute--;
						else
							Minute = 59;
						break;
					case BUTTON_DOWN:
						BlinkLed(BUTTON_LED);
						if(Minute < 59)
							Minute++;
						else
							Minute = 0;
						break;
					case BUTTON_SET:
						BlinkLed(BUTTON_LED);
						if(Hour != OldHour)
						{
							ClearLCD();
							LCDPrintString(1, CENTER_ALIGN, "Valore Salvato");
							SaveBandValues(MINUTE, Minute);
							Band.minute = Minute;
							ValidSet = true;
							TimeVar = EXIT;	
							Flag.BandInvalid = false;
						}
						else
						{
							if(Minute > OldMinute)
							{
								ClearLCD();
								LCDPrintString(1, CENTER_ALIGN, "Valore Salvato");
								SaveBandValues(MINUTE, Minute);
								Band.minute = Minute;
								ValidSet = true;
								TimeVar = EXIT;	
								Flag.BandInvalid = false;
							}
							else
							{
								ClearLCD();
								LCDPrintString(1, CENTER_ALIGN, "Valore non valido");
								ValidSet = false;
								TimeVar = EXIT;									
							}
						}
						break;
					case BUTTON_LEFT:
						BlinkLed(BUTTON_LED);
						TimeVar = EXIT;
						ValidSet = true;
					default:
						break;
				}
				break;
			case EXIT:
				ExitSetTimeBand = true;
				break;
			default:
				break;
			
		}
		
	}
	return ValidSet;
}