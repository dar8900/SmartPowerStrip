#include "LCDLib.h"
#include "TimeLib.h"
#include "SmartPowerStrip.h"
#include "EEPROM_Ard.h"
#include "I2CNano.h"
#include "Menu.h"
#include "Web.h"
#include "Band.h"
#include "Rele.h"

#define  TIMER_REFRESH_MEASURE	30

extern TIME_DATE_FORMAT PresentTime;
extern BAND_FORMAT Band;
extern RELE Rele[];
extern FLAGS Flag;
extern String HostName;
extern String VersionValue;
extern String VersionDate;

uint16_t TimerRefreshMenu;
short TimerClientConnected = DELAY_CLIENT_CONNECTION;
bool ExitFromBand = true;

const MENU_VOICES MainMenuItems[]
{
	{ManualRele		,	"Gestione Manuale"	},
	{ChangeTimeBand	,	"Cambio Banda"		},
	{WifiConnect	,	"Connetti WiFi"		},
	{HelpInfo		,	"Help e Info"		},
	{ShowMeasures	,	"Misure"   		    },
	{WiFiInfo		,	"Wifi Info"			},
	{AssignReleTimer,	"Assegna Timer"		},
	{Setup       	,	"Impostazioni"    	},
};

const MENU_VOICES GeneralSetup[]
{
	{ChangeTime			, "Cambio ora"      },
	{ChangeTimerDisplay , "Timer display on"},
	{TurnOffWifi        , "Spegni Wifi"     },
	{ChangeUdmEnergy    , "UDM Energia"     },
};

// WiFi.mode(WIFI_OFF);

const DELAY_TIMER_S TimerDalays[]
{
	{1   , "Sempre Acceso"},
	{6000, "10 min"},
	{3000, "5 min"},
	{1000, "1 min"},
	{500,  "30 sec"},
	{166,  "10 sec"},
};

const UDM_ENERGY_MENU UdmEnergyScale[MAX_UDM_ITEM] = 
{
	{3600.0,    "Wh" },
	{60.0  ,   "Wm" },
	{1.0  , "Ws" },	
};

const FORMAT_ENERGY TabFormat[] = 
{
	{0.001,             1000.0, "m"},
	{0.01,              1000.0, "m"},
	{0.1,               1000.0, "m"},	
	{1.0,                    1,  ""},
	{10.0,                   1,  ""},
	{100.0,                  1,  ""},
	{1000.0,             0.001, "k"},
	{10000.0,            0.001, "k"},
	{100000.0,           0.001, "k"},
	{1000000.0,       0.000001, "M"},
	{10000000.0,      0.000001, "M"},
	{100000000.0,     0.000001, "M"},
	{1000000000.0, 0.000000001, "G"},
};


static const String ONOFF[] = {"Off", "On"};

static uint8_t SearchFormatRange(float ValueToFormat)
{
	uint8_t TabLenght = 13;
	uint8_t Range = 0;
	for(Range = 0; Range < TabLenght; Range++)
	{
		if(Range == TabLenght - 1)
			break;
		if(ValueToFormat > TabFormat[Range].RangeValue && ValueToFormat < TabFormat[Range + 1].RangeValue)
		{
			break;
		}
	}
	return Range;
}

void MenuInit()
{
	short Delay = MAIN_SCREEN_TIMER_DEFAULT;
	short EnergyUdm;
	ReadMemory(TIMER_BACKLIGHT_ADDR, 1, &Delay);
	if(Delay > MAX_DELAY_TIMERS)
	{
		Delay = TEN_MINUTES;
		WriteMemory(TIMER_BACKLIGHT_ADDR, Delay);
		TimerRefreshMenu = TimerDalays[Delay].DelayValue;
	}
	else
	{
		TimerRefreshMenu = TimerDalays[Delay].DelayValue;
	}
	ClearLCD();
	EepromUpdate(UDM_ENERGY_ADDR, WATT_ORA);
	LCDPrintString(ONE, CENTER_ALIGN, "Retroilluminazione");
	LCDPrintString(TWO, CENTER_ALIGN, "impostata a:");
	LCDPrintString(THREE, CENTER_ALIGN, TimerDalays[Delay].DelayStr);
	delay(DELAY_MENU_MSG);
	ClearLCD();
	return;
}

