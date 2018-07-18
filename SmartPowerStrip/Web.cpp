
#include <ESP8266WebServer.h>
#include <Arduino.h>
#include "Web.h"
#include "LCDLib.h"
#include "SmartPowerStrip.h"
#include "Rele.h"
#include "Band.h"
#include "Menu.h"
#include "EEPROM_Ard.h"
#include "TimeLib.h"
#include "I2CNano.h"

#include "WebHandleFunctions.h"

#define TIMER_SCAN_WIFI		7500 // circa 16 s
#define MAX_NETWORK_NAME	4

extern FLAGS Flag;
extern RELE Rele[];
extern TIME_DATE_FORMAT PresentTime;
extern BAND_FORMAT Band;
extern ESP8266WebServer server;

const char* Hostname = "cavestrip";
String HostName = "cavestrip";
short MyConnectionNumber;
uint16_t TimerScanWifi = TIMER_SCAN_WIFI;
// WiFiServer ClientServer(80);

const WIFI_LIST MyNetworkList[] =
{
	{"No Conn."     , "password"							, "NoConn" 					},
	{"Dario Cell"	, "dari9299"							, "DEO DOOM"				},
	{"Wifi Nonna"	, "robbylinodario196219611989"			, "TIM-89557989_EXT_NONN"   },
	{"Camera mia"	, "dariolinorobby198919611962"			, "WIFI-56878495"			},
	{"Salotto Casa"	, "robbylinodario196219611989"			, "TIM-90914475"			},
	{"Camera Grande", "robbylinodario196219611989"			, "TIM-90914475_RPT"		},
};


const String NetworkName[]
{
	"Dario Cell"	,
	"Wifi Nonna"	,
	"Camera mia"	,
	"Salotto Casa"	,
	"Camera Grande" ,
};


const String WifiConfigName[]
{
	"WPS"	    ,
	"Cablata"	,
};

WpsConfigSelected

void WifiConfInit()
{
	short ButtonPress = NO_PRESS;
	short WifiConfigItem = WPS;
	bool ExitWifiConfInit = false;
	ClearLCD();
	LCDPrintString(ONE, CENTER_ALIGN, "Quale configurazione");
	LCDPrintString(TWO, CENTER_ALIGN, "WiFi usare?");
	while(!ExitWifiConfInit)
	{
		LCDPrintString(THREE, CENTER_ALIGN, WifiConfigName[WifiConfigItem]);
		ButtonPress = CheckButtons();
		switch(ButtonPress)
		{
			case BUTTON_UP:
				if(WifiConfigItem > WPS)
					WifiConfigItem--;
				else
					WifiConfigItem = MAX_WIFI_CONFIG - 1;
				break;
			case BUTTON_DOWN:
				if(WifiConfigItem < MAX_WIFI_CONFIG - 1)
					WifiConfigItem++;
				else
					WifiConfigItem = WPS;
				break;
			case BUTTON_LEFT:
				break;
		}
		delay(WHILE_LOOP_DELAY);
	}
}



