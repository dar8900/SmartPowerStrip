#include "LCDLib.h"
#include "TimeLib.h"
#include "SmartPowerStrip.h"
#include "EEPROM_Ard.h"
#include "Buttons.h"
#include "Menu.h"
#include "Web.h"

extern TIME_DATE_FORMAT PresentTime;
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
	ClearLCD();
	LCDPrintString(0, CENTER_ALIGN, "Premere Ok/Set");
	LCDPrintString(1, CENTER_ALIGN, "per entrare nel");
	LCDPrintString(2, CENTER_ALIGN, "Menu Principale");
	delay(3000);
	while(!Flag.EnterSetup)
	{
		Flag.EnterSetup = SetupInterrupt();
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
		LCDPrintString(1, CENTER_ALIGN, "Prese attive:");
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
		
		
		delay(100);
	}
	if(Flag.EnterSetup)
	{
		MainMenu();
		Flag.EnterSetup = false;
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
			EnterMenu = false;
		}
		ClearLCD();
		LCDPrintString(2, CENTER_ALIGN, MainMenuItems[Item].MenuTitle);
		TakePresentTime();
		ButtonPress = CheckButtons();
		switch(ButtonPress)
		{
			case BUTTON_UP:
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
				EnterMenu = true;
				break;
			case BUTTON_LEFT:
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
					if(Status == STATUS_ON)
						Status = STATUS_OFF;
					else
						Status = STATUS_ON;
					break;
				case BUTTON_DOWN:
					if(Status == STATUS_ON)
						Status = STATUS_OFF;
					else
						Status = STATUS_ON;
					break;
				case BUTTON_SET:
					ClearLCD();
					if(OldStatus != Status)
						LCDPrintString(2, CENTER_ALIGN, "Valore Salvato");
					else
						LCDPrintString(2, CENTER_ALIGN, "Valore non cambiato");
					Rele[ReleIndx].IsActive = Status;
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
	
}

bool HelpInfo()
{
	
}

bool WiFiInfo() // Per ora mostro solo IP e Hostname, per il client aspettare parte WEB
{
	bool ExitWifiInfo = false;
	short ButtonPress = 0;
	ClearLCD();
	LCDPrintString(0, CENTER_ALIGN, "Wifi Info");
	LCDPrintString(2, CENTER_ALIGN, "Premere Left/Back");
	LCDPrintString(3, CENTER_ALIGN, "per uscire");
	delay(3000);
	ClearLCD();
	while(1)
	{
		ButtonPress = CheckButtons();
		LCDPrintString(0, LEFT_ALIGN, "IP:");
		LCDPrintString(0, RIGHT_ALIGN, WifiIP());
		LCDPrintString(0, LEFT_ALIGN, "Hostname:");
		LCDPrintString(0, RIGHT_ALIGN, HostName);
		switch(ButtonPress)
		{
			case BUTTON_LEFT:
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