void MainScreen(short EnterSetup)
{
	short ReleIndx = 0;
	CheckReleStatus();
	CheckEvents();
	short RefreshDelayItem;
	Flag.InBand = CheckBand();
	if(Flag.InBand)
	{
		ClearLCD();
		LCDPrintString(TWO, CENTER_ALIGN, "Banda in attivazione");
		delay(DELAY_INFO_MSG);
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
			delay(WHILE_LOOP_DELAY);
		}
		EnterSetup = NO_PRESS;
		ExitFromBand = false;
		Flag.InBand = false;
	}
	else
	{
		if(!ExitFromBand)
		{
			LCDDisplayOn();
			Flag.IsDisplayOn = true;
			LCDPrintString(TWO, CENTER_ALIGN, "Uscita dalla banda");
			delay(DELAY_INFO_MSG);
			ClearLCD();
			BandInit();
			ReleReStart();
			ExitFromBand = true;
		}
		// Scrittura data e ora
		ShowDateTime(ONE);
		// Icona WebPage aperta
		if(Flag.ClientConnected)
		{
			TimerClientConnected--;
			if(TimerClientConnected > 0)
			{
				ShowClientConnected(ONE, 9, true);
			}
			else
			{
				TimerClientConnected = DELAY_CLIENT_CONNECTION;
				Flag.ClientConnected = false;
				ShowClientConnected(ONE, 9, false);
			}
		}
		// Stato relè
		LCDPrintString(TWO, CENTER_ALIGN, "Prese attive:");
		ShowReleIcons(THREE);
		// Stato wifi
		LCDPrintString(FOUR, LEFT_ALIGN, "Stato Wifi: ");
		ShowWifiStatus(FOUR, 14, Flag.WifiActive);
		ScreenTimerRefresh();
	}
	if(EnterSetup == BUTTON_SET)
	{
		if(!Flag.IsDisplayOn)
		{
			LCDDisplayOn();
			Flag.IsDisplayOn = true;
		}
		MainMenu();
		ClearLCD();
		EnterSetup = NO_PRESS;
	}
	else if(EnterSetup != NO_PRESS)
	{
		if(!Flag.IsDisplayOn)
		{
			Flag.IsDisplayOn = true;
			LCDDisplayOn();
			EnterSetup = NO_PRESS;
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
	delay(DELAY_MENU_MSG);
	ClearLCD();
	LCDPrintString(ONE  , CENTER_ALIGN, "Premere Left/Back");
	LCDPrintString(TWO  , CENTER_ALIGN, "per tornare alla");
	LCDPrintString(THREE, CENTER_ALIGN, "schermata iniziale");
	delay(DELAY_MENU_MSG);
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
			delay(DELAY_MENU_MSG);
			ClearLCD();
			LCDPrintString(ONE  , CENTER_ALIGN, "Premere Left/Back");
			LCDPrintString(TWO  , CENTER_ALIGN, "per tornare alla");
			LCDPrintString(THREE, CENTER_ALIGN, "schermata iniziale");
			delay(DELAY_MENU_MSG);
			ClearLCD();
			CheckEvents();
			ReEnterMenu = false;
		}
		CheckEvents();
		// Scrittura data e ora
		ShowDateTime(ONE);
		// Icona WebPage aperta
		if(Flag.ClientConnected)
		{
			TimerClientConnected--;
			if(TimerClientConnected > 0)
			{
				ShowClientConnected(ONE, 9, true);
			}
			else
			{
				TimerClientConnected = DELAY_CLIENT_CONNECTION;
				Flag.ClientConnected = false;
				ShowClientConnected(ONE, 9, false);
			}
		}
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
				ClearLCDLine(THREE);
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
				ClearLCDLine(THREE);
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
		delay(WHILE_LOOP_DELAY);
		if(ReEnterMenu)
		{
			MainMenuItems[Item].MenuFunc();
		}
	}
}

