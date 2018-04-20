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
	short ButtonPress = NO_PRESS;
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
	return EnterSetup;
}

void MainScreen()
{
	String Time,Date;
	short ReleIndx = 0;
	int TimerMenu = 300; // 30s circa
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
		LCDPrintString(0, CENTER_ALIGN, "Premere Ok/Set");
		LCDPrintString(1, CENTER_ALIGN, "per entrare nel");
		LCDPrintString(2, CENTER_ALIGN, "Menu Principale");
		delay(3000);
		while(!Flag.EnterSetup)
		{
			ClearLCD();
			TakeReleTime();
			Flag.EnterSetup = SetupInterrupt();
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
			LCDPrintString(0, LEFT_ALIGN, Time);
			LCDPrintString(0, RIGHT_ALIGN, Date);
			// Stato relè
			LCDPrintString(1, CENTER_ALIGN, "Prese attive:");
			ShowReleIcons();
			// Stato wifi
			LCDPrintString(3, LEFT_ALIGN, "Wifi status:");
			if(Flag.WifiActive)
			{
				LCDMoveCursor(3, 13);
				LCDShowIcon(WIFI_OK);
			}
			else
			{
				LCDMoveCursor(3, 13);
				LCDShowIcon(WIFI_NO);
			}
			
			// GESTIONE PARTE WEB
			
			TimerMenu--;
			delay(100);
			
			if(TimerMenu == 0)
			{
				ClearLCD();
				TakeReleTime();
				LCDPrintString(0, CENTER_ALIGN, "Premere Ok/Set");
				LCDPrintString(1, CENTER_ALIGN, "per entrare nel");
				LCDPrintString(2, CENTER_ALIGN, "Menu Principale");
				delay(3000);
				TakeReleTime();
				TimerMenu = 300;
			}
		}
		if(Flag.EnterSetup)
		{
			MainMenu();
			Flag.EnterSetup = false;
		}
	}
}

void MainMenu()
{
	bool ExitMainMenu = false;
	bool EnterMenu = false;
	short ButtonPress = 0, Item = MANUAL_RELE;
	ClearLCD();
	LCDPrintString(0, CENTER_ALIGN, "Premere Up o Down");
	LCDPrintString(1, CENTER_ALIGN, "per scegliere");
	LCDPrintString(2, CENTER_ALIGN, "Premere Ok/Set");
	LCDPrintString(3, CENTER_ALIGN, "per confermare");
	delay(2000);
	ClearLCD();
	LCDPrintString(0, CENTER_ALIGN, "Premere Left/Back");
	LCDPrintString(1, CENTER_ALIGN, "per tornare alla");
	LCDPrintString(2, CENTER_ALIGN, "schermata iniziale");
	delay(2000);
	ClearLCD();
	while(!ExitMainMenu)
	{	
		if(EnterMenu)
		{
			ClearLCD();
			TakeReleTime();
			LCDPrintString(0, CENTER_ALIGN, "Premere Up o Down");
			LCDPrintString(1, CENTER_ALIGN, "per scegliere");
			LCDPrintString(2, CENTER_ALIGN, "Premere Ok/Set");
			LCDPrintString(3, CENTER_ALIGN, "per confermare");
			delay(2000);
			ClearLCD();
			LCDPrintString(0, CENTER_ALIGN, "Premere Left/Back");
			LCDPrintString(1, CENTER_ALIGN, "per tornare alla");
			LCDPrintString(2, CENTER_ALIGN, "schermata iniziale");
			delay(2000);
			ClearLCD();
			TakeReleTime();
			EnterMenu = false;
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
				EnterMenu = true;
				break;
			case BUTTON_LEFT:
				BlinkLed(BUTTON_LED);
				ExitMainMenu = true;
				break;
			default:
				break;			
		}
		delay(100);
		if(EnterMenu)
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
	while(ReleIndx < RELE_MAX)
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
		while(1)
		{
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
					}
					else
					{
						Rele[ReleIndx].IsActive = true;
					}
					WriteMemory(Rele[ReleIndx].EepromAddr, Status);
					ReleSetted = true;
					break;
				case BUTTON_LEFT:
				default:
					break;
			}
			if(ReleSetted)
				break;
			delay(50);
		}
		ReleIndx++;	
	}
}

