#include <Wire.h>
#include "EEPROM_Ard.h"
#include "LCDLib.h"
#include "TimeLib.h"
#include "SmartPowerStrip.h"



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
// }RELE;

RELE Rele[]
{
	{false, 0, false, 0},  // RELE_1
	{false, 0, false, 0},  // RELE_2
	{false, 0, false, 0},  // RELE_3
	{false, 0, false, 0},  // RELE_4
	{false, 0, false, 0},  // RELE_5
	{false, 0, false, 0},  // RELE_6
	{false, 0, false, 0},  // RELE_7
	{false, 0, false, 0},  // RELE_8
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
	
	pinMode(RELE1, OUTPUT);
	pinMode(RELE2, OUTPUT);
	pinMode(RELE3, OUTPUT);
	pinMode(RELE4, OUTPUT);
	pinMode(RELE5, OUTPUT);
	pinMode(RELE6, OUTPUT);
	pinMode(RELE7, OUTPUT);
	pinMode(RELE8, OUTPUT);
	
	pinMode(LED, OUTPUT);
	
	LCDCreateIcon(WifiConnectionOn, WIFI_ON);
	LCDCreateIcon(WifiConnectionOff, WIFI_OFF);
	LCDCreateIcon(OffRele, RELE_OFF);
	LCDCreateIcon(OnRele, RELE_ON);
	TakePresentTime();
}

void loop() 
{


}