bool Setup()
{
	bool ExitSetup = false;
	short ButtonPress = NO_PRESS;
	short SetupItem = CHANGE_TIME;
	ClearLCD();
	while(!ExitSetup)
	{
		CheckEvents();
		ShowDateTime(ONE);
		if(Flag.ClientConnected)
		{
			TimerClientConnected--;
			if(TimerClientConnected > 0)
			{
				ShowClientConnected(ONE, 9, true);
			}
			else
			{
				TimerClientConnected = DELAY_CLIENT_CONNECTION;
				Flag.ClientConnected = false;
				ShowClientConnected(ONE, 9, false);
			}
		}
		LCDPrintString(THREE, CENTER_ALIGN, GeneralSetup[SetupItem].MenuTitle);
		ButtonPress = CheckButtons();
		switch(ButtonPress)
		{
			case BUTTON_UP:
				BlinkLed(BUTTON_LED);
				if(SetupItem > 0)
				{
					SetupItem--;
				}
				else
				{
					SetupItem = MAX_SETUP_ITEM - 1;
				}
				ClearLCD();
				break;
			case BUTTON_DOWN:
				BlinkLed(BUTTON_LED);
				if(SetupItem < MAX_SETUP_ITEM - 1)
				{
					SetupItem++;
				}
				else
				{
					SetupItem = 0;
				}
				ClearLCD();
				break;
			case BUTTON_SET:
				BlinkLed(BUTTON_LED);
				GeneralSetup[SetupItem].MenuFunc();
				ClearLCD();
				break;
			case BUTTON_LEFT:
				BlinkLed(BUTTON_LED);
				ClearLCD();
				ExitSetup = true;
				break;
			default:
				break;
		}
		delay(WHILE_LOOP_DELAY);
	}
}

bool ManualRele()
{
	short ReleIndx = RELE_1;
	bool ReleSetted = false, OnOffAll = false,  ExitAll = false;
	String SelRele;
	short ButtonPress = 0, Status = 0, OldStatus = 0;
	if(Flag.AllReleUp)
	{
		ClearLCD();
		LCDPrintString(ONE, CENTER_ALIGN, "Vuoi spegnere");
		LCDPrintString(TWO, CENTER_ALIGN, "tutte le prese?");
		delay(DELAY_MENU_MSG);
	}
	else
	{
		ClearLCD();
		LCDPrintString(ONE, CENTER_ALIGN, "Vuoi accendere");
		LCDPrintString(TWO, CENTER_ALIGN, "tutte le prese?");
		delay(DELAY_MENU_MSG);
	}
	OnOffAll = CheckYesNo();
	if(OnOffAll)
	{
		ClearLCD();
		LCDPrintString(TWO, CENTER_ALIGN, "Attendere...");
		delay(DELAY_MENU_MSG);
		if(Flag.AllReleUp)
		{
			TurnOffAllRele();
			for(ReleIndx = RELE_1; ReleIndx < RELE_MAX; ReleIndx++)
			{
				SaveReleStatus(ReleIndx, STATUS_OFF);
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
			SelRele = "Impostare la presa ";
			SelRele += String(ReleIndx + 1);
			CheckEvents();
			ReleSetted = false;
			LCDPrintString(TWO, CENTER_ALIGN, SelRele);
			if(Rele[ReleIndx].IsActive)
			{
				LCDPrintString(THREE, CENTER_ALIGN, ONOFF[STATUS_ON]);
				Status = STATUS_ON;
			}
			else
			{
				LCDPrintString(THREE, CENTER_ALIGN, ONOFF[STATUS_OFF]);
				Status = STATUS_OFF;
			}
			OldStatus = Status;
			while(!ReleSetted)
			{
				CheckEvents();
				ButtonPress = CheckButtons();
				LCDPrintString(TWO, CENTER_ALIGN, SelRele);
				LCDPrintString(THREE, CENTER_ALIGN, ONOFF[Status]);
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
							LCDPrintString(THREE, CENTER_ALIGN, "Valore Salvato");
						else
							LCDPrintString(THREE, CENTER_ALIGN, "Valore non cambiato");
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
							Rele[ReleIndx].ActiveTime.minute = 0;
						}
						SaveReleStatus(ReleIndx, Status);
						ReleSetted = true;
						delay(DELAY_MENU_MSG);
						ClearLCD();
						break;
					case BUTTON_LEFT:
						ExitAll = true;
						break;
					default:
						break;
				}
				delay(WHILE_LOOP_DELAY);
				ButtonPress = NO_PRESS;
				if(ExitAll)
					break; // Uscita while
			}
			if(ExitAll)
				break; // Uscita for
		}
		CheckReleStatus();
	}
	return true;
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
	delay(DELAY_INFO_MSG);
	ClearLCD();
	LCDPrintString(ONE  , CENTER_ALIGN, "Premere Ok/Set");
	LCDPrintString(TWO  , CENTER_ALIGN, "per cambiare");
	LCDPrintString(THREE, CENTER_ALIGN, "la banda o");
	LCDPrintString(FOUR , CENTER_ALIGN, "diabilitarla");
	delay(DELAY_INFO_MSG);
	ClearLCD();
	LCDPrintString(ONE  , CENTER_ALIGN, "Premere Left/Back");
	LCDPrintString(TWO  , CENTER_ALIGN, "per tornare al");
	LCDPrintString(THREE, CENTER_ALIGN, "Menu Principale");
	delay(DELAY_INFO_MSG);
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
			delay(DELAY_INFO_MSG);
			ClearLCD();
			LCDPrintString(ONE  , CENTER_ALIGN, "Premere Left/Back");
			LCDPrintString(TWO  , CENTER_ALIGN, "per tornare al");
			LCDPrintString(THREE, CENTER_ALIGN, "Menu Principale");
			delay(DELAY_INFO_MSG);

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
				delay(DELAY_INFO_MSG);
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
							LCDPrintString(ONE, CENTER_ALIGN, "Banda");
							LCDPrintString(TWO, CENTER_ALIGN, "non corretta");
							delay(DELAY_INFO_MSG);
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
		delay(WHILE_LOOP_DELAY);
		ButtonPress = NO_PRESS;
	}
	return true;
}

