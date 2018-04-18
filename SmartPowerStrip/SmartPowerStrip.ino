#include <Wire.h>
#include "EEPROM_Ard.h"
#include "LCDLib.h"
#include "TimeLib.h"
#include "SmartPowerStrip.h"
#include "Web.h"
#include "Menu.h"
#include "Band.h"
#include "Rele.h"



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
	0x00,
	0x0A,
	0x00,
	0x04,
	0x00,
	0x0E,
	0x11,
	0x00
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

// typedef struct
// {
	// bool IsActive;
	// uint32_t ActiveTime;
	// bool HaveTimer;
	// int TimerTime;	
	// short EepromAddr;	
// }RELE;

RELE Rele[]
{
	{false, 0, false, 0, RELE_1_ADDR},  // RELE_1
	{false, 0, false, 0, RELE_2_ADDR},  // RELE_2
	{false, 0, false, 0, RELE_3_ADDR},  // RELE_3
	{false, 0, false, 0, RELE_4_ADDR},  // RELE_4
	{false, 0, false, 0, RELE_5_ADDR},  // RELE_5
	{false, 0, false, 0, RELE_6_ADDR},  // RELE_6
	{false, 0, false, 0, RELE_7_ADDR},  // RELE_7
	{false, 0, false, 0, RELE_8_ADDR},  // RELE_8
};

FLAGS Flag;

void BlinkLed(short pin)
{
	ON(pin);
	delay(5);
	OFF(pin);	
}

void setup() 
{
	Wire.begin(SDA, SCL); // Inizializza I2C per NodeMCU
	EepromInit();
	LCDInit();
	RTCInit();
	WifiInit();
	
	pinMode(RELE1, OUTPUT);
	pinMode(RELE2, OUTPUT);
	pinMode(RELE3, OUTPUT);
	pinMode(RELE4, OUTPUT);
	pinMode(RELE5, OUTPUT);
	pinMode(RELE6, OUTPUT);
	pinMode(RELE7, OUTPUT);
	pinMode(RELE8, OUTPUT);
	
	pinMode(BUTTON_LED, OUTPUT);
	
	LCDCreateIcon(WifiConnectionOn, WIFI_OK);
	LCDCreateIcon(WifiConnectionOff, WIFI_NO);
	LCDCreateIcon(OffRele, RELE_OFF);
	LCDCreateIcon(OnRele, RELE_ON);
	TakePresentTime();
	ReleInit();
	SetBandInvalid();
}

void loop() 
{
	if(CheckBand)
	{
		if(!Flag.AllReleDown)
			TurnOffAllRele();
		LCDDisplayOff();
		Flag.ReleRS = false;
	}
	MainScreen();
}
