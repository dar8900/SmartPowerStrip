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

extern RELE Rele[];
extern TIME_DATE_FORMAT PresentTime;
ESP8266WebServer server(80);

void HandleTimer1() 
{
	CheckEvents();
	String TimerStr;
	if(Rele[RELE_1].HaveTimer && Rele[RELE_1].IsActive)
		TimerStr = String(Rele[RELE_1].TimerTime.hour) + "h " + String(Rele[RELE_1].TimerTime.minute) + "m";
	else
		TimerStr = "NESSUN TIMER"; 
	
	server.send(200, "text/plane", TimerStr); //Send to client ajax request
}

void HandleTimer2() 
{
	CheckEvents();
	String TimerStr;
	if(Rele[RELE_2].HaveTimer && Rele[RELE_2].IsActive)
		TimerStr = String(Rele[RELE_2].TimerTime.hour) + "h " + String(Rele[RELE_2].TimerTime.minute) + "m";
	else
		TimerStr = "NESSUN TIMER"; 
	
	server.send(200, "text/plane", TimerStr); //Send to client ajax request
}

void HandleTimer3() 
{
	CheckEvents();
	String TimerStr;
	if(Rele[RELE_3].HaveTimer && Rele[RELE_3].IsActive)
		TimerStr = String(Rele[RELE_3].TimerTime.hour) + "h " + String(Rele[RELE_3].TimerTime.minute) + "m";
	else
		TimerStr = "NESSUN TIMER"; 
	
	server.send(200, "text/plane", TimerStr); //Send to client ajax request
}

void HandleTimer4() 
{
	CheckEvents();
	String TimerStr;
	if(Rele[RELE_4].HaveTimer && Rele[RELE_4].IsActive)
		TimerStr = String(Rele[RELE_4].TimerTime.hour) + "h " + String(Rele[RELE_4].TimerTime.minute) + "m";
	else
		TimerStr = "NESSUN TIMER"; 
	
	server.send(200, "text/plane", TimerStr); //Send to client ajax request
}

void HandleTimer5() 
{
	CheckEvents();
	String TimerStr;
	if(Rele[RELE_5].HaveTimer && Rele[RELE_5].IsActive)
		TimerStr = String(Rele[RELE_5].TimerTime.hour) + "h " + String(Rele[RELE_5].TimerTime.minute) + "m";
	else
		TimerStr = "NESSUN TIMER"; 
	
	server.send(200, "text/plane", TimerStr); //Send to client ajax request
}

void HandleTimer6() 
{
	CheckEvents();
	String TimerStr;
	if(Rele[RELE_6].HaveTimer && Rele[RELE_6].IsActive)
		TimerStr = String(Rele[RELE_6].TimerTime.hour) + "h " + String(Rele[RELE_6].TimerTime.minute) + "m";
	else
		TimerStr = "NESSUN TIMER"; 
	
	server.send(200, "text/plane", TimerStr); //Send to client ajax request
}

void HandleTimer7() 
{
	CheckEvents();
	String TimerStr;
	if(Rele[RELE_7].HaveTimer && Rele[RELE_7].IsActive)
		TimerStr = String(Rele[RELE_7].TimerTime.hour) + "h " + String(Rele[RELE_7].TimerTime.minute) + "m";
	else
		TimerStr = "NESSUN TIMER"; 
	
	server.send(200, "text/plane", TimerStr); //Send to client ajax request
}

void HandleTimer8() 
{
	CheckEvents();
	String TimerStr;
	if(Rele[RELE_8].HaveTimer && Rele[RELE_8].IsActive)
		TimerStr = String(Rele[RELE_8].TimerTime.hour) + "h " + String(Rele[RELE_8].TimerTime.minute) + "m";
	else
		TimerStr = "NESSUN TIMER"; 
	
	server.send(200, "text/plane", TimerStr); //Send to client ajax request
}

void HandleTurnOn1() 
{
	CheckEvents();
	String TurnOnStr;
	if(Rele[RELE_1].IsActive)
		TurnOnStr = String(Rele[RELE_1].TurnOnTime.day) + "g " + String(Rele[RELE_1].TurnOnTime.hour) + "h " + String(Rele[RELE_1].TurnOnTime.minute) + "m";
	else
		TurnOnStr = "SPENTA";
	
	server.send(200, "text/plane", TurnOnStr); //Send to client ajax request
}