bool WifiConnect()
{
	bool WifiDisconnectChoice = false;
	if(Flag.WifiActive)
	{
		ClearLCD();
		LCDPrintString(ONE, CENTER_ALIGN, "Sei ancora connesso");
		LCDPrintString(TWO, CENTER_ALIGN, "ad una rete WiFi!");
		LCDPrintString(THREE, CENTER_ALIGN, "Disconnettere?");
		CheckEvents();
		WifiDisconnectChoice = CheckYesNo();
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

static bool BackPressed()
{
	short ButtonPress = NO_PRESS;
	short TimerPressing = 50; // 0.5 sec
	bool Pressed = false;
	while(TimerPressing > 0)
	{
		ButtonPress = CheckButtons();
		TimerPressing--;
		if(ButtonPress == BUTTON_LEFT)
		{
			Pressed = true;
			break;
		}
		delay(10);
	}
	return Pressed;
}


bool HelpInfo()
{
	short NumTimer = 0, ReleIndx = 0, ReleTimer[2];
	String BandTime1, BandTime2, Hour, Minute, Day, Second, TotalTime;
	bool NoTimer = true;
	CheckEvents();
	ClearLCD();
	LCDPrintString(ONE, CENTER_ALIGN, "Info e Aiuto");
	LCDPrintString(THREE, CENTER_ALIGN, "Premere Left/Back");
	LCDPrintString(FOUR, CENTER_ALIGN, "per uscire");
	delay(DELAY_INFO_MSG);
	CheckEvents();
	ClearLCD();
	while(1)
	{
		if(BackPressed())
			break;
		LCDPrintString(ONE, CENTER_ALIGN, "Stato Prese:");
		ShowReleIcons(TWO);
		LCDPrintString(THREE, CENTER_ALIGN, "Stato Wifi: ");
		ShowWifiStatus(THREE, 16, Flag.WifiActive);
		delay(DELAY_INFO_MSG);
		CheckEvents();
		if(BackPressed())
			break;
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
		if(BackPressed())
			break;
		delay(DELAY_INFO_MSG);
		CheckEvents();
		if(BackPressed())
			break;
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
		{
			LCDPrintString(TWO, CENTER_ALIGN, "Non Settata");
		}
		if(BackPressed())
			break;
		delay(DELAY_INFO_MSG);
		CheckEvents();
		if(BackPressed())
			break;
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
				ClearLCDLine(THREE);
			}
			else
			{
				LCDPrintString(THREE, CENTER_ALIGN, "Non Attiva");
				delay(DELAY_MENU_MSG);
				ClearLCDLine(THREE);
			}
			if(BackPressed())
				break;
		}
		delay(DELAY_INFO_MSG);
		if(BackPressed())
			break;
		ClearLCD();
		CheckEvents();
		LCDPrintString(ONE, CENTER_ALIGN, "Firmware version:");
		LCDPrintString(TWO, CENTER_ALIGN, VersionValue);
		LCDPrintString(THREE, CENTER_ALIGN, "Release Date:");
		LCDPrintString(FOUR, CENTER_ALIGN, VersionDate);
		if(BackPressed())
			break;
		delay(DELAY_INFO_MSG);
		CheckEvents();
		ClearLCD();
		if(BackPressed())
			break;
	}
	ClearLCD();
	LCDPrintString(THREE, CENTER_ALIGN, "Uscita...");
	delay(DELAY_INFO_MSG);
	ClearLCD();
    CheckEvents();
	
	return true;
}

