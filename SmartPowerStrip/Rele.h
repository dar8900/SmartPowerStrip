
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
	uint32_t ActiveTime;
	bool HaveTimer;
	int32_t TimerTime;	
	short EepromAddr;
}RELE;

bool ReleInit(void);
void ReleReStart(void);
void TakeReleTime(void);
void ShowReleIcons(void);
void TurnOffAllRele(void);
void TurnOnAllRele(void);
bool SetTimerRele(short ReleNbr);
