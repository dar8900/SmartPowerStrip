#include "LCDLib.h"
#include "TimeLib.h"
#include "SmartPowerStrip.h"
#include "EEPROM_Ard.h"
#include "Buttons.h"
#include "Menu.h"
#include "Web.h"
#include "Band.h"
#include "Rele.h"

extern TIME_DATE_FORMAT PresentTime;
extern BAND_FORMAT Band;
extern RELE Rele[];
extern FLAGS Flag;
extern String HostName;


MENU_VOICES MainMenuItems[]
{
	{ManualRele		,	"Rele Manuali"	},
	{ChangeTimeBand	,	"Cambio Banda"	},
	{HelpInfo		,	"Help e Info"	},
	{WiFiInfo		,	"Wifi Info"		},
	{AssignReleTimer,	"Assegna Timer"	},
};

static const String ONOFF[] = {"Off", "On"}; 

bool SetupInterrupt()
{
	bool EnterSetup = false;
	short ButtonPress = NO_PRESS, InterruptTimer = 10;
	while(InterruptTimer > 0)
	{
		ButtonPress = CheckButtons();
		if(ButtonPress == BUTTON_SET)
		{
			BlinkLed(BUTTON_LED);
			EnterSetup = true;
		}
		else
		{
			EnterSetup = false;
		}
		InterruptTimer--;
		delay(8);
	}
	return EnterSetup;
}

void MainScreen()
{
	String Time,Date;
	short ReleIndx = 0;
	int TimerMenu = 300; // 30s circa
	bool EnterSetup = false;
	if(!Flag.BandActive && !CheckBand())
	{
		ClearLCD();
		if(!Flag.IsDisplayOn)
		{
			LCDDisplayOn();
			Flag.IsDisplayOn = true;
		}
		TakeReleTime();
		if(!Flag.ReleRS)
		{
			ReleReStart();
		}
		LCDPrintString(ONE, CENTER_ALIGN, "Premere Ok/Set");
		LCDPrintString(TWO, CENTER_ALIGN, "per entrare nel");
		LCDPrintString(THREE, CENTER_ALIGN, "Menu Principale");
		delay(3000);
		while(!EnterSetup)
		{
			ClearLCD();
			CheckReleStatus();
			TakeReleTime();
			EnterSetup = SetupInterrupt();
			TakePresentTime();
			// Scrittura data e ora
			Time = String(PresentTime.hour);
			if(PresentTime.minute < 10)
			{
				Time += ":0" + String(PresentTime.minute);
			}
			else
			{
				Time += ":" + String(PresentTime.minute);
			}
			Date = String(PresentTime.day) + "/" + String(PresentTime.month);
			LCDPrintString(ONE, LEFT_ALIGN, Time);
			LCDPrintString(ONE, RIGHT_ALIGN, Date);
			// Stato relè
			LCDPrintString(TWO, CENTER_ALIGN, "Prese attive:");
			ShowReleIcons(THREE);
			// Stato wifi
			LCDPrintString(FOUR, LEFT_ALIGN, "Wifi status:");
			if(Flag.WifiActive)
			{
				LCDMoveCursor(FOUR, 13);
				LCDShowIcon(WIFI_OK);
			}
			else
			{
				LCDMoveCursor(FOUR, 13);
				LCDShowIcon(WIFI_NO);
			}
			
			// GESTIONE PARTE WEB
			
			TimerMenu--;
			delay(100);
			
			if(TimerMenu == 0 && !EnterSetup)
			{
				ClearLCD();
				TakeReleTime();
				LCDPrintString(ONE, CENTER_ALIGN, "Premere Ok/Set");
				LCDPrintString(TWO, CENTER_ALIGN, "per entrare nel");
				LCDPrintString(THREE, CENTER_ALIGN, "Menu Principale");
				delay(3000);
				TakeReleTime();
				TimerMenu = 300;
			}
		}
		if(EnterSetup)
		{
			MainMenu();
			EnterSetup = false;
		}
	}
}