bool ShowMeasures()
{
	uint16_t TimerDisplay = 3000; // 60s con delay 10ms
	uint8_t FormatRange = 0;
	short TimerRefreshMeasure = TIMER_REFRESH_MEASURE; 
	short ButtonPress = NO_PRESS;
	short UdmEnergy = 0;
	String EnergyStr;
	String CurrentStr;
	float EnergyScaled = 0.0;
	float CurrentScaled = 0.0;
	bool ExitShowEnergy = false, FormatEnergy = false;
	ReadMemory(UDM_ENERGY_ADDR, 1, &UdmEnergy);
	ClearLCD();
	if(!Flag.IsDisplayOn)
	{
		LCDDisplayOn();
		Flag.IsDisplayOn = true;
	}
	while(!ExitShowEnergy)
	{
		LCDPrintString(ONE, CENTER_ALIGN, "Corrente Misurata:");
		LCDPrintString(THREE, CENTER_ALIGN, "Energia Misurata:");
		if(TimerRefreshMeasure == TIMER_REFRESH_MEASURE)
		{
			ClearLCDLine(TWO);
			ClearLCDLine(FOUR);
			CurrentStr = CurrentValueStr();
			EnergyStr = EnergyValueStr();
			CurrentScaled = CurrentStr.toFloat();
			FormatRange = SearchFormatRange(CurrentScaled);
			CurrentScaled *= TabFormat[FormatRange].FormatFactor;
			CurrentStr = String(CurrentScaled);
			CurrentStr += " " + TabFormat[FormatRange].Prefix + "A";
			switch(UdmEnergy)
			{
				case WATT_MINUTO:
					EnergyScaled = ((EnergyStr.toFloat()) / UdmEnergyScale[UdmEnergy].UdmValue);
					FormatRange = SearchFormatRange(EnergyScaled);
					EnergyScaled *= TabFormat[FormatRange].FormatFactor;
					EnergyStr = String(EnergyScaled);
					break;
				case WATT_SECONDO:
					EnergyScaled = ((EnergyStr.toFloat()) / UdmEnergyScale[UdmEnergy].UdmValue);
					FormatRange = SearchFormatRange(EnergyScaled);
					EnergyScaled *= TabFormat[FormatRange].FormatFactor;
					EnergyStr = String(EnergyScaled);
					break;
				case WATT_ORA:
					EnergyScaled = ((EnergyStr.toFloat()) / UdmEnergyScale[UdmEnergy].UdmValue);
					FormatRange = SearchFormatRange(EnergyScaled);
					EnergyScaled *= TabFormat[FormatRange].FormatFactor;
					EnergyStr = String(EnergyScaled);
					break;
				default:
					break;
			}
			EnergyStr +=  " " + TabFormat[FormatRange].Prefix + UdmEnergyScale[UdmEnergy].UdmStr;
			LCDPrintString(TWO, CENTER_ALIGN, CurrentStr);
			LCDPrintString(FOUR, CENTER_ALIGN, EnergyStr);
		}
		ButtonPress = CheckButtons();
		switch(ButtonPress)
		{
			case BUTTON_UP:
			case BUTTON_DOWN:
			case BUTTON_SET:
				BlinkLed(BUTTON_LED);
				TimerDisplay = 3000;
				if(!Flag.IsDisplayOn)
				{
					LCDDisplayOn();
					Flag.IsDisplayOn = true;
				}
				break;
			case BUTTON_LEFT:
				BlinkLed(BUTTON_LED);
				ExitShowEnergy = true;
				if(!Flag.IsDisplayOn)
				{
					TimerDisplay = 3000;
					LCDDisplayOn();
					Flag.IsDisplayOn = true;
				}
				break;
		}
		if(TimerDisplay > 0)
			TimerDisplay--;
		if(TimerDisplay == 0 && Flag.IsDisplayOn)
		{
			LCDDisplayOff();
			Flag.IsDisplayOn = false;
		}
		TimerRefreshMeasure--;
		if(TimerRefreshMeasure == 0)
		{
			TimerRefreshMeasure = TIMER_REFRESH_MEASURE;
		}
		delay(10);
	}
	return true;
}

