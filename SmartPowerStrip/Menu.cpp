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

extern WIFI_LIST List[];
extern short WifiItemSsid;

MENU_VOICES MainMenuItems[]
{
	{ManualRele		,	"Gestione Manuale"	},
	{ChangeTimeBand	,	"Cambio Banda"		},
	{WifiConnect	,	"Connetti WiFi"		},
	{ChangeTime 	,	"Cambia ora"    	},
	{HelpInfo		,	"Help e Info"		},
	{WiFiInfo		,	"Wifi Info"			},
	{AssignReleTimer,	"Assegna Timer"		},
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
		ButtonPress = NO_PRESS;
	}
	return EnterSetup;
}

void MainScreen(short EnterSetup)
{
	String Time,Date;
	short ReleIndx = 0;
	int TimerMenu = 300; // 30s circa
	// short EnterSetup = NO_PRESS;
	bool InBand = false, ExitFromBand = true;
	ClearLCD();
	CheckReleStatus();
	CheckEvents();
	InBand = CheckBand();
	if(InBand)
	{
		ClearLCD();
		LCDPrintString(TWO, CENTER_ALIGN, "Banda in attivazione");
		delay(2500);
		ClearLCD();
		while(CheckBand())
		{
			if(Flag.WifiActive)
				WifiDisconnect();
			
			if(Flag.IsDisplayOn)
			{
				LCDDisplayOff();
				Flag.IsDisplayOn = false;				
			}
			if(!Flag.AllReleDown)
			{
				TurnOffAllRele();
			}
			for(ReleIndx = RELE_1; ReleIndx < RELE_MAX; ReleIndx++)
			{
				if(Rele[ReleIndx].HaveTimer == true)
				{
					Rele[ReleIndx].HaveTimer = false;
					Rele[ReleIndx].TimerTime = SetTimeVarRele(0,0,0,0);				
				}
			}
		}
		EnterSetup = NO_PRESS;
		ExitFromBand = false;
	}
	else
	{
		if(!ExitFromBand)
		{
			LCDDisplayOn();
			Flag.IsDisplayOn = true;
			LCDPrintString(TWO, CENTER_ALIGN, "Uscita dalla banda");
			delay(2000);
			ClearLCD();
			ReleReStart();
			ExitFromBand = true;
		}
		// EnterSetup = CheckButtons(); 
		// if(EnterSetup == BUTTON_SET)
		// {
			// break;
		// }
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
		Date = String(PresentTime.day) + "/" + String(PresentTime.month) + "/" + String(PresentTime.year);
		LCDPrintString(ONE, LEFT_ALIGN, Time);
		LCDPrintString(ONE, RIGHT_ALIGN, Date);
		// Stato relè
		LCDPrintString(TWO, CENTER_ALIGN, "Prese attive:");
		ShowReleIcons(THREE);
		// Stato wifi
		LCDPrintString(FOUR, LEFT_ALIGN, "Wifi status: ");
		if(Flag.WifiActive)
		{
			LCDMoveCursor(FOUR, 14);
			LCDShowIcon(WIFI_OK);
		}
		else
		{
			LCDMoveCursor(FOUR, 14);
			LCDShowIcon(WIFI_NO);
		}
		
		// GESTIONE PARTE WEB
		
		// TimerMenu--;
		// if(TimerMenu == 0)
		// {
			// ClearLCD();
			// CheckEvents();
			// LCDPrintString(ONE, CENTER_ALIGN, "Premere Ok/Set");
			// LCDPrintString(TWO, CENTER_ALIGN, "per entrare nel");
			// LCDPrintString(THREE, CENTER_ALIGN, "Menu Principale");
			// delay(2000);
			// CheckEvents();
			// TimerMenu = 300;
			// ClearLCD();
		// }
	}
	if(EnterSetup == BUTTON_SET)
	{
		MainMenu();
		EnterSetup = NO_PRESS;		
	}

}