void WifiInit()
{
	short NumbPoint = 0;
	short TimerNoConnection = 0;
	bool ExitWifiInit = false;
	short WifiListItem = NO_CONN;
	short ButtonPress = NO_PRESS;
	short OldWifiItem = 0;
	String HostnameExtended = "http://", NomeWifi;
	Flag.WifiActive = false;
	ClearLCD();
	WiFi.mode(WIFI_STA);
	ReadMemory(WIFI_SSID_ADDR, 1, &OldWifiItem);
	if(OldWifiItem != NO_CONN && OldWifiItem < MAX_WIFI_ITEM && !Flag.WifiReconnect)
	{
		LCDPrintString(ONE, CENTER_ALIGN, "Vuoi riconnettere");
		LCDPrintString(TWO, CENTER_ALIGN, "la rete:");
		LCDPrintString(THREE, CENTER_ALIGN, MyNetworkList[OldWifiItem].WifiName);
		if(CheckYesNo())
		{
			WifiListItem = OldWifiItem;
		}
		else
		{
			ClearLCD();
			WifiConnectionChoice(&WifiListItem, &NomeWifi);
		}
	}
	else
	{
		if(!Flag.WifiReconnect)
			WifiConnectionChoice(&WifiListItem, &NomeWifi);
	}
	ClearLCD();
	if(Flag.WifiReconnect)
	{
		for(WifiListItem = DARIO_CELL; WifiListItem < MAX_WIFI_ITEM; WifiListItem++)
		{
			ClearLCD();
			WiFi.hostname(Hostname);
			WiFi.begin(MyNetworkList[WifiListItem].RealSsid, MyNetworkList[WifiListItem].Password);
			LCDPrintString(ONE, CENTER_ALIGN, "Connettendo a:");
			LCDPrintString(TWO, CENTER_ALIGN, MyNetworkList[WifiListItem].WifiName);
			while (WiFi.status() != WL_CONNECTED)
			{
				TakePresentTime();
				TakeReleTime();
				delay(500);
				if(NumbPoint > 19)
				{
					NumbPoint = 0;
					ClearLCDLine(2);
				}
				LCDPrintString(THREE, 0 + NumbPoint, ".");
				NumbPoint++;
				TimerNoConnection++;
				if(TimerNoConnection == 25)
				{
					ClearLCD();
					LCDPrintString(ONE, CENTER_ALIGN, "Nessuna rete");
					LCDPrintString(TWO, CENTER_ALIGN, "rilevata.");
					LCDPrintString(THREE, CENTER_ALIGN, "Uscita...");
					TimerNoConnection = 0;
					NumbPoint = 0;
					delay(DELAY_INFO_MSG);
					ClearLCD();
					Flag.WifiActive = false;
					break;
				}
				Flag.WifiActive = true;
			}
			if(Flag.WifiActive)
			{
				LCDShowPopUp("Connesso!");
				HostnameExtended += String(Hostname);
				LCDPrintString(ONE, LEFT_ALIGN, "IP:");
				LCDPrintString(ONE, RIGHT_ALIGN, WifiIP());
				LCDPrintString(TWO, CENTER_ALIGN, "Hostname: ");
				LCDPrintString(THREE, CENTER_ALIGN, HostnameExtended);
				LCDPrintString(FOUR, LEFT_ALIGN, "Segnale:");
				LCDPrintString(FOUR, RIGHT_ALIGN, GetWifiSignalPower());
				WriteMemory(WIFI_SSID_ADDR, WifiListItem);
				delay(DELAY_INFO_MSG);
				ClearLCD();
				Flag.WifiReconnect = false;
				break; //Uscita for
			}
		}
		if(!Flag.WifiActive && Flag.WifiReconnect)
		{
			WifiTurnOff();
		}
	}
	else
	{
		if(NomeWifi != String(MyNetworkList[NO_CONN].RealSsid))
		{
			WiFi.hostname(Hostname);
			WiFi.begin(MyNetworkList[WifiListItem].RealSsid, MyNetworkList[WifiListItem].Password);
			LCDPrintString(ONE, CENTER_ALIGN, "Connettendo a:");
			LCDPrintString(TWO, CENTER_ALIGN, MyNetworkList[WifiListItem].WifiName);
			while (WiFi.status() != WL_CONNECTED)
			{
				delay(500);
				TakePresentTime();
				TakeReleTime();
				if(NumbPoint > 19)
				{
					NumbPoint = 0;
					ClearLCDLine(2);
				}
				LCDPrintString(THREE, 0 + NumbPoint, ".");
				NumbPoint++;
				TimerNoConnection++;
				if(TimerNoConnection == 25)
				{
					ClearLCD();
					LCDPrintString(ONE, CENTER_ALIGN, "Nessuna rete");
					LCDPrintString(TWO, CENTER_ALIGN, "rilevata.");
					LCDPrintString(THREE, CENTER_ALIGN, "Uscita...");
					delay(DELAY_INFO_MSG);
					ClearLCD();
					Flag.WifiActive = false;
					break;
				}
				Flag.WifiActive = true;
			}
			if(Flag.WifiActive)
			{
				LCDShowPopUp("Connesso!");
				HostnameExtended += String(Hostname);
				LCDPrintString(ONE, LEFT_ALIGN, "IP:");
				LCDPrintString(ONE, RIGHT_ALIGN, WifiIP());
				LCDPrintString(TWO, CENTER_ALIGN, "Hostname: ");
				LCDPrintString(THREE, CENTER_ALIGN, HostnameExtended);
				LCDPrintString(FOUR, LEFT_ALIGN, "Segnale:");
				LCDPrintString(FOUR, RIGHT_ALIGN, GetWifiSignalPower());
				Flag.WifiReconnect = false;
				delay(DELAY_INFO_MSG);
				ClearLCD();
			}
			else
			{
				WriteMemory(WIFI_SSID_ADDR, NO_CONN);
				WifiTurnOff();
			}
		}
		else
		{
			WriteMemory(WIFI_SSID_ADDR, NO_CONN);
			WifiTurnOff();
		}
	}
	return;
}