void HandleTurnOn2() 
{
	CheckEvents();
	String TurnOnStr;
	if(Rele[RELE_2].IsActive)
		TurnOnStr = String(Rele[RELE_2].TurnOnTime.day) + "g " + String(Rele[RELE_2].TurnOnTime.hour) + "h " + String(Rele[RELE_2].TurnOnTime.minute) + "m";
	else
		TurnOnStr = "SPENTA";
	
	server.send(200, "text/plane", TurnOnStr); //Send to client ajax request
}
void HandleTurnOn3() 
{
	CheckEvents();
	String TurnOnStr;
	if(Rele[RELE_3].IsActive)
		TurnOnStr = String(Rele[RELE_3].TurnOnTime.day) + "g " + String(Rele[RELE_3].TurnOnTime.hour) + "h " + String(Rele[RELE_3].TurnOnTime.minute) + "m";
	else
		TurnOnStr = "SPENTA";
	
	server.send(200, "text/plane", TurnOnStr); //Send to client ajax request
}
void HandleTurnOn4() 
{
	CheckEvents();
	String TurnOnStr;
	if(Rele[RELE_4].IsActive)
		TurnOnStr = String(Rele[RELE_4].TurnOnTime.day) + "g " + String(Rele[RELE_4].TurnOnTime.hour) + "h " + String(Rele[RELE_4].TurnOnTime.minute) + "m";
	else
		TurnOnStr = "SPENTA"; 
	
	server.send(200, "text/plane", TurnOnStr); //Send to client ajax request
}
void HandleTurnOn5() 
{
	CheckEvents();
	String TurnOnStr;
	if(Rele[RELE_5].IsActive)
		TurnOnStr = String(Rele[RELE_5].TurnOnTime.day) + "g " + String(Rele[RELE_5].TurnOnTime.hour) + "h " + String(Rele[RELE_5].TurnOnTime.minute) + "m";
	else
		TurnOnStr = "SPENTA";
	
	server.send(200, "text/plane", TurnOnStr); //Send to client ajax request
}
void HandleTurnOn6() 
{
	CheckEvents();
	String TurnOnStr;
	if(Rele[RELE_6].IsActive)
		TurnOnStr = String(Rele[RELE_6].TurnOnTime.day) + "g " + String(Rele[RELE_6].TurnOnTime.hour) + "h " + String(Rele[RELE_6].TurnOnTime.minute) + "m";
	else
		TurnOnStr = "SPENTA"; 
	
	server.send(200, "text/plane", TurnOnStr); //Send to client ajax request
}
void HandleTurnOn7() 
{
	CheckEvents();
	String TurnOnStr;
	if(Rele[RELE_7].IsActive)
		TurnOnStr = String(Rele[RELE_7].TurnOnTime.day) + "g " + String(Rele[RELE_7].TurnOnTime.hour) + "h " + String(Rele[RELE_7].TurnOnTime.minute) + "m";
	else
		TurnOnStr = "SPENTA";
	
	server.send(200, "text/plane", TurnOnStr); //Send to client ajax request
}

void HandleTurnOn8() 
{
	CheckEvents();
	String TurnOnStr;
	if(Rele[RELE_8].IsActive)
		TurnOnStr = String(Rele[RELE_8].TurnOnTime.day) + "g " + String(Rele[RELE_8].TurnOnTime.hour) + "h " + String(Rele[RELE_8].TurnOnTime.minute) + "m";
	else
		TurnOnStr = "SPENTA";
	
	server.send(200, "text/plane", TurnOnStr); //Send to client ajax request
}

void HandleRele1() 
{
	String LedStatus = "OFF";
	String t_state = server.arg("STATUS_RELE1"); //Refer  xhttp.open("GET", "setLED?LEDstate="+led, true);
	if(t_state == "1")
	{
		if(!Rele[RELE_1].IsActive)
		{
			ON(ReleIdx2Pin(RELE_1)); //LED ON
			Rele[RELE_1].IsActive = true;
			Rele[RELE_1].TurnOnTime.day = PresentTime.day;
			Rele[RELE_1].TurnOnTime.hour = PresentTime.hour;
			Rele[RELE_1].TurnOnTime.minute = PresentTime.minute;			
		}
		LedStatus = "ACCESA"; //Feedback parameter
	}
	else
	{
		if(Rele[RELE_1].IsActive)
		{
			OFF(ReleIdx2Pin(RELE_1)); //LED OFF
			Rele[RELE_1].IsActive = true;
			Rele[RELE_1].ActiveTime = SetTimeVarRele(0,0,0,0);
			Rele[RELE_1].TurnOnTime = SetTimeVarRele(0,0,0,0);
		}
		LedStatus = "SPENTA"; //Feedback parameter  
	}
	server.send(200, "text/plane", LedStatus); //Send web page	
	delay(200);
}