void MainMenu()
{
	bool ExitMainMenu = false;
	bool ReEnterMenu = false;
	short ButtonPress = 0, Item = MANUAL_RELE;
	ClearLCD();
	LCDPrintString(ONE  , CENTER_ALIGN, "Premere Up o Down");
	LCDPrintString(TWO  , CENTER_ALIGN, "per scegliere");
	LCDPrintString(THREE, CENTER_ALIGN, "Premere Ok/Set");
	LCDPrintString(FOUR , CENTER_ALIGN, "per confermare");
	delay(2000);
	ClearLCD();
	LCDPrintString(ONE  , CENTER_ALIGN, "Premere Left/Back");
	LCDPrintString(TWO  , CENTER_ALIGN, "per tornare alla");
	LCDPrintString(THREE, CENTER_ALIGN, "schermata iniziale");
	delay(2000);
	ClearLCD();
	while(!ExitMainMenu)
	{	
		if(ReEnterMenu)
		{
			ClearLCD();
			TakeReleTime();
			LCDPrintString(ONE  , CENTER_ALIGN, "Premere Up o Down");
			LCDPrintString(TWO  , CENTER_ALIGN, "per scegliere");
			LCDPrintString(THREE, CENTER_ALIGN, "Premere Ok/Set");
			LCDPrintString(FOUR , CENTER_ALIGN, "per confermare");
			delay(2000);
			ClearLCD();
			LCDPrintString(ONE  , CENTER_ALIGN, "Premere Left/Back");
			LCDPrintString(TWO  , CENTER_ALIGN, "per tornare alla");
			LCDPrintString(THREE, CENTER_ALIGN, "schermata iniziale");
			delay(2000);
			ClearLCD();
			TakeReleTime();
			ReEnterMenu = false;
		}
		ClearLCD();
		TakeReleTime();
		LCDPrintString(2, CENTER_ALIGN, MainMenuItems[Item].MenuTitle);
		TakePresentTime();
		ButtonPress = CheckButtons();
		switch(ButtonPress)
		{
			case BUTTON_UP:
				BlinkLed(BUTTON_LED);
				if(Item < MANUAL_RELE)
				{
					Item = MAX_MENU_ITEM - 1;
				}
				else
				{
					Item--;
				}
				break;
			case BUTTON_DOWN:
				BlinkLed(BUTTON_LED);
				if(Item > MAX_MENU_ITEM - 1)
				{
					Item = MANUAL_RELE;
				}
				else
				{
					Item++;
				}
				break;
			case BUTTON_SET:
				BlinkLed(BUTTON_LED);
				ReEnterMenu = true;
				break;
			case BUTTON_LEFT:
				BlinkLed(BUTTON_LED);
				ExitMainMenu = true;
				break;
			default:
				break;			
		}
		delay(100);
		if(ReEnterMenu)
		{
			MainMenuItems[Item].MenuFunc();
		}
	}
}

bool ManualRele()
{
	short ReleIndx = RELE_1;
	bool ReleSetted = false;
	short ButtonPress = 0, Status = 0, OldStatus = 0;
	String SelRele = "Impostare il Rele ";
	for(ReleIndx = RELE_1; ReleIndx < RELE_MAX; ReleIndx++)
	{		
		ClearLCD();
		TakePresentTime();
		TakeReleTime();
		ReleSetted = false;
		LCDPrintString(1, CENTER_ALIGN, SelRele + String(ReleIndx + 1));
		if(Rele[ReleIndx].IsActive)
		{
			LCDPrintString(2, CENTER_ALIGN, ONOFF[STATUS_ON]);
			Status = STATUS_ON;
		}
		else
		{
			LCDPrintString(2, CENTER_ALIGN, ONOFF[STATUS_OFF]);
			Status = STATUS_OFF;
		}	
		OldStatus = Status;
		while(!ReleSetted)
		{
			ClearLCD();
			TakePresentTime();
			ButtonPress = CheckButtons();
			LCDPrintString(2, CENTER_ALIGN, ONOFF[Status]);
			switch(ButtonPress)
			{
				case BUTTON_UP:
					BlinkLed(BUTTON_LED);
					if(Status == STATUS_ON)
						Status = STATUS_OFF;
					else
						Status = STATUS_ON;
					break;
				case BUTTON_DOWN:
					BlinkLed(BUTTON_LED);
					if(Status == STATUS_ON)
						Status = STATUS_OFF;
					else
						Status = STATUS_ON;
					break;
				case BUTTON_SET:
					BlinkLed(BUTTON_LED);
					ClearLCD();
					if(OldStatus != Status)
						LCDPrintString(2, CENTER_ALIGN, "Valore Salvato");
					else
						LCDPrintString(2, CENTER_ALIGN, "Valore non cambiato");
					if(Status == 0)
					{
						Rele[ReleIndx].IsActive = false;
						Rele[ReleIndx].ActiveTime = 0;
						Rele[ReleIndx].TurnOnTime = 0;
						OFF(ReleIdx2Pin(ReleIndx));
					}
					else
					{
						Rele[ReleIndx].IsActive = true;
						ON(ReleIdx2Pin(ReleIndx));
						Flag.AllReleDown = false;
						Rele[ReleIndx].TurnOnTime = PRESENT_DAY_HOUR_MINUTE_SECOND;
					}
					WriteMemory(Rele[ReleIndx].EepromAddr, Status);
					ReleSetted = true;
					delay(1500);
					break;
				case BUTTON_LEFT:
				default:
					break;
			}
			delay(50);
		}	
	}
	CheckReleStatus();
}

