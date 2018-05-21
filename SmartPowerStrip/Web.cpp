
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

extern FLAGS Flag;
extern RELE Rele[];
extern TIME_DATE_FORMAT PresentTime;
extern BAND_FORMAT Band;
extern ESP8266WebServer server;

const char* Hostname = "cavestrip";
String HostName = "cavestrip";
// WiFiServer ClientServer(80);

WIFI_LIST List[] =
{
	{"No Conn."     , "password"					, "NoConn" 					, 0},
	{"Dario Cell"	, "dari9299"					, "DEO DOOM"				, 0},
	{"Wifi Nonna"	, "Kyr2FGdVynR9ejUE"			, "TP-LINK_Extender_2.4GHz" , 0},
	{"Camera mia"	, "dariolinorobby198919611962"	, "ZIXEL"					, 0},
	{"Salotto Casa"	, "Kyr2FGdVynR9ejUE"			, "TIM-56878495"			, 0},
	{"Camera Grande", "Kyr2FGdVynR9ejUE"			, "TIM-56878495_EXT"		, 0},
};



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
	if(OldWifiItem != NO_CONN && OldWifiItem < MAX_WIFI_ITEM)
	{
		LCDPrintString(ONE, CENTER_ALIGN, "Vuoi riconnettere");
		LCDPrintString(TWO, CENTER_ALIGN, "la rete:");
		LCDPrintString(THREE, CENTER_ALIGN, List[OldWifiItem].Ssid);
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
		WifiConnectionChoice(&WifiListItem, &NomeWifi);
	}
	ClearLCD();
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
			delay(2000);
			ClearLCD();
		}
	}
	return;
}

String GetWifiSignalPower()
{
	short NumberOfNetworks = WiFi.scanNetworks(false, true);
	String ssid, SignalPower;
	uint8_t encryptionType;
	int32_t RSSI;
	uint8_t* BSSID;
	int32_t channel;
	bool isHidden, Found = false;
	short CurrentNetwork = 0;
	short WifiItem = 0;
	ReadMemory(WIFI_SSID_ADDR, 1, &WifiItem);
	for (CurrentNetwork = 0; CurrentNetwork < NumberOfNetworks; CurrentNetwork++)
	{
		WiFi.getNetworkInfo(CurrentNetwork, ssid, encryptionType, RSSI, BSSID, channel, isHidden);
		if(ssid == String(List[WifiItem].RealSsid))
			break;
	}
	if(RSSI <= 0 && RSSI >= -25)
	{
		SignalPower = "Ottimo";
	}
	else if(RSSI <= -26 && RSSI >= -70)
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
	return SignalPower;
}

String WifiIP()
{
	String IP;
	//IP = String(WiFi.localIP());
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

void WifiScanForSignal()
{
	short NumberOfNetworks = WiFi.scanNetworks(false, true);
	short WifiListItem = 0;
	ReadMemory(WIFI_SSID_ADDR, 1, &WifiListItem);
	short CurrentNetwork = 0;
	String ssid;
	uint8_t encryptionType;
	int32_t RSSI;
	uint8_t* BSSID;
	int32_t channel;
	bool isHidden;

	for (CurrentNetwork = 0; CurrentNetwork < NumberOfNetworks; CurrentNetwork++)
	{
		WiFi.getNetworkInfo(CurrentNetwork, ssid, encryptionType, RSSI, BSSID, channel, isHidden);
		if(ssid == String(List[WifiListItem].RealSsid))
			break;
	}
	if(RSSI < -96)
	{
		WiFi.disconnect();
		WifiRiconnect();
	}

}

void WifiRiconnect()
{
	String ssid;
	uint8_t encryptionType;
	int32_t RSSI;
	uint8_t* BSSID;
	int32_t channel;
	bool isHidden;
	short NumberOfNetworks = WiFi.scanNetworks(false, true);
	short CurrentNetwork = 0, MyNetworks = 0;
	bool Connect = false;
	for (CurrentNetwork = 0; CurrentNetwork < NumberOfNetworks; CurrentNetwork++)
	{
		bool Exit = false;
		WiFi.getNetworkInfo(CurrentNetwork, ssid, encryptionType, RSSI, BSSID, channel, isHidden);
		for(MyNetworks = DARIO_CELL; MyNetworks < MAX_WIFI_ITEM; MyNetworks++)
		{
			if(ssid == String(List[MyNetworks].RealSsid) && RSSI > -90)
			{
				Exit = true;
				break;
			}
		}
		if(Exit)
		{
			Connect = true;
			break;
		}

	}
	if(Connect)
	{
		ClearLCD();
		WiFi.hostname(Hostname);
		WiFi.begin(List[MyNetworks].RealSsid, List[MyNetworks].Password);
		while (WiFi.status() != WL_CONNECTED)
		{
			LCDPrintString(ONE, CENTER_ALIGN, "Connettendo a:");
			LCDPrintString(TWO, CENTER_ALIGN, List[MyNetworks].Ssid);
			Flag.WifiActive = true;
		}
		WriteMemory(WIFI_SSID_ADDR, MyNetworks);
		ClearLCD();
	}
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
	delay(1000);
	ClearLCD();
}

void WifiDisconnect()
{
	ClearLCD();
	short WifiItemSsid = 0;
	ReadMemory(WIFI_SSID_ADDR, 1, &WifiItemSsid);
	LCDPrintString(ONE, CENTER_ALIGN, "Disconnesso");
	LCDPrintString(TWO, CENTER_ALIGN, "dalla rete:");
	LCDPrintString(THREE, CENTER_ALIGN, List[WifiItemSsid].Ssid);
	WiFi.disconnect();
	server.close();
	Flag.WifiActive = false;
	delay(1500);
	ClearLCD();
}

// static bool ClientConnected()
// {
	// WiFiClient client = ClientServer.available();
	// if(client)
	// {
		// Flag.ClientConnected = true;
		// client.stop();
		// return true;
	// }
	// else
	// {
		// Flag.ClientConnected = false;
		// return false;
	// }

// }

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
		LCDPrintString(THREE, CENTER_ALIGN, List[ListItem].Ssid);
		ButtonPress = CheckButtons();
		switch(ButtonPress)
		{
			case BUTTON_UP:
				if(ListItem > 0)
					ListItem--;
				else
					ListItem = MAX_WIFI_ITEM - 1;
				LCDPrintLineVoid(THREE);
				break;
			case BUTTON_DOWN:
				if(ListItem < MAX_WIFI_ITEM - 1)
					ListItem++;
				else
					ListItem = 0;
				LCDPrintLineVoid(THREE);
				break;
			case BUTTON_SET:
				Nome = String(List[ListItem].RealSsid);
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
		delay(60);
	}
}
