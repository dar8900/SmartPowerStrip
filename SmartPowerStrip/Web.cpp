#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Arduino.h>
#include "Web.h"
#include "LCDLib.h"
#include "SmartPowerStrip.h"

extern FLAGS Flag;

const char* ssid = "ssid";  // Enter SSID here
const char* password = "password";  //Enter Password here
const char* Hostname = "cavestrip";

String HostName = "cavestrip";

ESP8266WebServer server(80);

void WifiInit()
{
	short NumbPoint = 0;
	String HostnameExtended = "http://";
	Flag.WifiActive = false;
	ClearLCD();
	WiFi.mode(WIFI_STA);
	WiFi.begin(ssid, password);	
	WiFi.hostname(Hostname);
	LCDPrintString(0, CENTER_ALIGN, "Connettendo a:");
	LCDPrintString(1, CENTER_ALIGN, String(ssid));
	while (WiFi.status() != WL_CONNECTED) 
	{
		delay(1000);
		LCDPrintString(2, 0 + NumbPoint, ".");
		NumbPoint++;
		if(NumbPoint > 20)
		{
			NumbPoint = 0;
			LCDPrintLineVoid(2);
		}
			
	}
	LCDPrintString(3, CENTER_ALIGN, "Connesso");
	delay(2000);
	ClearLCD();
	HostnameExtended += String(Hostname);
	LCDPrintString(0, CENTER_ALIGN, "Hostname: ");	
	LCDPrintString(1, CENTER_ALIGN, HostnameExtended);
	delay(3000);
	ClearLCD();
	Flag.WifiActive = true;
	return;
}

String WifiIP()
{
	String IP;
	IP = String(WiFi.localIP());
	return IP;
}