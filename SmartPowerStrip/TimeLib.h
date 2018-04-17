
typedef struct
{
	short hour;
	short minute;
	short day;
	short month;
	short year;
} TIME_DATE_FORMAT;


void RTCInit(void);
void TakePresentTime(void);