#include "ArduinoNanoI2c.h"
#include "Measure.h"



#define REAL_MEASURE		

float 		      EnergyMeasured;
static float      EnergyAcc;

#ifndef REAL_MEASURE
static const float PowerMeasureTEST = TENSIONE_LINEA * 0.5;
#endif

#ifdef REAL_MEASURE
float   	PowerMeasure;
#endif

String		EnergyStr;

static float CalcCurrent()
{
	float Current = 0.0;
	float mVolt = 0.0;
	uint16_t ReadedValue = 0;
	uint64_t RmsAnalog = 0;
	
	for(int cnt = 0; cnt < N_CAMPIONI_CORRENTE; cnt++) // legge per 80 ms (4 periodi di rete a 50 Hz)
	{	
		ReadedValue = analogRead(A0);
		if(ReadedValue < ZERO_CURRENT_ANALOG_VALUE)
		{
			ReadedValue = (1023 - ReadedValue);
		}
		RmsAnalog += (ReadedValue * ReadedValue);
	}
	RmsAnalog /= N_CAMPIONI_CORRENTE;
	mVolt = sqrt(RmsAnalog);
	
	mVolt = (mVolt * 5.0) / 1023.0; // Conversione per uscita analogica
	Current = (mVolt / MV_PER_A);  // Conversione da mVolt ad Ampere
	return Current;
}


void CalcEnergy() // 200ms c.a.
{
	float 	CurrentCalculated;
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
	EnergyStr = String(EnergyMeasured / 3600.0); // Invio la stringa già formattata per W/h
}