bool ChangeTimeBand()
{
	bool ExitChangeBand = false, OkBandSet = true;
	short ButtonPress;
    ClearLCD();
	TakeReleTime();
	LCDPrintString(ONE  , CENTER_ALIGN, "Cambiare la banda");
	LCDPrintString(TWO  , CENTER_ALIGN, "per spegnere la");
	LCDPrintString(THREE, CENTER_ALIGN, "ciabatta all'interno");
	LCDPrintString(FOUR , CENTER_ALIGN, "dell'orario");
	delay(3000);
	ClearLCD();
	LCDPrintString(ONE  , CENTER_ALIGN, "Premere Ok/Set");
	LCDPrintString(TWO  , CENTER_ALIGN, "per cambiare");
	LCDPrintString(THREE, CENTER_ALIGN, "la banda o");
	LCDPrintString(FOUR , CENTER_ALIGN, "diabilitarla");
	delay(3000);
	ClearLCD();
	LCDPrintString(ONE  , CENTER_ALIGN, "Premere Left/Back");
	LCDPrintString(TWO  , CENTER_ALIGN, "per tornare al");
	LCDPrintString(THREE, CENTER_ALIGN, "Menu Principale");
	delay(3000);
	ClearLCD();
	TakeReleTime();
	while(!ExitChangeBand)
	{
		if(!OkBandSet)
		{
			ClearLCD();
			TakeReleTime();
			LCDPrintString(ONE  , CENTER_ALIGN, "Premere Ok/Set");
			LCDPrintString(TWO  , CENTER_ALIGN, "per cambiare");
			LCDPrintString(THREE, CENTER_ALIGN, "la banda o");
			LCDPrintString(FOUR , CENTER_ALIGN, "diabilitarla");
			delay(4000);
			ClearLCD();
			LCDPrintString(ONE  , CENTER_ALIGN, "Premere Left/Back");
			LCDPrintString(TWO  , CENTER_ALIGN, "per tornare al");
			LCDPrintString(THREE, CENTER_ALIGN, "Menu Principale");
			delay(2000);

			ClearLCD();	
			OkBandSet = true;
		}
		TakePresentTime();
		TakeReleTime();
		ButtonPress = CheckButtons();
		LCDPrintString(1, CENTER_ALIGN, "Cambia Banda");
		switch(ButtonPress)
		{
			case BUTTON_UP:
			case BUTTON_DOWN:
			default:
				break;
			case BUTTON_LEFT:
				BlinkLed(BUTTON_LED);
				ExitChangeBand = true;
				break;
			case BUTTON_SET:
				BlinkLed(BUTTON_LED);
				if(!Flag.IsBandSetted)
				{
					if(SetTimeBand())
					{
					  if(IsBandCorrect())
						{
							Flag.IsBandSetted = true;
							ExitChangeBand = true;
						}
						else
						{	
							ClearLCD();
							LCDPrintString(0, CENTER_ALIGN, "La banda");
							LCDPrintString(1, CENTER_ALIGN, "non è");
							LCDPrintString(2, CENTER_ALIGN, "corretta");
							delay(2000);
							ClearLCD();
							TakeReleTime();
							SetBandInvalid();
							ExitChangeBand = false;	
							OkBandSet = false;
						}
					}
				}					
				else
				{
					SetBandInvalid();
					ExitChangeBand = true;
				}
				break;
		}
		delay(60);
	}
}

