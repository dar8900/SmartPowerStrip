typedef enum
{
	MANUAL_RELE = 0,
	CHANGE_TIME_BAND,
	HELP_INFO,
	WIFI_INFO,
	ASSIGN_RELE_TIMER,
	MAX_MENU_ITEM
}MENU_NUMBER;

typedef struct
{
	bool (*MenuFunc)(void);
	String MenuTitle;
	
}MENU_VOICES;

enum
{
	STATUS_OFF = 0,
	STATUS_ON
};

bool SetupInterrupt(void);
void MainScreen(void);
void MainMenu(void);
bool ManualRele(void);
bool ChangeTimeBand(void);
bool HelpInfo(void);
bool WiFiInfo(void);
bool AssignReleTimer(void);
bool SetTimer(short Rele);
bool ReleInit(void);
void TakeReleTime(short Rele);