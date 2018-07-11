#include <Arduino.h>

#define 	TENSIONE_LINEA	230.0
#define 	MV_PER_A		100
#define 	SENSOR_NOISE	11
#define 	MIN_CURRENT		(SENSOR_NOISE/MV_PER_A)
#define 	ZERO_CURRENT_ANALOG_VALUE	512
#define 	N_CAMPIONI_CORRENTE	800
#define 	N_CAMPIONI_ENERGIA	40

void CalcEnergy(void);
void EnergyValueSec(void);