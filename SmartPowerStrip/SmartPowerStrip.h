#include <stdint.h>
#define  OFF(pin)  digitalWrite(pin, LOW)
#define  ON(pin)   digitalWrite(pin, HIGH)

#define READ(pin)  digitalRead(pin)


#define SDA   D1
#define SCL   D2

#define RELE1 D0
#define RELE2 D3
#define RELE3 D4
#define RELE4 D5
#define RELE5 D6
#define RELE6 D7
#define RELE7 D8
#define RELE8 D9

#define BUTTON_LED  D10
#define ANALOG	A0

typedef enum
{
	WIFI_OK = 0,
	WIFI_NO,
	RELE_OFF,
	RELE_ON
}ICON_NUMB;



typedef struct
{
	bool EnterSetup;
	bool BandActive;
	bool BandInvalid;
	bool WifiActive;
	bool ClientConnected;
	bool AllReleDown;
	bool ReleRS;
}FLAGS;



void BlinkLed(short pin);