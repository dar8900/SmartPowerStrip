
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
	short hour;
	short minute;
	short second;
	short day;
}RELE_TIME_FORMAT;

enum
{
	TURN_ON_TIME = 0,
	TIMER_TIME
};

typedef enum
{
	STATUS_OFF = 0,
	STATUS_ON
} RELE_STATUS;

typedef struct
{
	bool IsActive;
	RELE_TIME_FORMAT TurnOnTime;
	RELE_TIME_FORMAT ActiveTime;
	bool HaveTimer;
	RELE_TIME_FORMAT TimerTime;
	short EepromAddr;
}RELE;

void SaveReleStatus(short ReleIndx, short Status);

bool ReleInit(bool FirstGo);
void ReleReStart(void);
void TakeReleTime(void);
void ShowReleIcons(short Row);
void TurnOffAllRele(void);
void TurnOnAllRele(void);
void CheckReleStatus(void);
bool SetTimerRele(short ReleNbr);
RELE_TIME_FORMAT SetTimeVarRele(short Hour, short Minute, short Second, short Day);
short ReleIdx2Pin(short ReleIndx);