void HandleRele2() 
{
	String LedStatus = "OFF";
	String t_state = server.arg("STATUS_RELE2"); //Refer  xhttp.open("GET", "setLED?LEDstate="+led, true);
	if(t_state == "1")
	{
		if(!Rele[RELE_2].IsActive)
		{
			ON(ReleIdx2Pin(RELE_2)); //LED ON
			Rele[RELE_2].IsActive = true;
			Rele[RELE_2].TurnOnTime.day = PresentTime.day;
			Rele[RELE_2].TurnOnTime.hour = PresentTime.hour;
			Rele[RELE_2].TurnOnTime.minute = PresentTime.minute;			
		}
		LedStatus = "ACCESA"; //Feedback parameter
	}
	else
	{
		if(Rele[RELE_2].IsActive)
		{
			OFF(ReleIdx2Pin(RELE_2)); //LED OFF
			Rele[RELE_2].IsActive = true;
			Rele[RELE_2].ActiveTime = SetTimeVarRele(0,0,0,0);
			Rele[RELE_2].TurnOnTime = SetTimeVarRele(0,0,0,0);
		}
		LedStatus = "SPENTA"; //Feedback parameter  
	}
	server.send(200, "text/plane", LedStatus); //Send web page	
	delay(200);
}
void HandleRele3() 
{
	String LedStatus = "OFF";
	String t_state = server.arg("STATUS_RELE3"); //Refer  xhttp.open("GET", "setLED?LEDstate="+led, true);
	if(t_state == "1")
	{
		if(!Rele[RELE_3].IsActive)
		{
			ON(ReleIdx2Pin(RELE_3)); //LED ON
			Rele[RELE_3].IsActive = true;
			Rele[RELE_3].TurnOnTime.day = PresentTime.day;
			Rele[RELE_3].TurnOnTime.hour = PresentTime.hour;
			Rele[RELE_3].TurnOnTime.minute = PresentTime.minute;			
		}
		LedStatus = "ACCESA"; //Feedback parameter
	}
	else
	{
		if(Rele[RELE_3].IsActive)
		{
			OFF(ReleIdx2Pin(RELE_3)); //LED OFF
			Rele[RELE_3].IsActive = true;
			Rele[RELE_3].ActiveTime = SetTimeVarRele(0,0,0,0);
			Rele[RELE_3].TurnOnTime = SetTimeVarRele(0,0,0,0);
		}
		LedStatus = "SPENTA"; //Feedback parameter  
	}
	server.send(200, "text/plane", LedStatus); //Send web page	
	delay(200);
}

void HandleRele4() 
{
	String LedStatus = "OFF";
	String t_state = server.arg("STATUS_RELE4"); //Refer  xhttp.open("GET", "setLED?LEDstate="+led, true);
	if(t_state == "1")
	{
		if(!Rele[RELE_4].IsActive)
		{
			ON(ReleIdx2Pin(RELE_4)); //LED ON
			Rele[RELE_4].IsActive = true;
			Rele[RELE_4].TurnOnTime.day = PresentTime.day;
			Rele[RELE_4].TurnOnTime.hour = PresentTime.hour;
			Rele[RELE_4].TurnOnTime.minute = PresentTime.minute;			
		}
		LedStatus = "ACCESA"; //Feedback parameter
	}
	else
	{
		if(Rele[RELE_4].IsActive)
		{
			OFF(ReleIdx2Pin(RELE_4)); //LED OFF
			Rele[RELE_4].IsActive = true;
			Rele[RELE_4].ActiveTime = SetTimeVarRele(0,0,0,0);
			Rele[RELE_4].TurnOnTime = SetTimeVarRele(0,0,0,0);
		}
		LedStatus = "SPENTA"; //Feedback parameter  
	}
	server.send(200, "text/plane", LedStatus); //Send web page	
	delay(200);
}

