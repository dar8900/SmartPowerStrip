#include <Wire.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "EEPROM_Ard.h"
#include "LCDLib.h"
#include "TimeLib.h"
#include "SmartPowerStrip.h"
#include "Web.h"
#include "Menu.h"
#include "Band.h"
#include "Rele.h"
#include "I2CNano.h"


#undef FIRST_GO

extern RELE Rele[];
extern WiFiClient client;
extern ESP8266WebServer server;

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

uint8_t ClientConnection[]
{
	0x02,
	0x1F,
	0x02,
	0x00,
	0x00,
	0x08,
	0x1F,
	0x08
};


uint8_t EmptyIcon[]
{
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00
};

FLAGS Flag;
short EnterSetup;

String VersionValue = "1.8";
String VersionDate = "12/07/18";

void BlinkLed(short pin)
{
	ON(pin);
	delay(5);
	OFF(pin);
}

void delayms(short ms)
{
	delay(ms);
}

void CheckEvents()
{
	TakePresentTime();
	TakeReleTime();
	WebClient();
	if(Flag.WifiActive)
		WifiScanForSignal();
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
	Wire.begin(SDA_PIN, SCL_PIN); // Inizializza I2C per NodeMCU
	EepromInit();
	LCDInit();
	RTCInit();
	WifiInit();
	if(Flag.WifiActive)
	{
		WebServerInit();
	}
	LCDCreateIcon(WifiConnectionOn, WIFI_OK);
	LCDCreateIcon(WifiConnectionOff, WIFI_NO);
	LCDCreateIcon(OffRele, RELE_OFF);
	LCDCreateIcon(OnRele, RELE_ON);
	LCDCreateIcon(ClientConnection, CLIENT_CONN);
	LCDCreateIcon(EmptyIcon, EMPTY);
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
		MenuInit();
	}
	ClearLCD();
	LCDPrintString(ONE, CENTER_ALIGN, "Premere Ok/Set");
	LCDPrintString(TWO, CENTER_ALIGN, "per entrare nel");
	LCDPrintString(THREE, CENTER_ALIGN, "Menu Principale");
	delay(1500);
	ClearLCD();
#else
	Wire.begin(SDA, SCL);
	EepromInit();
	ClearMemory();
	LCDInit();
	if(IsMemoryEmpty())
		LCDPrintString(TWO, CENTER_ALIGN, "Memoria Vuota");

#endif // FIRST_GO
}

void loop()
{
#ifndef FIRST_GO
	EnterSetup = NO_PRESS;
	EnterSetup = CheckButtons();
	if(Flag.WifiActive)
	{
		server.handleClient();
	}
	MainScreen(EnterSetup);
#endif
}
