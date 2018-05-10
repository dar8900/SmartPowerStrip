#include <ESP8266WiFi.h>
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
#include "Buttons.h"

#include "WebHandleFunctions.h"
#include "WebPage.h"

extern FLAGS Flag;
extern RELE Rele[];
extern TIME_DATE_FORMAT PresentTime;
extern BAND_FORMAT Band;
extern ESP8266WebServer server;

const char* Hostname = "cavestrip";
String HostName = "cavestrip";
short WifiItemSsid;
WiFiServer ClientServer(80);
WiFiClient client;

WIFI_LIST List[] = 
{
	{"No Conn."     , "password"					, "NoConn" 					},
	{"Dario Cell"	, "dari9299"					, "DEO DOOM"				},
	{"Wifi Nonna"	, "Kyr2FGdVynR9ejUE"			, "TP-LINK_Extender_2.4GHz" },
	{"Camera mia"	, "dariolinorobby198919611962"	, "ZIXEL"					},
	{"Salotto Casa"	, "Kyr2FGdVynR9ejUE"			, "TIM-56878495"			},
	{"Camera Grande", "Kyr2FGdVynR9ejUE"			, "TIM-56878495_EXT"		},
};



void WifiInit()
{
	short NumbPoint = 0;
	short TimerNoConnection = 0;
	bool ExitWifiInit = false;
	short WifiListItem = 0;
	short ButtonPress = NO_PRESS;
	String HostnameExtended = "http://", NomeWifi;
	Flag.WifiActive = false;
	ClearLCD();
	WiFi.mode(WIFI_STA);
	while(!ExitWifiInit)
	{
		LCDPrintString(ONE, CENTER_ALIGN, "Scegli a quale rete");
		LCDPrintString(TWO, CENTER_ALIGN, "connettersi:");
		LCDPrintString(THREE, CENTER_ALIGN, List[WifiListItem].Ssid);
		ButtonPress = CheckButtons();
		switch(ButtonPress)
		{
			case BUTTON_UP:
				if(WifiListItem > 0)
					WifiListItem--;
				else
					WifiListItem = MAX_WIFI_ITEM - 1;
				LCDPrintLineVoid(THREE);
				break;
			case BUTTON_DOWN:
				if(WifiListItem < MAX_WIFI_ITEM - 1)
					WifiListItem++;
				else
					WifiListItem = 0;
				LCDPrintLineVoid(THREE);			
				break;
			case BUTTON_SET:
				NomeWifi = String(List[WifiListItem].RealSsid);
				WifiItemSsid = WifiListItem;
				ExitWifiInit = true;
				ClearLCD();
				break;
			case BUTTON_LEFT:
			default:
				break;
		}
		ButtonPress = NO_PRESS;
		delay(60);
	}
	if(NomeWifi != String(List[NO_CONN].RealSsid))
	{
		WiFi.hostname(Hostname);
		WiFi.begin(List[WifiListItem].RealSsid, List[WifiListItem].Password);	
		LCDPrintString(ONE, CENTER_ALIGN, "Connettendo a:");
		LCDPrintString(TWO, CENTER_ALIGN, List[WifiListItem].Ssid);
		while (WiFi.status() != WL_CONNECTED) 
		{
			delay(500);
			if(NumbPoint > 19)
			{
				NumbPoint = 0;
				LCDPrintLineVoid(2);
			}
			LCDPrintString(2, 0 + NumbPoint, ".");
			NumbPoint++;
			TimerNoConnection++;
			if(TimerNoConnection == 25)
			{
				ClearLCD();
				LCDPrintString(ONE, CENTER_ALIGN, "Nessuna rete");
				LCDPrintString(TWO, CENTER_ALIGN, "rilevata.");
				LCDPrintString(THREE, CENTER_ALIGN, "Uscita...");
				delay(2000);
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
			LCDPrintString(TWO, CENTER_ALIGN, "Hostname: ");	
			LCDPrintString(THREE, CENTER_ALIGN, HostnameExtended);
			delay(3000);
			ClearLCD();		
		}
	}
	return;
}

String WifiIP()
{
	String IP;
	//IP = String(WiFi.localIP());
	IP = WiFi.localIP().toString();
	return IP;
}


void WebServerInit()
{
	server.on("/", HandleHomePage);      //Which routine to handle at root location. This is display page
	server.on("/SetRele1", HandleRele1);
	server.on("/SetRele2", HandleRele2);
	server.on("/SetRele3", HandleRele3);
	server.on("/SetRele4", HandleRele4);
	server.on("/SetRele5", HandleRele5);
	server.on("/SetRele6", HandleRele6);
	server.on("/SetRele7", HandleRele7);
	server.on("/SetRele8", HandleRele8);
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
}

void WifiDisconnect()
{
	ClearLCD();
	LCDPrintString(ONE, CENTER_ALIGN, "Disconnesso");
	LCDPrintString(TWO, CENTER_ALIGN, "dalla rete:");
	LCDPrintString(THREE, CENTER_ALIGN, List[WifiItemSsid].Ssid);
	WiFi.disconnect(); 
	server.close();
	Flag.WifiActive = false;
	delay(1500);
	ClearLCD();
}

static bool ClientConnected()
{
	WiFiClient client = ClientServer.available();
	if(client)
		return true;
	else
		return false;
}

void WebClient()
{
	if(Flag.WifiActive)
	{
		if(ClientConnected())
		{
			ClearLCD();
			LCDPrintString(THREE, CENTER_ALIGN, "Client CONNESSO");
			while(ClientConnected())
			{
				TakePresentTime();
				TakeReleTime();
				server.handleClient();
				delay(50);
			}
			LCDPrintLineVoid(THREE);
			LCDPrintString(THREE, CENTER_ALIGN, "Client DISCONNESSO");
			delay(1500);
			ClearLCD();
			CheckReleStatus();
		}
	}
}