bool ChangeTimeBand()
{
	bool ExitChangeBand = false, OkBandSet = true;
	short ButtonPress;
    ClearLCD();
	TakeReleTime();
	LCDPrintString(0, CENTER_ALIGN, "Cambiare la banda");
	LCDPrintString(1, CENTER_ALIGN, "per spegnere la");
	LCDPrintString(2, CENTER_ALIGN, "ciabatta all'interno");
	LCDPrintString(3, CENTER_ALIGN, "dell'orario");
	delay(3000);
	ClearLCD();
	LCDPrintString(0, CENTER_ALIGN, "Premere Ok/Set");
	LCDPrintString(1, CENTER_ALIGN, "per cambiare");
	LCDPrintString(2, CENTER_ALIGN, "la banda");
	delay(3000);
	ClearLCD();
	LCDPrintString(0, CENTER_ALIGN, "Premere Left/Back");
	LCDPrintString(1, CENTER_ALIGN, "per tornare al");
	LCDPrintString(2, CENTER_ALIGN, "Menu Principale");
	delay(3000);
	ClearLCD();
	TakeReleTime();
	while(!ExitChangeBand)
	{
		if(!OkBandSet)
		{
			ClearLCD();
			TakeReleTime();
			LCDPrintString(0, CENTER_ALIGN, "Premere Ok/Set");
			LCDPrintString(1, CENTER_ALIGN, "per cambiare");
			LCDPrintString(2, CENTER_ALIGN, "la banda");
			delay(4000);
			ClearLCD();
			LCDPrintString(0, CENTER_ALIGN, "Premere Left/Back");
			LCDPrintString(1, CENTER_ALIGN, "per tornare al");
			LCDPrintString(2, CENTER_ALIGN, "Menu Principale");
			delay(2000);
			ClearLCD();	
			LCDPrintString(0, CENTER_ALIGN, "e disabilitare");
			LCDPrintString(1, CENTER_ALIGN, "la banda");
			delay(3000);
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
				SetBandInvalid();
				ExitChangeBand = true;
				break;
			case BUTTON_SET:
				BlinkLed(BUTTON_LED);
				if(SetTimeBand())
				{
				  OkBandSet = IsBandCorrect();
				}
				if(OkBandSet)
					ExitChangeBand = true;
				else
				{
					SetBandInvalid();
					ExitChangeBand = false;
				}
				break;
		}
		delay(60);
	}
}

bool HelpInfo()
{
	
}

// Per ora mostro solo IP e Hostname, per il client aspettare parte WEB
bool WiFiInfo() 
{
	bool ExitWifiInfo = false;
	short ButtonPress = 0;
	ClearLCD();
	LCDPrintString(0, CENTER_ALIGN, "Wifi Info");
	LCDPrintString(1, CENTER_ALIGN, "Premere Left/Back");
	LCDPrintString(2, CENTER_ALIGN, "per tornare al");
	LCDPrintString(2, CENTER_ALIGN, "Menu Principale");
	delay(3000);
	TakeReleTime();
	ClearLCD();
	while(1)
	{
		TakePresentTime();
		TakeReleTime();
		ButtonPress = CheckButtons();
		LCDPrintString(0, LEFT_ALIGN, "IP:");
		LCDPrintString(0, RIGHT_ALIGN, WifiIP());
		LCDPrintString(0, LEFT_ALIGN, "Hostname:");
		LCDPrintString(0, RIGHT_ALIGN, HostName);
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
	LCDPrintString(1, CENTER_ALIGN, "Assegna un timer");
	LCDPrintString(2, CENTER_ALIGN, "a massimo 2 prese");
	delay(2000);
	ClearLCD();
	LCDPrintString(1, CENTER_ALIGN, "Premi Up o Down");
	LCDPrintString(2, CENTER_ALIGN, "per scegliere");
	LCDPrintString(3, CENTER_ALIGN, "il rele");
	delay(2000);
	ClearLCD();
	LCDPrintString(0, CENTER_ALIGN, "Premi Ok/Set");
	LCDPrintString(1, CENTER_ALIGN, "per confermare");
	LCDPrintString(0, CENTER_ALIGN, "o per disabilitare");
	LCDPrintString(1, CENTER_ALIGN, "un timer");
	delay(2000);
	ClearLCD();
	LCDPrintString(1, CENTER_ALIGN, "Premi Left/Back");
	LCDPrintString(2, CENTER_ALIGN, "per tornare al");
	LCDPrintString(3, CENTER_ALIGN, "Menu Principale");
	delay(2000);
	ClearLCD();
	TakeReleTime();
	while(!ExitAssignReleTimer)
	{		
		TakeReleTime();
		LCDPrintString(0, CENTER_ALIGN, "Assegna il timer");
		LCDPrintString(1, CENTER_ALIGN, "alla presa numero:");
		LCDPrintValue(2, CENTER_ALIGN, ReleNumber+1);
		if(Rele[ReleNumber].HaveTimer)
		{
			LCDPrintString(3, CENTER_ALIGN, "Timer abilitato");
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
					LCDPrintString(2, CENTER_ALIGN, "Timer disabilitato");
					delay(2000);
					ClearLCD();
					Rele[ReleNumber].HaveTimer = false;
					Rele[ReleNumber].TimerTime = 0;
					TimerAssignedCnt--;
				}
				else if(TimerAssignedCnt == 1)
				{
					ClearLCD();
					LCDPrintString(0, CENTER_ALIGN, "Raggiunto il");
					LCDPrintString(1, CENTER_ALIGN, "numero massimo");
					LCDPrintString(2, CENTER_ALIGN, "di timer assegnati");	
					delay(3000);
					ClearLCD();	
					LCDPrintString(0, CENTER_ALIGN, "Disabilitare");
					LCDPrintString(1, CENTER_ALIGN, "un timer");
					LCDPrintString(2, CENTER_ALIGN, "o uscire");	
					delay(3000);
					ClearLCD();						
				}
				else
				{
					ClearLCD();
					LCDPrintString(1, CENTER_ALIGN, "Timer abilitato");
					delay(2000);
					ClearLCD();
					Rele[ReleNumber].HaveTimer = SetTimerRele(ReleNumber);
					ON(ReleNumber);
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







