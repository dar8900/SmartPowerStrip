typedef enum
{
	MANUAL_RELE = 0,
	CHANGE_TIME_BAND,
	HELP_INFO,
	WIFI_INFO,
	ASSIGN_RELE_TIMER
}MENU_NUMBER;

typedef struct
{
	MENU_NUMBER  MenuNumber;
	bool (*MenuFunc)(void);
	
}MENU_VOICES;




void MainScreen(void);
void MainMenu(void);
bool ManualRele(void);
bool ChangeTimeBand(void);
bool HelpInfo(void);
bool WiFiInfo(void);
bool AssignReleTimer(void);
bool SetTimer(short Rele)
bool ReleInit(void);
void TakeReleTime(short Rele);