#define SEC_IN_DAY		   86400
#define SEC_IN_HOUR 		3600
#define SEC_IN_MINUTE 	      60
#define SEC_IN_31_GG 	 2678400
#define HOUR_IN_DAY			  23
#define MINUTE_IN_HOUR 	      59

#define PRESENT_DAY_HOUR_MINUTE_SECOND ((PresentTime.day * SEC_IN_DAY) + (PresentTime.hour * SEC_IN_HOUR) + (PresentTime.minute * SEC_IN_MINUTE) + PresentTime.second)

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
void TimeAdjust(short Hour, short Minute);
void TakePresentTime(void);