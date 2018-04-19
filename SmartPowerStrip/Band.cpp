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
BAND_FORMAT Band;
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
		if((Band.InitHour <= PresentTime.hour && Band.InitMinute <= PresentTime.minute) &&
			(Band.EndHour >= PresentTime.hour && Band.EndMinute >= PresentTime.minute))
		{
			InBand = true;
			Flag.BandActive = true;
		}
		else
		{
			if(Band.InitDay < PresentTime.day)
			{
				if((0 <= PresentTime.hour && 0 <= PresentTime.minute) &&
				  (Band.EndHour >= PresentTime.hour && Band.EndMinute >= PresentTime.minute))				
			}
			else
			{
				InBand = false;
				Flag.BandActive = false;
			}
		}
	}
	return InBand;
}

bool IsBandCorrect()
{
	bool BandCorrect = false;
	if(Band.InitHour <= Band.EndHour)
	{
		if(Band.InitMinute < Band.EndMinute)
		{
			BandCorrect = true;
			Flag.BandInvalid = false;
		}
		else
		{
			BandCorrect = false;
			Flag.BandInvalid = true;
		}
	}
	else
	{
		TakePresentTime();
		if(Band.InitDay < PresentTime.day)
		{
			BandCorrect = true;
			Flag.BandInvalid = false;
		}
		else
		{
			BandCorrect = false;
			Flag.BandInvalid = true;
		}
		
	}
	return BandCorrect;
}


void SetBandInvalid()
{
	Band.hour = INVALID_BAND_VALUE;
	Band.minute = INVALID_BAND_VALUE;
	ClearLCD();
	LCDPrintString(1, CENTER_ALIGN, "La banda non è");
	LCDPrintString(2, CENTER_ALIGN, "valida");
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
	short ButtonPress, TimeVar = INIT_HOUR;
	short Hour = PresentTime.hour, Minute = PresentTime.minute;
	short OldHour = PresentTime.hour, OldMinute = PresentTime.minute;
	String MinuteStr;
	ClearLCD();
	LCDPrintString(0, CENTER_ALIGN, "Cambiare l'orario");
	LCDPrintString(1, CENTER_ALIGN, "della banda");
	LCDPrintString(2, CENTER_ALIGN, "con Up e Down");
	delay(2000);
	ClearLCD();
	LCDPrintString(0, CENTER_ALIGN, "Premere Ok/Set");
	LCDPrintString(1, CENTER_ALIGN, "per confermare");
	LCDPrintString(2, CENTER_ALIGN, "Premere Left/Back");
	LCDPrintString(3, CENTER_ALIGN, "per uscire");
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
			case INIT_HOUR:
				ButtonPress = CheckButtons();
				LCDPrintString(2, CENTER_ALIGN, "Ora iniziale:");
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
							Band.InitHour = Hour;
							ValidSet = true;
							TimeVar = INIT_MINUTE;
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
							TimeVar = INIT_MINUTE;
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
			case INIT_MINUTE:
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
							Band.InitMinute = Minute;
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
								Band.InitMinute = Minute;
								ValidSet = true;
								TimeVar = EXIT;	
								Flag.BandInvalid = false;
							}
							else
							{
								ClearLCD();
								LCDPrintString(1, CENTER_ALIGN, "Valore non valido");
								ValidSet = false;
								TimeVar = END_HOUR;									
							}
						}
						break;
			case END_HOUR:
				ButtonPress = CheckButtons();
				LCDPrintString(2, CENTER_ALIGN, "Ora finale:");
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
							Band.EndHour = Hour;
							ValidSet = true;
							TimeVar = END_MINUTE;
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
							TimeVar = END_MINUTE;
						}
						break;
					case BUTTON_LEFT:
						BlinkLed(BUTTON_LED);
						TimeVar = END_MINUTE;
						ValidSet = true;
					default:
						break;
				}
				break;
			case END_MINUTE:
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
							Band.EndMinute = Minute;
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
								Band.EndMinute = Minute;
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
				Band.InitDay = PresentTime.day;
				ExitSetTimeBand = true;
				break;
			default:
				break;
			
		}
		
	}
	return ValidSet;
}