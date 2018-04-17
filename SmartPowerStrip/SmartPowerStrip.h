#include <stdint.h>
#define  OFF(pin)  digitalWrite(pin, LOW)
#define  ON(pin)   digitalWrite(pin, HIGH)

#define READ(pin)  digitalRead(pin)


#define SDA  D1
#define SCL  D2
#define LED  D8

typedef enum
{
	RELE_1 = 0,
	RELE_2,
	RELE_3,
	RELE_4,
	RELE_5,
	RELE_6,
	RELE_7,
	RELE_8
}RELE_NUMB;

typedef struct
{
	bool IsActive;
	uint32_t ActiveTime;
	bool HaveTimer;
	int TimerTime;		
}RELE;


void BlinkLed(short pin);