void MainMenu()
{
	bool ExitMainMenu = false;
	bool ReEnterMenu = false;
	short ButtonPress = 0, Item = MANUAL_RELE;
	String Time,Date;
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
			CheckEvents();
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
			CheckEvents();
			ReEnterMenu = false;
		}
		CheckEvents();
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
		Date = String(PresentTime.day) + "/" + String(PresentTime.month) + "/" + String(PresentTime.year);
		LCDPrintString(ONE, LEFT_ALIGN, Time);
		LCDPrintString(ONE, RIGHT_ALIGN, Date);
		LCDPrintString(THREE, CENTER_ALIGN, MainMenuItems[Item].MenuTitle);
		
		ButtonPress = CheckButtons();
		switch(ButtonPress)
		{
			case BUTTON_UP:
				BlinkLed(BUTTON_LED);
				if(Item > MANUAL_RELE)
				{
					Item--;
				}
				else
				{
					Item = MAX_MENU_ITEM - 1;
				}
				ClearLCD();
				break;
			case BUTTON_DOWN:
				BlinkLed(BUTTON_LED);
				if(Item < MAX_MENU_ITEM - 1)
				{
					Item++;					
				}
				else
				{
					Item = MANUAL_RELE;
				}
				ClearLCD();
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
		ButtonPress = NO_PRESS;
		delay(100);
		if(ReEnterMenu)
		{
			MainMenuItems[Item].MenuFunc();
		}
	}
}

bool ManualRele()
{
	short ReleIndx = RELE_1, YesNoChoice = 0;
	bool ReleSetted = false, OnOffAll = false, ExitYesNo = false, ExitAll = false;
	String YesNo[] = {"No", "Si"};
	String SelRele;
	short ButtonPress = 0, Status = 0, OldStatus = 0;
	if(Flag.AllReleUp)
	{
		ClearLCD();
		LCDPrintString(ONE, CENTER_ALIGN, "Vuoi spegnere");
		LCDPrintString(TWO, CENTER_ALIGN, "tutte le prese?");
		delay(1000);		
	}
	else
	{
		ClearLCD();
		LCDPrintString(ONE, CENTER_ALIGN, "Vuoi accendere");
		LCDPrintString(TWO, CENTER_ALIGN, "tutte le prese?");
		delay(1000);
	}
	ClearLCD();
	LCDPrintString(ONE, CENTER_ALIGN, "Premi Ok/Set");
	LCDPrintString(TWO, CENTER_ALIGN, "per confermare");
	while(!ExitYesNo)
	{
		CheckEvents();
		LCDPrintString(THREE, CENTER_ALIGN, YesNo[YesNoChoice]);
		ButtonPress = CheckButtons();
		switch(ButtonPress)
		{
			case BUTTON_UP:
				BlinkLed(BUTTON_LED);
				if(YesNoChoice == 0)
					YesNoChoice = 1;
				else
					YesNoChoice = 0;
				break;
			case BUTTON_DOWN:
				BlinkLed(BUTTON_LED);
				if(YesNoChoice == 0)
					YesNoChoice = 1;
				else
					YesNoChoice = 0;
				break;
			case BUTTON_SET:
				BlinkLed(BUTTON_LED);
				if(YesNoChoice == 0)
				{
					OnOffAll = false;
					ExitYesNo = true;
				}					
				else
				{
					OnOffAll = true;
					ExitYesNo = true;
				}
				break;
			default:
				break;
		}
		ButtonPress = NO_PRESS;
		delay(80);
	}
	if(OnOffAll)
	{
		ClearLCD();
		LCDPrintString(TWO, CENTER_ALIGN, "Attendere...");
		delay(1000);
		if(Flag.AllReleUp)
		{
			TurnOffAllRele();
			for(ReleIndx = RELE_1; ReleIndx < RELE_MAX; ReleIndx++)
			{
				WriteMemory(Rele[ReleIndx].EepromAddr, STATUS_OFF);
			}
			LCDShowPopUp("Tutte Spente");
		}
		else
		{
			TurnOnAllRele();
			LCDShowPopUp("Tutte Accese");
		}
	}
	else
	{
		ClearLCD();
		for(ReleIndx = RELE_1; ReleIndx < RELE_MAX; ReleIndx++)
		{		
			ClearLCD();
			SelRele = "Impostare il Rele ";
			SelRele += String(ReleIndx + 1);
			CheckEvents();
			ReleSetted = false;
			LCDPrintString(TWO, CENTER_ALIGN, SelRele);
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
				CheckEvents();
				ButtonPress = CheckButtons();
				LCDPrintString(TWO, CENTER_ALIGN, SelRele);
				LCDPrintString(2, CENTER_ALIGN, ONOFF[Status]);
				switch(ButtonPress)
				{
					case BUTTON_UP:
						BlinkLed(BUTTON_LED);
						if(Status == STATUS_ON)
							Status = STATUS_OFF;
						else
							Status = STATUS_ON;
						ClearLCD();
						break;
					case BUTTON_DOWN:
						BlinkLed(BUTTON_LED);
						if(Status == STATUS_ON)
							Status = STATUS_OFF;
						else
							Status = STATUS_ON;
						ClearLCD();
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
							Rele[ReleIndx].ActiveTime = SetTimeVarRele(0,0,0,0);
							Rele[ReleIndx].TurnOnTime = SetTimeVarRele(0,0,0,0);
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
						}
						WriteMemory(Rele[ReleIndx].EepromAddr, Status);
						ReleSetted = true;
						delay(1000);
						ClearLCD();
						break;
					case BUTTON_LEFT:
						ExitAll = true;
						break;
					default:
						break;
				}
				delay(50);
				ButtonPress = NO_PRESS;
				if(ExitAll)
					break; // Uscita while
			}	
			if(ExitAll)
				break; // Uscita for
		}
		CheckReleStatus();
	}
}

