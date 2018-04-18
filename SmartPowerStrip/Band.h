#define INVALID_BAND_VALUE 	99

enum
{
	HOUR = 0,
	MINUTE,
	EXIT
};


bool CheckBand(void);
void SetBandInvalid(void);
void SaveBandValues(short WichItem, short Value);
void ReadBandValues(short WichItem);
bool SetTimeBand(void);