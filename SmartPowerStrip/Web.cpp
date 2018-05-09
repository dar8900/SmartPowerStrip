#include <ESP8266WiFi.h>
// #include <ESP8266WebServer.h>
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

const char* Hostname = "cavestrip";
String HostName = "cavestrip";

char HTTP_req[REQ_BUF_SZ] = {0}; // buffered HTTP request stored as null terminated string
char req_index = 0;              // index into HTTP_req buffer

WIFI_LIST List[] = 
{
	{"No Conn."     , "password"					, "NoConn" 					},
	{"Dario Cell"	, "dari9299"					, "DEO DOOM"				},
	{"Wifi Nonna"	, "Kyr2FGdVynR9ejUE"			, "TP-LINK_Extender_2.4GHz" },
	{"Camera mia"	, "dariolinorobby198919611962"	, "ZIXEL"					},
	{"Salotto Casa"	, "Kyr2FGdVynR9ejUE"			, "TIM-56878495"			},
	{"Camera Grande", "Kyr2FGdVynR9ejUE"			, "TIM-56878495_EXT"		},
};

short WifiItemSsid;

// WiFiServer server(80);
WiFiClient client;

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
    // server.begin();           // start to listen for clients
}

void WifiDisconnect()
{
	ClearLCD();
	LCDPrintString(ONE, CENTER_ALIGN, "Disconnesso");
	LCDPrintString(TWO, CENTER_ALIGN, "dalla rete:");
	LCDPrintString(THREE, CENTER_ALIGN, List[WifiItemSsid].Ssid);
	WiFi.disconnect(); 
	Flag.WifiActive = false;
	delay(1500);
	ClearLCD();
}



void WebClient()
{
    if (client) 
	{  // got client?
        bool currentLineIsBlank = true;
		Flag.ClientConnected = true;
		ClearLCD();
		LCDShowPopUp("Client CONNESSO");
		LCDPrintString(THREE, CENTER_ALIGN, "Client CONNESSO");
        while (client.connected()) 
		{
			TakeReleTime();
            if (client.available()) 
			{   // client data available to read
		
			}
		}
        client.stop(); // close the connection
		LCDShowPopUp("Client DISCONNESSO");
		delay(1000);
		ClearLCD();
		CheckReleStatus();
		Flag.ClientConnected = false;
    } // end if (client)
}