static void AssignSsid()
{
	short ButtonPress = NO_PRESS;
	short WifiListItem = DARIO_CELL;
	bool ExitAssignSsid = false;
	ClearLCD();
	while(!ExitAssignSsid)
	{
		TakePresentTime();
		TakeReleTime();
		LCDPrintString(ONE, CENTER_ALIGN, "Assegna il nome");
		LCDPrintString(TWO, CENTER_ALIGN, "alla nuova rete");
		LCDPrintString(THREE, CENTER_ALIGN, NetworkName[WifiListItem]);
		ButtonPress = CheckButtons();
		switch(ButtonPress)
		{
			case BUTTON_UP:
				if(WifiListItem > 0)
					WifiListItem--;
				else
					WifiListItem = MAX_NETWORK_NAME;
				ClearLCDLine(THREE);
				break;
			case BUTTON_DOWN:
				if(WifiListItem < MAX_NETWORK_NAME)
					WifiListItem++;
				else
					WifiListItem = 0;
				ClearLCDLine(THREE);
				break;
			case BUTTON_LEFT:
				break;
			case BUTTON_SET:
				ClearLCD();
				LCDPrintString(TWO, CENTER_ALIGN, "Settato");
				WriteMemory(WIFI_WPS_NAME_ADDR, WifiListItem);
				// MyNetworkList[WifiListItem].RealSsid = WiFi.SSID().c_str();
				delay(DELAY_INFO_MSG);
				ClearLCD();
				ExitAssignSsid = true;
				break;
			default:
				break;
		}
		delay(WHILE_LOOP_DELAY);
	}
	
}


bool WPSConnection()
{
	bool WpsSuccess = false, FindSsid = false;
	short NumbPoint = 0;
	short WifiListItem = NO_CONN;
	WiFi.mode(WIFI_STA);
	WpsSuccess = WiFi.beginWPSConfig();
	delay(3000);
	ClearLCD();
	if(WiFi.SSID().lenght() > 0)
	{
		if(WpsSuccess)
		{
			while (WiFi.status() != WL_CONNECTED)
			{
				TakePresentTime();
				TakeReleTime();
				delay(500);
				if(NumbPoint > 19)
				{
					NumbPoint = 0;
					ClearLCDLine(2);
				}
				LCDPrintString(THREE, 0 + NumbPoint, ".");
				NumbPoint++;
			}
			Flag.WifiActive = true;
			LCDShowPopUp("Connesso");
			LCDPrintString(TWO, LEFT_ALIGN, "IP:");
			LCDPrintString(TWO, RIGHT_ALIGN, WifiIP());
			for(WifiListItem = DARIO_CELL; WifiListItem < MAX_WIFI_ITEM; WifiListItem++)
			{
				if(String(MyNetworkList[WifiListItem].RealSsid) == WiFi.SSID())
				{
					FindSsid = true;
					break;
				}
			}
			LCDPrintString(THREE, CENTER_ALIGN, "SSID:");
			if(FindSsid)
			{
				LCDPrintString(FOUR, CENTER_ALIGN, MyNetworkList[WifiListItem].WifiName);
				delay(DELAY_INFO_MSG);
			}
			else
			{
				LCDPrintString(FOUR, CENTER_ALIGN, "Sconosciuto");
				delay(DELAY_INFO_MSG);
				AssignSsid();
			}
			ClearLCD();
		}
		else
		{
			LCDPrintString(ONE, CENTER_ALIGN, "Configurazione WPS");
			LCDPrintString(TWO, CENTER_ALIGN, "fallita");
			delay(DELAY_INFO_MSG);
			Flag.WifiActive = false;
			ClearLCD();
		}
	}
	else
		WpsSuccess = false;
	return WpsSuccess;
}

