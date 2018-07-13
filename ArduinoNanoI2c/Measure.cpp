#include "ArduinoNanoI2c.h"
#include "Measure.h"



#define REAL_MEASURE		

float 			  CurrentCalculated;
float 		      EnergyMeasured;
static float      EnergyAcc;

#ifndef REAL_MEASURE
static const float PowerMeasureTEST = TENSIONE_LINEA * 0.5;
#endif

#ifdef REAL_MEASURE
float   	PowerMeasure;
#endif

String		EnergyStr;
String		CurrentStr;

const uint16_t SimWave[10] =
{
	210, 
	367,
	587,
	698,
	1000,
	987,
	862,
	347,
	999,
	1023,
}; 

static float CalcCurrent()
{
	float Current = 0.0;
	float mVolt = 0.0;
	uint16_t ReadedValue = 0;
	uint8_t TmpCnt = 0;
	
	for(int cnt = 0; cnt < N_CAMPIONI_CORRENTE; cnt++) // legge per 80 ms (4 periodi di rete a 50 Hz)
	{	
		if((TmpCnt % 10) == 0)
			TmpCnt = 0;
		Current += (0.049 * SimWave[TmpCnt] - 25.0) / N_CAMPIONI_CORRENTE;
		delayMicroseconds(100);
		TmpCnt++;
	}
	return Current;
}


void CalcEnergy() // 200ms c.a.
{
#ifdef REAL_MEASURE
	CurrentCalculated = CalcCurrent();
	PowerMeasure = CurrentCalculated * TENSIONE_LINEA;
#endif
	for(int cnt = 0; cnt < N_CAMPIONI_ENERGIA; cnt++)
	{
#ifndef REAL_MEASURE
		EnergyAcc += PowerMeasureTEST;
#else
		EnergyAcc += PowerMeasure;
#endif
		delay(3);
	}
}

void EnergyValueSec()
{
	EnergyMeasured += (EnergyAcc / N_CAMPIONI_ENERGIA);	
	EnergyAcc = 0.0;
	EnergyStr = String(EnergyMeasured); // Invio la stringa già formattata per W/h
	CurrentStr = String(CurrentCalculated);
}
