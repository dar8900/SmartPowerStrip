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
	400, 
	420,
	430,
	440,
	460,
	500,
	520,
	530,
	550,
	600,
}; 

static float CalcCurrent()
{
	float Current = 0.0;
	float mVolt = 0.0;
	uint16_t ReadedValue = 0;
	uint8_t TmpCnt = 0;
	uint16_t MaxVal = 0, MinVal = 1023;
	
	for(int cnt = 0; cnt < N_CAMPIONI_CORRENTE; cnt++) // legge per 80 ms (4 periodi di rete a 50 Hz)
	{	
		ReadedValue = analogRead(A0);
       // see if you have a new maxValue
       if (ReadedValue > MaxVal) 
       {
           /*record the maximum sensor value*/
           MaxVal = ReadedValue;
       }
       if (ReadedValue < MinVal) 
       {
           /*record the maximum sensor value*/
           MinVal = ReadedValue;
       }
	}
	mVolt = ((MaxVal - MinVal) * 5.0) / 1023.0;
	mVolt = (mVolt / 2.0) * 0.707;
	Current = (mVolt * 1000.0) / MV_PER_A;

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