String GetWifiSignalPower()
{
	short NumberOfNetworks = WiFi.scanNetworks(false, true);
	String ssid, SignalPower;
	int32_t RSSI;
	bool Found = false;
	short CurrentNetwork = 0;
	short WifiItem = 0;
	ReadMemory(WIFI_SSID_ADDR, 1, &WifiItem);
	TakePresentTime();
	TakeReleTime();
	for (CurrentNetwork = 0; CurrentNetwork < NumberOfNetworks; CurrentNetwork++)
	{
		ssid = String(WiFi.SSID(CurrentNetwork));
		RSSI = WiFi.RSSI(CurrentNetwork);
		if(ssid == String(MyNetworkList[WifiItem].RealSsid))
		{
			Found = true;
			MyConnectionNumber = CurrentNetwork;
			break;
		}
	}
	if(Found)
	{
		if(RSSI <= -26 && RSSI >= -60)
		{
			SignalPower = "Ottimo";
		}
		else if(RSSI <= -61 && RSSI >= -70)
		{
			SignalPower = "Buono";
		}
		else if(RSSI <= -71 && RSSI >= -80)
		{
			SignalPower = "Discreto";
		}
		else if(RSSI <= -81 && RSSI >= -96)
		{
			SignalPower = "Pessimo";
		}
		else if(RSSI < 97)
		{
			SignalPower = "No segnale";
		}
	}
	else
	{
		SignalPower = "No rete";
	}
	return SignalPower;
}

void WifiScanForSignal()
{
	short WifiListItem = 0, ConnectionNumbers = 0, CurrentNetwork = 0;
	ReadMemory(WIFI_SSID_ADDR, 1, &WifiListItem);
	String Ssid;
	int32_t RSSI;
	bool Found = false;
	if(TimerScanWifi == 0)
	{
		ClearLCD();
		LCDPrintString(TWO, CENTER_ALIGN, "Scan Wifi...");
		delay(1000);
		TimerScanWifi = TIMER_SCAN_WIFI;
		ConnectionNumbers = WiFi.scanNetworks(false, true);
		TakePresentTime();
		TakeReleTime();
		ReadMemory(WIFI_SSID_ADDR, 1, &WifiListItem);
		for (CurrentNetwork = 0; CurrentNetwork < ConnectionNumbers; CurrentNetwork++)
		{
			Ssid = String(WiFi.SSID(CurrentNetwork));
			RSSI = WiFi.RSSI(CurrentNetwork);
			if(Ssid == String(MyNetworkList[WifiListItem].RealSsid))
			{
				Found = true;
				MyConnectionNumber = CurrentNetwork;
				break;
			}
		}
		if(!Found)
		{
			ClearLCDLine(TWO);
			LCDPrintString(ONE, CENTER_ALIGN, "La rete");
			LCDPrintString(TWO, CENTER_ALIGN, MyNetworkList[WifiListItem].WifiName);
			LCDPrintString(THREE, CENTER_ALIGN, "risulta spenta.");
			LCDPrintString(FOUR, CENTER_ALIGN, "Ne cerco un'altra");
			delay(DELAY_INFO_MSG);
			ClearLCD();
			Flag.WifiActive = false;
			Flag.WifiReconnect = true;
		}
		else
		{
			ClearLCDLine(TWO);
			Flag.WifiReconnect = false;
		}
		if(Flag.WifiReconnect)
		{
			WifiInit();
		}

	}

	TimerScanWifi--;
}

