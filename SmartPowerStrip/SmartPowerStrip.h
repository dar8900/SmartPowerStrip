#ifndef SMART_POWER_STRIP_H
#define SMART_POWER_STRIP_H
#include <stdint.h>
#define  OFF(pin)  digitalWrite(pin, LOW)
#define  ON(pin)   digitalWrite(pin, HIGH)

#define READ(pin)  digitalRead(pin)


#define SDA_PIN   5
#define SCL_PIN   4

#define RELE1  0
#define RELE2  2
#define RELE3 14
#define RELE4 12
#define RELE5 13
#define RELE6 15
#define RELE7  3
#define RELE8  1

#define BUTTON_LED  10

typedef enum
{
	WIFI_OK = 0,
	WIFI_NO,
	RELE_OFF,
	RELE_ON,
	CLIENT_CONN,
	EMPTY
}ICON_NUMB;



typedef struct
{
	bool IsDisplayOn;
	bool BandActive;
	bool InBand;
	bool IsBandSetted;
	bool WifiActive;
	bool WifiReconnect;
	bool WpsConfigSelected;
	bool ClientConnected;
	bool AllReleDown;
	bool AllReleUp;
	bool ReleRS;
}FLAGS;


bool IsRebooted(void);
void BlinkLed(short pin);
void CheckEvents(void);
void delayms(short ms);
#endif