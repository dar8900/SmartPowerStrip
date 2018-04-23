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

void BandInit()
{
	short ValidValue = 0, indx;
	for(indx = INIT_HOUR; indx <= END_MINUTE; indx++)
	{
		ReadBandValues(indx);
	}
	CheckBand();
}

bool CheckBand()
{
	bool InBand = false;
	short  ValidBand;
	TakePresentTime();
	ReadMemory(BAND_VALIDATION_VALUE_ADDR, 1, &ValidBand);
	if(ValidBand == 0)
		Flag.BandInvalid = false;
	else
		Flag.BandInvalid = true;
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
			else
			{
				InBand = false;
				Flag.BandActive = false;
			}
		}
	}
	TakeReleTime();
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
			WriteMemory(BAND_VALIDATION_VALUE_ADDR, 0);
		}
		else
		{
			BandCorrect = false;
			SetBandInvalid();
		}
	}
	else
	{
		TakePresentTime();
		if(Band.InitDay < PresentTime.day)
		{
			BandCorrect = true;
			Flag.BandInvalid = false;
			WriteMemory(BAND_VALIDATION_VALUE_ADDR, 0);
		}
		else
		{
			BandCorrect = false;
			SetBandInvalid();
		}
		
	}
	return BandCorrect;
}


void SetBandInvalid()
{
	Band.InitHour = INVALID_BAND_VALUE;
	Band.InitMinute = INVALID_BAND_VALUE;
	Band.EndHour = INVALID_BAND_VALUE;
	Band.EndMinute = INVALID_BAND_VALUE;
	ClearLCD();
	LCDPrintString(TWO, CENTER_ALIGN, "La banda è");
	LCDPrintString(THREE, CENTER_ALIGN, "disabilitata");
	delay(2000);
	TakeReleTime();
	ClearLCD();
	Flag.BandInvalid = true;
	WriteMemory(BAND_VALIDATION_VALUE_ADDR, 1);
}

void SaveBandValues(short WichItem, short Value)
{
	if(WichItem == INIT_HOUR)
	{
		WriteMemory(BAND_INIT_HOUR_ADDR, Value);
	}
	else if(WichItem == INIT_MINUTE)
	{
		WriteMemory(BAND_INIT_MINUTE_ADDR, Value);
	}
	else if(WichItem == END_HOUR)
	{
		WriteMemory(BAND_END_HOUR_ADDR, Value);
	}
    else if(WichItem == END_MINUTE)
	{
		WriteMemory(BAND_END_MINUTE_ADDR, Value);
	}
}

