#include <Wire.h>
#include <ESP8266WiFi.h>
#include "EEPROM_Ard.h"
#include "LCDLib.h"
#include "TimeLib.h"
#include "SmartPowerStrip.h"
#include "Web.h"
#include "Menu.h"
#include "Band.h"
#include "Rele.h"


#undef FIRST_GO


extern uint8_t AlarmIcon[];
extern RELE Rele[];
extern WiFiClient client;
extern WiFiServer server;

uint8_t WifiConnectionOn[]
{
	0x0E,
	0x0A,
	0x0E,
	0x00,
	0x0A,
	0x0C,
	0x0C,
	0x0A
};

uint8_t WifiConnectionOff[]
{
	0x11,
	0x19,
	0x15,
	0x13,
	0x0E,
	0x11,
	0x11,
	0x0E
};

uint8_t OffRele[]
{
	0x0E,
	0x11,
	0x11,
	0x11,
	0x11,
	0x0E,
	0x04,
	0x04
};

uint8_t OnRele[]
{
	0x0E,
	0x1F,
	0x1F,
	0x1F,
	0x1F,
	0x0E,
	0x04,
	0x04
};


FLAGS Flag;

void BlinkLed(short pin)
{
	ON(pin);
	delay(5);
	OFF(pin);	
}

void CheckEvents()
{
	TakePresentTime();
	TakeReleTime();
	if(Flag.WifiActive)
	{
		client = server.available();
		WebClient();
	}
}

void RebootESP()
{
	WriteMemory(REBOOT_ADDR, 1);
	ESP.restart();
}

bool IsRebooted()
{
	bool IsESPRebboted = false;
	short RebootValue = 0;
	ReadMemory(REBOOT_ADDR, 1, &RebootValue);
	if(RebootValue == 1)
	{
		IsESPRebboted = true;
		RebootValue = 0;
		WriteMemory(REBOOT_ADDR, RebootValue);
	}
	else
	{
		IsESPRebboted = false;
	}
	
	return IsESPRebboted;
}

void setup() 
{
#ifndef FIRST_GO
	Serial.begin(115200);
	pinMode(RELE1, OUTPUT);
	pinMode(RELE2, OUTPUT);
	pinMode(RELE3, OUTPUT);
	pinMode(RELE4, OUTPUT);
	pinMode(RELE5, OUTPUT);
	pinMode(RELE6, OUTPUT);
	pinMode(RELE7, OUTPUT);
	pinMode(RELE8, OUTPUT);
	
	pinMode(BUTTON_LED, OUTPUT);
	
	short FirstStart = 0;
	Wire.begin(SDA, SCL); // Inizializza I2C per NodeMCU
	EepromInit();
	LCDInit();
	RTCInit();
	WifiInit();
	WebServerInit();
		
	LCDCreateIcon(WifiConnectionOn, WIFI_OK);
	LCDCreateIcon(WifiConnectionOff, WIFI_NO);
	LCDCreateIcon(OffRele, RELE_OFF);
	LCDCreateIcon(OnRele, RELE_ON);
	LCDCreateIcon(AlarmIcon, ALARM);
	TakePresentTime();
	
	ReadMemory(FIRST_START_CHECK_ADDR, 1, &FirstStart);
	if(FirstStart == EMPTY_MEMORY_VALUE)
	{
		WriteMemory(FIRST_START_CHECK_ADDR, 0);
		LCDNoBlink();
		LCDDisplayOn();
		SetBandInvalid();
		LCDDisplayOff();
		TurnOffAllRele();
		Flag.IsBandSetted = false;
		Flag.IsDisplayOn = false;
		Flag.ReleRS = true;
		LCDDisplayOn();
		Flag.IsDisplayOn = true;	
		ReleInit(true);
	}
	else
	{
		LCDNoBlink();
		LCDDisplayOn();
		Flag.IsDisplayOn = true;
		ReleInit(false);	
		BandInit();	
	}
#else
	Wire.begin(SDA, SCL);
	EepromInit();
	ClearMemory();
	LCDInit();
	if(IsMemoryEmpty())
		LCDPrintString(1, CENTER_ALIGN, "Memoria Vuota");

#endif // FIRST_GO	
}

void loop() 
{
#ifndef FIRST_GO
	MainScreen();
#endif
}