bool WiFiInfo()
{
	bool ExitWifiInfo = false;
	short ButtonPress = 0;
	short WifiItemSsid = 0;
	short RefreshSignal = 150;
	ClearLCD();
	LCDPrintString(ONE  , CENTER_ALIGN, "Wifi Info");
	LCDPrintString(TWO  , CENTER_ALIGN, "Premere Left/Back");
	LCDPrintString(THREE, CENTER_ALIGN, "per tornare al");
	LCDPrintString(FOUR , CENTER_ALIGN, "Menu Principale");
	delay(DELAY_MENU_MSG);
	ReadMemory(WIFI_SSID_ADDR, 1, &WifiItemSsid);
	CheckEvents();
	ClearLCD();
	while(!ExitWifiInfo)
	{
		CheckEvents();
		if(Flag.WifiActive)
		{
			LCDPrintString(ONE, LEFT_ALIGN, "IP:");
			LCDPrintString(TWO, LEFT_ALIGN, "SSID:");
			LCDPrintString(TWO, RIGHT_ALIGN, MyNetworkList[WifiItemSsid].Ssid);
			LCDPrintString(THREE, LEFT_ALIGN, "Hostname:");
			LCDPrintString(THREE, RIGHT_ALIGN, HostName);
			LCDPrintString(ONE, RIGHT_ALIGN, WifiIP());
			LCDPrintString(FOUR, LEFT_ALIGN, "Segnale:");
			// Ogni 4.5s
			if(RefreshSignal == 150)
			{
				ClearLCDLine(FOUR);
				LCDPrintString(FOUR, LEFT_ALIGN, "Segnale:");
				LCDPrintString(FOUR, RIGHT_ALIGN, GetWifiSignalPower());
				if(!Flag.WifiActive)
				{
					delay(DELAY_MENU_MSG);
					ClearLCD();
				}
				RefreshSignal = 0;
			}

		}
		else
		{
			LCDPrintString(TWO, CENTER_ALIGN, "Nessuna rete");
			LCDPrintString(THREE, CENTER_ALIGN, "associata");
		}
		ButtonPress = CheckButtons();
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
		RefreshSignal++;
		delay(WHILE_LOOP_DELAY);
	}
	return true;
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
		delay(DELAY_INFO_MSG);
		ClearLCD();
	}
	else
	{
		LCDPrintString(TWO, CENTER_ALIGN, "Assegna un timer");
		LCDPrintString(THREE, CENTER_ALIGN, "a massimo 2 prese");
		delay(DELAY_MENU_MSG);
		ClearLCD();
		LCDPrintString(TWO, CENTER_ALIGN, "Premi Up o Down");
		LCDPrintString(THREE, CENTER_ALIGN, "per scegliere");
		LCDPrintString(FOUR, CENTER_ALIGN, "la presa");
		delay(DELAY_MENU_MSG);
		ClearLCD();
		LCDPrintString(ONE, CENTER_ALIGN, "Premi Ok/Set");
		LCDPrintString(TWO, CENTER_ALIGN, "per confermare");
		LCDPrintString(THREE, CENTER_ALIGN, "o per disabilitare");
		LCDPrintString(FOUR, CENTER_ALIGN, "un timer");
		delay(DELAY_MENU_MSG);
		ClearLCD();
		LCDPrintString(TWO, CENTER_ALIGN, "Premi Left/Back");
		LCDPrintString(THREE, CENTER_ALIGN, "per tornare al");
		LCDPrintString(FOUR, CENTER_ALIGN, "Menu Principale");
		delay(DELAY_MENU_MSG);
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
				ClearLCDLine(FOUR);
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
						delay(DELAY_MENU_MSG);
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
						delay(DELAY_INFO_MSG);
						ClearLCD();
						LCDPrintString(ONE, CENTER_ALIGN, "Disabilitare");
						LCDPrintString(TWO, CENTER_ALIGN, "un timer");
						LCDPrintString(THREE, CENTER_ALIGN, "o uscire");
						delay(DELAY_INFO_MSG);
						ClearLCD();
					}
					else if(!Rele[ReleNumber].HaveTimer && Rele[ReleNumber].IsActive)
					{
						ClearLCD();
						LCDPrintString(TWO, CENTER_ALIGN, "Timer abilitato");
						delay(DELAY_INFO_MSG);
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
			delay(WHILE_LOOP_DELAY);
		}
	}
	return true;
}

