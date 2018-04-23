
#define MAX_TIME_TIMER		14401



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
	uint32_t TurnOnTime;
	uint32_t ActiveTime;
	bool HaveTimer;
	int32_t TimerTime;	
	short EepromAddr;
}RELE;

bool ReleInit(bool FirstGo);
void ReleReStart(void);
void TakeReleTime(void);
void ShowReleIcons(short Row);
void TurnOffAllRele(void);
void TurnOnAllRele(void);
void CheckReleStatus(void);
bool SetTimerRele(short ReleNbr);
short ReleIdx2Pin(short ReleIndx);
