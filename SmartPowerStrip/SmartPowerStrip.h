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

#define LED  D10
#define ANALOG	A0

typedef enum
{
	WIFI_ON = 0,
	WIFI_OFF,
	RELE_OFF,
	RELE_ON
}ICON_NUMB;


typedef enum
{
	RELE_1 = 0,
	RELE_2,
	RELE_3,
	RELE_4,
	RELE_5,
	RELE_6,
	RELE_7,
	RELE_8,
	RELE_MAX
}RELE_NUMB;

typedef struct
{
	bool IsActive;
	uint32_t ActiveTime;
	bool HaveTimer;
	int TimerTime;		
}RELE;

typedef struct
{
	bool EnterSetup;
}FLAGS;



void BlinkLed(short pin);