bool ChangeTimerDisplay()
{
	short ButtonPress = NO_PRESS;
	short DelayItem = ALWAYS_ON;
	bool ExitchangeDelay = false;
	ClearLCD();
	LCDPrintString(ONE, CENTER_ALIGN, "Scegliere il");
	LCDPrintString(TWO, CENTER_ALIGN, "timer per la");
	LCDPrintString(THREE, CENTER_ALIGN, "retroilluminazione: ");
	ReadMemory(TIMER_BACKLIGHT_ADDR, 1, &DelayItem);
	while(!ExitchangeDelay)
	{
		LCDPrintString(FOUR, CENTER_ALIGN, TimerDalays[DelayItem].DelayStr);
		ButtonPress	= CheckButtons();
		switch(ButtonPress)
		{
			case BUTTON_UP:
				BlinkLed(BUTTON_LED);
				if(DelayItem > ALWAYS_ON)
					DelayItem--;
				else
					DelayItem = MAX_DELAY_TIMERS - 1;
				ClearLCDLine(FOUR);
				break;
			case BUTTON_DOWN:
				BlinkLed(BUTTON_LED);
				if(DelayItem < MAX_DELAY_TIMERS - 1)
					DelayItem++;
				else
					DelayItem = ALWAYS_ON;
					ClearLCDLine(FOUR);
				break;
			case BUTTON_SET:
				BlinkLed(BUTTON_LED);
				WriteMemory(TIMER_BACKLIGHT_ADDR, DelayItem);
				TimerRefreshMenu = TimerDalays[DelayItem].DelayValue;
				ExitchangeDelay = true;
				break;
			case BUTTON_LEFT:
				ExitchangeDelay = true;
			default:
				break;
		}
		delay(WHILE_LOOP_DELAY);
	}
	return true;
}

bool TurnOffWifi()
{
	bool TurnOff = false;
	ClearLCD();
	LCDPrintString(TWO, CENTER_ALIGN, "Spegnere il wifi?");
	TurnOff = CheckYesNo();
	if(TurnOff)
	{
		WifiTurnOff();
	}
	return true;
}

