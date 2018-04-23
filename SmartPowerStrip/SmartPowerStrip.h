#include <stdint.h>
#define  OFF(pin)  digitalWrite(pin, LOW)
#define  ON(pin)   digitalWrite(pin, HIGH)

#define READ(pin)  digitalRead(pin)


#define SDA   5
#define SCL   4

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
	ALARM
}ICON_NUMB;



typedef struct
{
	bool IsDisplayOn;
	bool BandActive;
	bool BandInvalid;
	bool IsBandSetted;
	bool WifiActive;
	bool ClientConnected;
	bool AllReleDown;
	bool ReleRS;
}FLAGS;



void BlinkLed(short pin);
void CheckEvents(void);