bool HelpInfo()
{
	short NumTimer = 0, ReleIndx = 0, ReleTimer[2];
	String BandTime1, BandTime2;
	TakeReleTime();
	ClearLCD();
	LCDPrintString(THREE, CENTER_ALIGN, "Info e Aiuto");
	delay(2000);
	TakeReleTime();
	ClearLCD();
	LCDPrintString(ONE, CENTER_ALIGN, "Stato Rele:");
	ShowReleIcons(TWO);
	LCDPrintString(THREE, CENTER_ALIGN, "Stato Wifi:");
	if(Flag.WifiActive)
	{
		LCDMoveCursor(THREE, 13);
		LCDShowIcon(WIFI_OK);
	}
	else
	{
		LCDMoveCursor(THREE, 13);
		LCDShowIcon(WIFI_NO);
	}
	delay(3000);
	TakeReleTime();
	ClearLCD();
	for(ReleIndx = RELE_1; ReleIndx < RELE_MAX; ReleIndx++)
	{
		if(Rele[ReleIndx].HaveTimer)
		{	
			if(NumTimer > 1)
				break;
			ReleTimer[NumTimer] = ReleIndx;
			NumTimer++;
		}
	}
	LCDPrintString(ONE, LEFT_ALIGN, "Timer attivi:");
	LCDPrintValue(ONE, 15, NumTimer);
	LCDPrintString(TWO, LEFT_ALIGN, "Rele associati:");
	LCDPrintValue(THREE, 5, ReleTimer[0]);
	LCDPrintValue(THREE, 15, ReleTimer[1]);
	delay(3000);
	TakeReleTime();
	ClearLCD();
	LCDPrintString(ONE, CENTER_ALIGN, "Stato Banda:");
	if(Flag.IsBandSetted)
	{
		LCDPrintString(TWO, CENTER_ALIGN, "Settata");
		LCDPrintString(THREE, CENTER_ALIGN, "Orario impostato:");
		BandTime1 = String(Band.InitHour) + ":";
		BandTime2 = String(Band.EndHour) + ":";
		if(Band.InitMinute < 10)
			BandTime1 += "0" + String(Band.InitMinute);
		else
			BandTime1 += String(Band.InitMinute);
		if(Band.EndMinute < 10)
			BandTime2 += "0" + String(Band.EndMinute);
		else
			BandTime2 += String(Band.EndMinute);	
		
		BandTime1 = BandTime1 + "  " + BandTime2;
		LCDPrintString(FOUR, CENTER_ALIGN, BandTime1);
	}
	else
		LCDPrintString(TWO, CENTER_ALIGN, "Non Settata");
	delay(4000);
	TakeReleTime();
	ClearLCD();
	LCDPrintString(THREE, CENTER_ALIGN, "Uscita...");
	delay(2000);
	TakeReleTime();
	ClearLCD();
	return true;
}

// Per ora mostro solo IP e Hostname, per il client aspettare parte WEB
bool WiFiInfo() 
{
	bool ExitWifiInfo = false;
	short ButtonPress = 0;
	ClearLCD();
	LCDPrintString(ONE  , CENTER_ALIGN, "Wifi Info");
	LCDPrintString(TWO  , CENTER_ALIGN, "Premere Left/Back");
	LCDPrintString(THREE, CENTER_ALIGN, "per tornare al");
	LCDPrintString(FOUR , CENTER_ALIGN, "Menu Principale");
	delay(3000);
	TakeReleTime();
	ClearLCD();
	while(1)
	{
		TakePresentTime();
		TakeReleTime();
		ButtonPress = CheckButtons();
		LCDPrintString(ONE, LEFT_ALIGN, "IP:");
		if(Flag.WifiActive)
		{
			LCDPrintString(ONE, RIGHT_ALIGN, WifiIP());
		}
		else
		{
			LCDMoveCursor(ONE, 4);
			LCDShowIcon(WIFI_NO);
		}
		LCDPrintString(TWO, LEFT_ALIGN, "Hostname:");
		LCDPrintString(TWO, RIGHT_ALIGN, HostName);
		switch(ButtonPress)
		{
			case BUTTON_LEFT:
				BlinkLed(BUTTON_LED);
				ExitWifiInfo = true;
				break;
			case BUTTON_UP:
			case BUTTON_DOWN:
			case BUTTON_SET:
			default:
				break;
			
		}
		if(ExitWifiInfo)
			break;
	}
}