bool ChangeTimeBand()
{
	bool ExitChangeBand = false, OkBandSet = true;
	short ButtonPress;
    ClearLCD();
	CheckEvents();
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
	CheckEvents();
	while(!ExitChangeBand)
	{
		if(!OkBandSet)
		{
			ClearLCD();
			CheckEvents();
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
		
		CheckEvents();
		ButtonPress = CheckButtons();
		LCDPrintString(ONE, CENTER_ALIGN, "Premi Ok/Set");
		LCDPrintString(TWO, CENTER_ALIGN, "per modificare");
		LCDPrintString(THREE, CENTER_ALIGN, "Premi Left/Back");
		LCDPrintString(FOUR, CENTER_ALIGN, "per uscire");
		switch(ButtonPress)
		{
			case BUTTON_UP:
			case BUTTON_DOWN:
			default:
				break;
			case BUTTON_LEFT:
				BlinkLed(BUTTON_LED);
				ClearLCD();
				LCDPrintString(TWO, CENTER_ALIGN, "Uscita...");
				delay(1500);
				ClearLCD();
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
							LCDPrintString(0, CENTER_ALIGN, "Banda");
							LCDPrintString(1, CENTER_ALIGN, "non");
							LCDPrintString(2, CENTER_ALIGN, "corretta");
							delay(2000);
							ClearLCD();
							CheckEvents();
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
		ButtonPress = NO_PRESS;
	}
}

bool WifiConnect()
{
	short ButtonPress = NO_PRESS;
	short YesNoChoice = 0;
	bool ExitWifiConnect = false, WifiDisconnectChoice = false;
	String YesNo[] = {"Si", "No"};
	if(Flag.WifiActive)
	{
		ClearLCD();
		LCDPrintString(ONE, CENTER_ALIGN, "Sei ancora connesso");
		LCDPrintString(TWO, CENTER_ALIGN, "ad una rete WiFi!");
		LCDPrintString(THREE, CENTER_ALIGN, "Disconnettere?");
		delay(2000);
		while(!ExitWifiConnect)
		{
			CheckEvents();
			LCDPrintString(FOUR, CENTER_ALIGN, YesNo[YesNoChoice]);
			ButtonPress = CheckButtons();
			switch(ButtonPress)
			{
				case BUTTON_UP:
					BlinkLed(BUTTON_LED);
					if(YesNoChoice == 0)
						YesNoChoice = 1;
					else
						YesNoChoice = 0;
					LCDPrintLineVoid(FOUR);
					break;
				case BUTTON_DOWN:
					BlinkLed(BUTTON_LED);
					if(YesNoChoice == 0)
						YesNoChoice = 1;
					else
						YesNoChoice = 0;
						LCDPrintLineVoid(FOUR);
					break;
				case BUTTON_SET:
					BlinkLed(BUTTON_LED);
					if(YesNoChoice == 0)
					{
						ExitWifiConnect = true;
						WifiDisconnectChoice = true;
					}					
					else
					{
						ExitWifiConnect = true;
						WifiDisconnectChoice = false;
					}
					break;
				default:
					break;
			}
			ButtonPress = NO_PRESS;
			delay(80);			
		}
		ClearLCD();
		if(WifiDisconnectChoice)		
			WifiDisconnect();
	}
	else
	{
		WifiInit();
		WebServerInit();		
	}
	return true;
}

bool ChangeTime()
{
	TakePresentTime();
	short Hour = PresentTime.hour, Minute = PresentTime.minute, TimeSM = END_HOUR;
	short ButtonPress = NO_PRESS;
	bool ExitTimeChange = false;
	String MinuteStr;
	ClearLCD();
	while(!ExitTimeChange)
	{
		CheckEvents();
		LCDPrintString(ONE, CENTER_ALIGN, "Cambia orario");
		ButtonPress = CheckButtons();
		switch(TimeSM)
		{
			case END_HOUR:
				LCDPrintString(TWO, CENTER_ALIGN, "Ora:");
				LCDPrintValue(THREE, CENTER_ALIGN, Hour);
				switch(ButtonPress)
				{
					case BUTTON_UP:	
						if(Hour > 0)
							Hour--;
						else
							Hour = HOUR_IN_DAY;
						ClearLCD();
						break;
					case BUTTON_DOWN:
						if(Hour < HOUR_IN_DAY)
							Hour++;
						else
							Hour = 0;
						ClearLCD();
						break;
					case BUTTON_SET:
						TimeSM = END_MINUTE;
						break;
					case BUTTON_LEFT:
					default:
						break;
				}
				break;
			case END_MINUTE:
				LCDPrintString(TWO, CENTER_ALIGN, "Minuti:");
				if(Minute < 10)
				{
					MinuteStr = "0" + String(Minute);
				}
				else
				{
					MinuteStr = String(Minute);
				}
				LCDPrintString(THREE, CENTER_ALIGN, MinuteStr);
				switch(ButtonPress)
				{
					case BUTTON_UP:	
						if(Minute > 0)
							Minute--;
						else
							Minute = MINUTE_IN_HOUR;
						ClearLCD();
						break;
					case BUTTON_DOWN:
						if(Minute < MINUTE_IN_HOUR)
							Minute++;
						else
							Minute = 0;
						ClearLCD();
						break;
					case BUTTON_SET:
						TimeSM = EXIT;
						break;
					case BUTTON_LEFT:
					default:
						break;
				}
				break;
			case EXIT:
				ExitTimeChange = true;
				ClearLCD();
				break;
			default:
				break;
		}
		delay(60);
	}
	TimeAdjust(Hour, Minute);
}


bool HelpInfo()
{
	short NumTimer = 0, ReleIndx = 0, ReleTimer[2];
	short CountTimer = 0;
	String BandTime1, BandTime2, Hour, Minute, Day, Second, TotalTime;
	bool NoTimer = true;
	CheckEvents();
	ClearLCD();
	LCDPrintString(THREE, CENTER_ALIGN, "Info e Aiuto");
	delay(2000);
	CheckEvents();
	ClearLCD();
	LCDPrintString(ONE, CENTER_ALIGN, "Stato Prese:");
	ShowReleIcons(TWO);
	LCDPrintString(THREE, CENTER_ALIGN, "Stato Wifi: ");
	if(Flag.WifiActive)
	{
		LCDMoveCursor(THREE, 16);
		LCDShowIcon(WIFI_OK);
	}
	else
	{
		LCDMoveCursor(THREE, 16);
		LCDShowIcon(WIFI_NO);
	}
	delay(3000);
	CheckEvents();
	ClearLCD();
	for(ReleIndx = RELE_1; ReleIndx < RELE_MAX; ReleIndx++)
	{
		if(Rele[ReleIndx].HaveTimer)
		{	
			if(NumTimer > 1)
				break;
			ReleTimer[NumTimer] = ReleIndx + 1;
			NumTimer++;
			NoTimer = false;
		}
		else
		{
			if(ReleIndx == RELE_MAX-1 && NoTimer)
			{
				break;
			}
		}
	}
	CheckEvents();
	LCDPrintString(ONE, LEFT_ALIGN, "Timer attivi:");
	if(!NoTimer)
	{
		LCDPrintValue(ONE, 15, NumTimer);
		LCDPrintString(TWO, LEFT_ALIGN, "Prese associate:");
		LCDPrintValue(THREE, 5, ReleTimer[0]);
		if(ReleTimer[1] <= 8)
			LCDPrintValue(THREE, 15, ReleTimer[1]);				
	}
	else
	{
		LCDPrintString(TWO, CENTER_ALIGN, "Nessuno");
	}
	delay(3000);
	CheckEvents();
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
	delay(2000);
	CheckEvents();
	ClearLCD();
	LCDPrintString(ONE, CENTER_ALIGN, "Tempo accensione");
	for(ReleIndx = RELE_1; ReleIndx < RELE_MAX; ReleIndx++)
	{
		CheckEvents();
		LCDPrintString(TWO, 6, "presa: ");
		LCDPrintValue(TWO, 14, ReleIndx+1);
		if(Rele[ReleIndx].IsActive)
		{
			short TimerShowTotalTime = 30;
			while(TimerShowTotalTime > 0)
			{
				CheckEvents();
				Day = String(Rele[ReleIndx].ActiveTime.day);
				Hour = String(Rele[ReleIndx].ActiveTime.hour);
				Minute = String(Rele[ReleIndx].ActiveTime.minute);
				TotalTime = Day + "g " + Hour + "h " + Minute + "m";
				LCDPrintString(THREE, CENTER_ALIGN, TotalTime);		
				TimerShowTotalTime--;
				delay(100);
			}
			LCDPrintLineVoid(THREE);
		}
		else
		{
			LCDPrintString(THREE, CENTER_ALIGN, "Non Attiva");
			delay(1000);
			LCDPrintLineVoid(THREE);
		}
		
	}
	ClearLCD();
	LCDPrintString(THREE, CENTER_ALIGN, "Uscita...");
	delay(2000);
	CheckEvents();
	ClearLCD();
	return true;
}

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
	CheckEvents();
	ClearLCD();
	while(1)
	{		
		CheckEvents();
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
		LCDPrintString(TWO, LEFT_ALIGN, "SSID:");
		LCDPrintString(TWO, RIGHT_ALIGN, List[WifiItemSsid].Ssid);
		LCDPrintString(THREE, LEFT_ALIGN, "Hostname:");
		LCDPrintString(THREE, RIGHT_ALIGN, HostName);
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
		ButtonPress = NO_PRESS;
		if(ExitWifiInfo)
			break;
		delay(80);
	}
}

bool AssignReleTimer()
{
	short ButtonPress, ReleNumber = RELE_1, TimerAssignedCnt = 0;
	bool ExitAssignReleTimer = false;
	ClearLCD();
	CheckEvents();
	if(Flag.AllReleDown)
	{
		LCDPrintString(TWO, CENTER_ALIGN, "Non ci sono");
		LCDPrintString(THREE, CENTER_ALIGN, "prese attive!");
		delay(2000);
		ClearLCD();
	}
	else
	{
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
		CheckEvents();
		while(!ExitAssignReleTimer)
		{		
			CheckEvents();
			if(Rele[ReleNumber].IsActive)
			{
				LCDPrintString(ONE, CENTER_ALIGN, "Assegna il timer");
				LCDPrintString(TWO, CENTER_ALIGN, "alla presa numero:");
				LCDPrintValue(THREE, CENTER_ALIGN, ReleNumber + 1);
			}
			else
			{
				LCDPrintString(ONE, CENTER_ALIGN, "Presa numero:");
				LCDPrintValue(TWO, CENTER_ALIGN, ReleNumber + 1);
				LCDPrintString(THREE, CENTER_ALIGN, "spenta");
			}
			if(Rele[ReleNumber].HaveTimer)
			{
				LCDPrintString(FOUR, CENTER_ALIGN, "Timer abilitato");
			}
			else
			{
				LCDPrintLineVoid(FOUR);
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
					ClearLCD();
					break;
				case BUTTON_DOWN:
					BlinkLed(BUTTON_LED);
					if(ReleNumber < RELE_MAX - 1)
						ReleNumber++;
					else
						ReleNumber = RELE_1;
					ClearLCD();
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
						Rele[ReleNumber].TimerTime = SetTimeVarRele(0,0,0,0);
						TimerAssignedCnt--;
					}
					else if(TimerAssignedCnt == 2)
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
					else if(!Rele[ReleNumber].HaveTimer && Rele[ReleNumber].IsActive)
					{
						ClearLCD();
						LCDPrintString(TWO, CENTER_ALIGN, "Timer abilitato");
						delay(2000);
						ClearLCD();
						Rele[ReleNumber].HaveTimer = SetTimerRele(ReleNumber);
						ON(ReleIdx2Pin(ReleNumber));
						Flag.AllReleDown = false;
						CheckEvents();
						TimerAssignedCnt++;	
					}

					break;
				case BUTTON_LEFT:
					ExitAssignReleTimer = true;
					break;
				default:
					break;
			}
			ButtonPress = NO_PRESS;
			delay(60);
		}
	}
	return true;
}







