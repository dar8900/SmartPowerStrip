#define INVALID_BAND_VALUE 	99

enum
{
	INIT_HOUR = 0,
	INIT_MINUTE,
	END_HOUR,
	END_MINUTE,
	EXIT
};


typedef struct
{
	short InitHour;
	short InitMinute;
	short InitDay;
	short EndHour;
	short EndMinute;
	short EndDay;

} BAND_FORMAT;

bool IsBandCorrect(void);
bool CheckBand(void);
void SetBandInvalid(void);
void SaveBandValues(short WichItem, short Value);
void ReadBandValues(short WichItem);
bool SetTimeBand(void);