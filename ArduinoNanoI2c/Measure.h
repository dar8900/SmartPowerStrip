#include <Arduino.h>

#define 	TENSIONE_LINEA	230.0
#define 	MV_PER_A		100.0
#define 	SENSOR_NOISE	11
#define 	MIN_CURRENT		(SENSOR_NOISE/MV_PER_A)
#define 	ZERO_CURRENT_ANALOG_VALUE	515
#define 	N_CAMPIONI_CORRENTE	2000
#define 	N_CAMPIONI_ENERGIA	40

void CalcEnergy(void);
void EnergyValueSec(void);