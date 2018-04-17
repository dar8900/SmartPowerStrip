#include <Wire.h>
#include "EEPROM_Ard.h"
#include "LCDLib.h"
#include "TimeLib.h"
#include "SmartPowerStrip.h"

uint8_t WifiConnection[]
{
	0x15,
	0x1D,
	0x1D,
	0x00,
	0x00,
	0x1D,
	0x19,
	0x11
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
	
	LCDCreateIcon(WifiConnection, 0);
	LCDCreateIcon(OffRele, 1);
	LCDCreateIcon(OnRele, 2);
	TakePresentTime();
}

void loop() 
{


}
