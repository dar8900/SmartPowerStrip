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

extern FLAGS Flag;
extern RELE Rele[];
extern TIME_DATE_FORMAT PresentTime;
extern BAND_FORMAT Band;

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

/*
void WebServer()
{
// INIT WEB
  server.on("/", handle_OnConnect);
  server.on("/ledon", handle_ledon);
  server.on("/ledoff", handle_ledoff);
  server.onNotFound(handle_NotFound);
  
  server.handleClient();
  if(LEDstatus)
  digitalWrite(LEDpin, HIGH);
  else
  digitalWrite(LEDpin, LOW);
}

void handle_OnConnect() {
  LEDstatus = LOW;
  server.send(200, "text/html", SendHTML(false)); 
}

void handle_ledon() {
  LEDstatus = HIGH;
  server.send(200, "text/html", SendHTML(true)); 
}

void handle_ledoff() {
  LEDstatus = LOW;
  server.send(200, "text/html", SendHTML(false)); 
}

void handle_NotFound(){
  server.send(404, "text/plain", "Not found");
}

String SendHTML(short ReleStatus){
	String ptr = "<!DOCTYPE html>\n";
	ptr +="<html>\n";
	ptr +="<head>\n";
	ptr +="<title>Smart Power Strip</title>\n";
	ptr +="</head>\n";
	ptr +="<body>\n";
	ptr +="<h1>LED</h1>\n";
	ptr +="<p>Click to switch LED on and off.</p>\n";
	ptr +="<form method=\"get\">\n";
	if(ReleStatus)
		ptr +="<input type=\"button\" value=\"LED OFF\" onclick=\"window.location.href='/ledoff'\">\n";
	else
		ptr +="<input type=\"button\" value=\"LED ON\" onclick=\"window.location.href='/ledon'\">\n";
	ptr +="</form>\n";
	ptr +="</body>\n";
	ptr +="</html>\n";
	return ptr;
}

*/