void ReadBandValues(short WichItem)
{
	if(WichItem == INIT_HOUR)
	{
		ReadMemory(BAND_INIT_HOUR_ADDR, 1, &Band.InitHour);
	}
	else if(WichItem == INIT_MINUTE)
	{
		ReadMemory(BAND_INIT_MINUTE_ADDR, 1, &Band.InitMinute);
	}
	else if(WichItem == END_HOUR)
	{
		ReadMemory(BAND_END_HOUR_ADDR, 1, &Band.EndHour);
	}
    else if(WichItem == END_MINUTE)
	{
		ReadMemory(BAND_END_MINUTE_ADDR, 1, &Band.EndMinute);
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
	TakeReleTime();
	LCDPrintString(ONE  , CENTER_ALIGN, "Cambiare l'orario");
	LCDPrintString(TWO  , CENTER_ALIGN, "della banda");
	LCDPrintString(THREE, CENTER_ALIGN, "con Up e Down");
	delay(2000);
	ClearLCD();
	LCDPrintString(ONE  , CENTER_ALIGN, "Premere Ok/Set");
	LCDPrintString(TWO  , CENTER_ALIGN, "per confermare");
	LCDPrintString(THREE, CENTER_ALIGN, "Premere Left/Back");
	LCDPrintString(FOUR , CENTER_ALIGN, "per uscire");
	delay(2000);
	ClearLCD();
	TakeReleTime();
	while(!ExitSetTimeBand)
	{
		ClearLCD();
		TakePresentTime();
		TakeReleTime();
		LCDPrintString(ONE  , CENTER_ALIGN, "Cambiare l'orario");
		LCDPrintString(TWO  , CENTER_ALIGN, "della banda");
		switch(TimeVar)
		{
			case INIT_HOUR:
				ButtonPress = CheckButtons();
				LCDPrintString(THREE, CENTER_ALIGN, "Ora iniziale:");
				LCDPrintValue(FOUR, CENTER_ALIGN, Hour);
				TakeReleTime();
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
							LCDPrintString(TWO, CENTER_ALIGN, "Valore Salvato");
							delay(1000);
							SaveBandValues(INIT_HOUR, Hour);
							Band.InitHour = Hour;
							ValidSet = true;
							TimeVar = INIT_MINUTE;
						}
						else if(Hour != OldHour && Hour < OldHour)
						{
							ClearLCD();
							LCDPrintString(TWO, CENTER_ALIGN, "Valore non valido");
							LCDPrintString(THREE, CENTER_ALIGN, "Re-inserire ora");
							delay(1000);
							ClearLCD();
							ValidSet = false;
							TimeVar = INIT_HOUR;							
						}
						else if(Hour == OldHour)
						{
							ClearLCD();
							LCDPrintString(TWO, CENTER_ALIGN, "Valore uguale");
							delay(1000);
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
				LCDPrintString(THREE, CENTER_ALIGN, "Minuti:");	
				TakeReleTime();
				if(Minute < 10)
				{
					MinuteStr += String(Minute);
				}
				else
				{
					MinuteStr = String(Minute);					
				}
				LCDPrintString(FOUR, CENTER_ALIGN, MinuteStr);
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
							LCDPrintString(TWO, CENTER_ALIGN, "Valore Salvato");
							delay(1000);
							SaveBandValues(INIT_MINUTE, Minute);
							Band.InitMinute = Minute;
							ValidSet = true;
							TimeVar = END_HOUR;	
							Flag.BandInvalid = false;
							WriteMemory(BAND_VALIDATION_VALUE_ADDR, 0);
						}
						else
						{
							if(Minute > OldMinute)
							{
								ClearLCD();
								LCDPrintString(TWO, CENTER_ALIGN, "Valore Salvato");
								delay(1000);
								SaveBandValues(INIT_MINUTE, Minute);
								Band.InitMinute = Minute;
								ValidSet = true;
								TimeVar = END_HOUR;	
								Flag.BandInvalid = false;
								WriteMemory(BAND_VALIDATION_VALUE_ADDR, 0);
							}
							else
							{
								ClearLCD();
								LCDPrintString(TWO  , CENTER_ALIGN, "Valore non valido");
								LCDPrintString(THREE, CENTER_ALIGN, "Re-inserire i");
								LCDPrintString(FOUR , CENTER_ALIGN, "minuti");
								delay(1000);
								ClearLCD();
								ValidSet = false;
								TimeVar = INIT_MINUTE;									
							}
						}
						break;
				}
				break;
			case END_HOUR:
				ButtonPress = CheckButtons();
				LCDPrintString(THREE, CENTER_ALIGN, "Ora finale:");
				LCDPrintValue(FOUR, CENTER_ALIGN, Hour);
				TakeReleTime();
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
							LCDPrintString(TWO, CENTER_ALIGN, "Valore Salvato");
							delay(1000);
							SaveBandValues(END_HOUR, Hour);
							Band.EndHour = Hour;
							ValidSet = true;
							TimeVar = END_MINUTE;
						}
						else if(Hour != OldHour && Hour < OldHour)
						{
							ClearLCD();
							LCDPrintString(TWO, CENTER_ALIGN, "Valore non valido");
							LCDPrintString(THREE, CENTER_ALIGN, "Re-inserire ora");
							delay(1000);
							ClearLCD();
							ValidSet = false;
							TimeVar = END_HOUR;							
						}
						else if(Hour == OldHour)
						{
							ClearLCD();
							LCDPrintString(TWO, CENTER_ALIGN, "Valore uguale");
							delay(1000);
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
				LCDPrintString(THREE, CENTER_ALIGN, "Minuti:");	
				TakeReleTime();
				if(Minute < 10)
				{
					MinuteStr += String(Minute);
				}
				else
				{
					MinuteStr = String(Minute);					
				}
				LCDPrintString(FOUR, CENTER_ALIGN, MinuteStr);
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
							LCDPrintString(TWO, CENTER_ALIGN, "Valore Salvato");
							delay(1000);
							SaveBandValues(END_MINUTE, Minute);
							Band.EndMinute = Minute;
							ValidSet = true;
							TimeVar = EXIT;	
							Flag.BandInvalid = false;
							WriteMemory(BAND_VALIDATION_VALUE_ADDR, 0);
						}
						else
						{
							if(Minute > OldMinute)
							{
								ClearLCD();
								LCDPrintString(TWO, CENTER_ALIGN, "Valore Salvato");
								delay(1000);
								SaveBandValues(END_MINUTE, Minute);
								Band.EndMinute = Minute;
								ValidSet = true;
								TimeVar = EXIT;	
								Flag.BandInvalid = false;
								WriteMemory(BAND_VALIDATION_VALUE_ADDR, 0);
							}
							else
							{
								ClearLCD();
								LCDPrintString(TWO, CENTER_ALIGN, "Valore non valido");
								LCDPrintString(THREE, CENTER_ALIGN, "Re-inserire i");
								LCDPrintString(FOUR, CENTER_ALIGN, "minuti");
								delay(1000);
								ClearLCD();
								ValidSet = false;
								TimeVar = END_MINUTE;									
							}
						}
						break;
					case BUTTON_LEFT:
						BlinkLed(BUTTON_LED);
						TimeVar = EXIT;
						TakeReleTime();
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