void WebServerInit()
{
	server.on("/", HandleHomePage);      //Which routine to handle at root location. This is display page

	server.on("/GetStatusRele1", HandleStatus1);
	server.on("/GetStatusRele2", HandleStatus2);
	server.on("/GetStatusRele3", HandleStatus3);
	server.on("/GetStatusRele4", HandleStatus4);
	server.on("/GetStatusRele5", HandleStatus5);
	server.on("/GetStatusRele6", HandleStatus6);
	server.on("/GetStatusRele7", HandleStatus7);
	server.on("/GetStatusRele8", HandleStatus8);

	server.on("/SetRele1", HandleRele1);
	server.on("/SetRele2", HandleRele2);
	server.on("/SetRele3", HandleRele3);
	server.on("/SetRele4", HandleRele4);
	server.on("/SetRele5", HandleRele5);
	server.on("/SetRele6", HandleRele6);
	server.on("/SetRele7", HandleRele7);
	server.on("/SetRele8", HandleRele8);

	server.on("/BandStatus", HandleBandStatus);
	server.on("/GetFWInfo", HandleFWInfo);

	server.on("/TurnOnRele1", HandleTurnOn1);
	server.on("/TurnOnRele2", HandleTurnOn2);
	server.on("/TurnOnRele3", HandleTurnOn3);
	server.on("/TurnOnRele4", HandleTurnOn4);
	server.on("/TurnOnRele5", HandleTurnOn5);
	server.on("/TurnOnRele6", HandleTurnOn6);
	server.on("/TurnOnRele7", HandleTurnOn7);
	server.on("/TurnOnRele8", HandleTurnOn8);

	server.on("/TimerRele1", HandleTimer1);
	server.on("/TimerRele2", HandleTimer2);
	server.on("/TimerRele3", HandleTimer3);
	server.on("/TimerRele4", HandleTimer4);
	server.on("/TimerRele5", HandleTimer5);
	server.on("/TimerRele6", HandleTimer6);
	server.on("/TimerRele7", HandleTimer7);
	server.on("/TimerRele8", HandleTimer8);
	server.onNotFound(HandleNotFound);
	server.begin();
	ClearLCD();
	LCDPrintString(TWO, CENTER_ALIGN, "Server Avviato");
	delay(DELAY_INFO_MSG);
	ClearLCD();
}

void WifiDisconnect()
{
	ClearLCD();
	short WifiItemSsid = 0;
	ReadMemory(WIFI_SSID_ADDR, 1, &WifiItemSsid);
	LCDPrintString(ONE, CENTER_ALIGN, "Disconnesso");
	LCDPrintString(TWO, CENTER_ALIGN, "dalla rete:");
	LCDPrintString(THREE, CENTER_ALIGN, MyNetworkList[WifiItemSsid].WifiName);
	WiFi.disconnect();
	server.close();
	Flag.WifiActive = false;
	delay(DELAY_INFO_MSG);
	ClearLCD();
}

void WifiTurnOff()
{
	WiFi.disconnect();
	server.close();
	WiFi.mode(WIFI_OFF);
	Flag.WifiActive = false;
	LCDShowPopUp("Wifi Spento");
}

void WebClient()
{
	server.handleClient();
}

void WifiConnectionChoice(short *WifiListItem, String *NomeWifi)
{
	bool Exit = false;
	short ListItem = *WifiListItem, ButtonPress = NO_PRESS;
	String Nome = *NomeWifi;
	while(!Exit)
	{
		LCDPrintString(ONE, CENTER_ALIGN, "Scegli a quale rete");
		LCDPrintString(TWO, CENTER_ALIGN, "connettersi:");
		LCDPrintString(THREE, CENTER_ALIGN, MyNetworkList[ListItem].WifiName);
		ButtonPress = CheckButtons();
		switch(ButtonPress)
		{
			case BUTTON_UP:
				if(ListItem > 0)
					ListItem--;
				else
					ListItem = MAX_WIFI_ITEM - 1;
				ClearLCDLine(THREE);
				break;
			case BUTTON_DOWN:
				if(ListItem < MAX_WIFI_ITEM - 1)
					ListItem++;
				else
					ListItem = 0;
				ClearLCDLine(THREE);
				break;
			case BUTTON_SET:
				Nome = String(MyNetworkList[ListItem].RealSsid);
				WriteMemory(WIFI_SSID_ADDR, ListItem);
				*WifiListItem = ListItem;
				*NomeWifi = Nome;
				Exit = true;
				ClearLCD();
				break;
			case BUTTON_LEFT:
			default:
				break;
		}
		ButtonPress = NO_PRESS;
		delay(WHILE_LOOP_DELAY);
	}
}


String WifiIP()
{
	String IP;
	IP = WiFi.localIP().toString();
	return IP;
}

void ShowWifiStatus(short Row, short Col, bool Status)
{
	if(Status)
	{
		LCDMoveCursor(Row, Col);
		LCDShowIcon(WIFI_OK);
	}
	else
	{
		LCDMoveCursor(Row, Col);
		LCDShowIcon(WIFI_NO);
	}
}

void ShowClientConnected(short Row, short Col, bool Status)
{
	if(Status)
	{
		LCDMoveCursor(Row, Col);
		LCDShowIcon(CLIENT_CONN);
	}
	else
	{
		LCDMoveCursor(Row, Col);
		LCDShowIcon(EMPTY);
	}

}