void HandleRele5() 
{
	String LedStatus = "OFF";
	String t_state = server.arg("STATUS_RELE5"); //Refer  xhttp.open("GET", "setLED?LEDstate="+led, true);
	if(t_state == "1")
	{
		if(!Rele[RELE_5].IsActive)
		{
			ON(ReleIdx2Pin(RELE_5)); //LED ON
			Rele[RELE_5].IsActive = true;
			Rele[RELE_5].TurnOnTime.day = PresentTime.day;
			Rele[RELE_5].TurnOnTime.hour = PresentTime.hour;
			Rele[RELE_5].TurnOnTime.minute = PresentTime.minute;			
		}
		LedStatus = "ACCESA"; //Feedback parameter
	}
	else
	{
		if(Rele[RELE_5].IsActive)
		{
			OFF(ReleIdx2Pin(RELE_5)); //LED OFF
			Rele[RELE_5].IsActive = true;
			Rele[RELE_5].ActiveTime = SetTimeVarRele(0,0,0,0);
			Rele[RELE_5].TurnOnTime = SetTimeVarRele(0,0,0,0);
		}
		LedStatus = "SPENTA"; //Feedback parameter  
	}
	server.send(200, "text/plane", LedStatus); //Send web page	
	delay(200);
}

void HandleRele6()
{
	String LedStatus = "OFF";
	String t_state = server.arg("STATUS_RELE6"); //Refer  xhttp.open("GET", "setLED?LEDstate="+led, true);
	if(t_state == "1")
	{
		if(!Rele[RELE_6].IsActive)
		{
			ON(ReleIdx2Pin(RELE_6)); //LED ON
			Rele[RELE_6].IsActive = true;
			Rele[RELE_6].TurnOnTime.day = PresentTime.day;
			Rele[RELE_6].TurnOnTime.hour = PresentTime.hour;
			Rele[RELE_6].TurnOnTime.minute = PresentTime.minute;			
		}
		LedStatus = "ACCESA"; //Feedback parameter
	}
	else
	{
		if(Rele[RELE_6].IsActive)
		{
			OFF(ReleIdx2Pin(RELE_6)); //LED OFF
			Rele[RELE_6].IsActive = true;
			Rele[RELE_6].ActiveTime = SetTimeVarRele(0,0,0,0);
			Rele[RELE_6].TurnOnTime = SetTimeVarRele(0,0,0,0);
		}
		LedStatus = "SPENTA"; //Feedback parameter  
	}
	server.send(200, "text/plane", LedStatus); //Send web page	
	delay(200);
}

void HandleRele7() 
{
	String LedStatus = "OFF";
	String t_state = server.arg("STATUS_RELE7"); //Refer  xhttp.open("GET", "setLED?LEDstate="+led, true);
	if(t_state == "1")
	{
		if(!Rele[RELE_7].IsActive)
		{
			ON(ReleIdx2Pin(RELE_7)); //LED ON
			Rele[RELE_7].IsActive = true;
			Rele[RELE_7].TurnOnTime.day = PresentTime.day;
			Rele[RELE_7].TurnOnTime.hour = PresentTime.hour;
			Rele[RELE_7].TurnOnTime.minute = PresentTime.minute;			
		}
		LedStatus = "ACCESA"; //Feedback parameter
	}
	else
	{
		if(Rele[RELE_7].IsActive)
		{
			OFF(ReleIdx2Pin(RELE_7)); //LED OFF
			Rele[RELE_7].IsActive = true;
			Rele[RELE_7].ActiveTime = SetTimeVarRele(0,0,0,0);
			Rele[RELE_7].TurnOnTime = SetTimeVarRele(0,0,0,0);
		}
		LedStatus = "SPENTA"; //Feedback parameter  
	}
	server.send(200, "text/plane", LedStatus); //Send web page	
	delay(200);
}

void HandleRele8()
{
	String LedStatus = "OFF";
	String t_state = server.arg("STATUS_RELE8"); //Refer  xhttp.open("GET", "setLED?LEDstate="+led, true);
	if(t_state == "1")
	{
		if(!Rele[RELE_8].IsActive)
		{
			ON(ReleIdx2Pin(RELE_8)); //LED ON
			Rele[RELE_8].IsActive = true;
			Rele[RELE_8].TurnOnTime.day = PresentTime.day;
			Rele[RELE_8].TurnOnTime.hour = PresentTime.hour;
			Rele[RELE_8].TurnOnTime.minute = PresentTime.minute;			
		}
		LedStatus = "ACCESA"; //Feedback parameter
	}
	else
	{
		if(Rele[RELE_8].IsActive)
		{
			OFF(ReleIdx2Pin(RELE_8)); //LED OFF
			Rele[RELE_8].IsActive = true;
			Rele[RELE_8].ActiveTime = SetTimeVarRele(0,0,0,0);
			Rele[RELE_8].TurnOnTime = SetTimeVarRele(0,0,0,0);
		}
		LedStatus = "SPENTA"; //Feedback parameter  
	}
	server.send(200, "text/plane", LedStatus); //Send web page	
	delay(200);
}