bool ChangeUdmEnergy()
{
	bool ExitChangeUdm = false;
	short ButtonPress = NO_PRESS;
	short UdmEnergyItem, OldUdmEnergyItem;
	ReadMemory(UDM_ENERGY_ADDR, 1, &UdmEnergyItem);
	OldUdmEnergyItem = UdmEnergyItem;
	ClearLCD();
	while(!ExitChangeUdm)
	{
		LCDPrintString(ONE, CENTER_ALIGN, "Visualizzazione");
		LCDPrintString(TWO, CENTER_ALIGN, "misura energia in:");
		LCDPrintString(THREE, CENTER_ALIGN, UdmEnergyScale[UdmEnergyItem].UdmStr);
		ButtonPress = CheckButtons();
		switch(ButtonPress)
		{
			case BUTTON_UP:
				BlinkLed(BUTTON_LED);
				if(UdmEnergyItem > 0)
					UdmEnergyItem--;
				else
					UdmEnergyItem = MAX_UDM_ITEM - 1;
				ClearLCDLine(THREE);
				break;
			case BUTTON_DOWN:
				BlinkLed(BUTTON_LED);
				if(UdmEnergyItem < MAX_UDM_ITEM - 1)
					UdmEnergyItem++;
				else
					UdmEnergyItem = WATT_ORA;
				ClearLCDLine(THREE);
				break;
			case BUTTON_LEFT:
				BlinkLed(BUTTON_LED);
				ExitChangeUdm = true;
				break;
			case BUTTON_SET:
				BlinkLed(BUTTON_LED);
				if(UdmEnergyItem != OldUdmEnergyItem)
				{
					ClearLCD();
					LCDPrintString(TWO, CENTER_ALIGN, "Settato!");
					WriteMemory(UDM_ENERGY_ADDR, UdmEnergyItem);
					delay(DELAY_INFO_MSG);
				}
				else
				{
					ClearLCD();
					LCDPrintString(TWO, CENTER_ALIGN, "Uguale a prima");
					EepromUpdate(UDM_ENERGY_ADDR, UdmEnergyItem);
					delay(DELAY_INFO_MSG);					
				}
				ExitChangeUdm = true;
				break;
			default:
				break;
		}
		ButtonPress = NO_PRESS;
		delay(WHILE_LOOP_DELAY);
	}
	ClearLCD();
}

bool CheckYesNo()
{
	bool Exit = false, Choice = false;
	String YesNo[] = {"Si", "No"};
	short ButtonPress = NO_PRESS;
	short YesNoChoice = NO;
	while(!Exit)
	{
		CheckEvents();
		LCDPrintString(FOUR, CENTER_ALIGN, YesNo[YesNoChoice]);
		ButtonPress = CheckButtons();
		switch(ButtonPress)
		{
			case BUTTON_UP:
				BlinkLed(BUTTON_LED);
				if(YesNoChoice == YES)
					YesNoChoice = NO;
				else
					YesNoChoice = YES;
				ClearLCDLine(FOUR);
				break;
			case BUTTON_DOWN:
				BlinkLed(BUTTON_LED);
				if(YesNoChoice == YES)
					YesNoChoice = NO;
				else
					YesNoChoice = YES;
					ClearLCDLine(FOUR);
				break;
			case BUTTON_SET:
				BlinkLed(BUTTON_LED);
				if(YesNoChoice == YES)
				{
					Exit = true;
					Choice = true;
				}
				else
				{
					Exit = true;
					Choice = false;
				}
				break;
			case BUTTON_LEFT:
			default:
				break;
		}
		ButtonPress = NO_PRESS;
		delay(WHILE_LOOP_DELAY);
	}
	return Choice;
}

void ScreenTimerRefresh()
{
	short DelayItem;
	ReadMemory(TIMER_BACKLIGHT_ADDR, 1, &DelayItem);
	if(DelayItem != ALWAYS_ON)
	{
		TimerRefreshMenu--;
		if(TimerRefreshMenu == 0)
		{
			TimerRefreshMenu = TimerDalays[DelayItem].DelayValue;
			if(Flag.IsDisplayOn)
			{
				LCDDisplayOff();
				Flag.IsDisplayOn = false;
			}
		}
	}
	return;
}