bool AssignReleTimer()
{
	short ButtonPress, ReleNumber = RELE_1, TimerAssignedCnt = 0;
	bool ExitAssignReleTimer = false;
	ClearLCD();
	TakeReleTime();
	LCDPrintString(TWO, CENTER_ALIGN, "Assegna un timer");
	LCDPrintString(THREE, CENTER_ALIGN, "a massimo 2 prese");
	delay(2000);
	ClearLCD();
	LCDPrintString(TWO, CENTER_ALIGN, "Premi Up o Down");
	LCDPrintString(THREE, CENTER_ALIGN, "per scegliere");
	LCDPrintString(FOUR, CENTER_ALIGN, "il rele");
	delay(2000);
	ClearLCD();
	LCDPrintString(ONE, CENTER_ALIGN, "Premi Ok/Set");
	LCDPrintString(TWO, CENTER_ALIGN, "per confermare");
	LCDPrintString(THREE, CENTER_ALIGN, "o per disabilitare");
	LCDPrintString(FOUR, CENTER_ALIGN, "un timer");
	delay(2000);
	ClearLCD();
	LCDPrintString(TWO, CENTER_ALIGN, "Premi Left/Back");
	LCDPrintString(THREE, CENTER_ALIGN, "per tornare al");
	LCDPrintString(FOUR, CENTER_ALIGN, "Menu Principale");
	delay(2000);
	ClearLCD();
	TakeReleTime();
	while(!ExitAssignReleTimer)
	{		
		TakeReleTime();
		LCDPrintString(ONE, CENTER_ALIGN, "Assegna il timer");
		LCDPrintString(TWO, CENTER_ALIGN, "alla presa numero:");
		LCDPrintValue(THREE, CENTER_ALIGN, ReleNumber+1);
		if(Rele[ReleNumber].HaveTimer)
		{
			LCDPrintString(FOUR, CENTER_ALIGN, "Timer abilitato");
		}
		ButtonPress = CheckButtons();
		switch(ButtonPress)
		{
			case BUTTON_UP:
				BlinkLed(BUTTON_LED);
				if(ReleNumber > RELE_1)
					ReleNumber--;
				else
					ReleNumber = RELE_MAX - 1;
				break;
			case BUTTON_DOWN:
				BlinkLed(BUTTON_LED);
				if(ReleNumber < RELE_MAX - 1)
					ReleNumber++;
				else
					ReleNumber = RELE_1;
				break;
			case BUTTON_SET:
				BlinkLed(BUTTON_LED);
				if(Rele[ReleNumber].HaveTimer)
				{
					ClearLCD();
					LCDPrintString(THREE, CENTER_ALIGN, "Timer disabilitato");
					delay(2000);
					ClearLCD();
					Rele[ReleNumber].HaveTimer = false;
					Rele[ReleNumber].TimerTime = 0;
					TimerAssignedCnt--;
				}
				else if(TimerAssignedCnt == 1)
				{
					ClearLCD();
					LCDPrintString(ONE, CENTER_ALIGN, "Raggiunto il");
					LCDPrintString(TWO, CENTER_ALIGN, "numero massimo");
					LCDPrintString(THREE, CENTER_ALIGN, "di timer assegnati");	
					delay(3000);
					ClearLCD();	
					LCDPrintString(ONE, CENTER_ALIGN, "Disabilitare");
					LCDPrintString(TWO, CENTER_ALIGN, "un timer");
					LCDPrintString(THREE, CENTER_ALIGN, "o uscire");	
					delay(3000);
					ClearLCD();						
				}
				else
				{
					ClearLCD();
					LCDPrintString(TWO, CENTER_ALIGN, "Timer abilitato");
					delay(2000);
					ClearLCD();
					Rele[ReleNumber].HaveTimer = SetTimerRele(ReleNumber);
					ON(ReleIdx2Pin(ReleNumber));
					Flag.AllReleDown = false;
					TakeReleTime();
					TimerAssignedCnt++;	
				}

				break;
			case BUTTON_LEFT:
				ExitAssignReleTimer = true;
				break;
			default:
				break;
		}
		
	}
}







