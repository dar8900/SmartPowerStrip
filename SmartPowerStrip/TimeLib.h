#define SEC_IN_HOUR 		3600
#define SEC_IN_MINUTE 	      60
#define SEC_IN_31_GG 	 2678400



typedef struct
{
	short hour;
	short minute;
	short second;
	short day;
	short month;
	short year;
} TIME_DATE_FORMAT;


void RTCInit(void);
void TakePresentTime(void);