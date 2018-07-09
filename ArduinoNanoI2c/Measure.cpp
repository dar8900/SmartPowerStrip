#include "ArduinoNanoI2c.h"
#include "Measure.h"

#define 	TENSIONE_LINEA	230.0
#define 	MV_PER_A		100

double 		       EnergyMeasured;
static double      EnergyInst;
static uint32_t    EnergyTimeCounter;
static const float PowerMeasureTEST = TENSIONE_LINEA * 1.0;
// float   	PowerMeasure;
String		EnergyStr;

static float CalcCurrent()
{
	float Current = 0;
	int ReadedValue = 0;
	int MaxValue = 0;          // store max value here
	int MinValue = 1024;       // store min value here
	
	for(int cnt = 0; cnt < 500; cnt++) // legge per 50 ms
	{	
		ReadedValue = analogRead(A0);
		if (ReadedValue > MaxValue) 
		{
		   /*record the maximum sensor value*/
		   MaxValue = ReadedValue;
		}
		if (ReadedValue < MinValue) 
		{
		   /*record the maximum sensor value*/
		   MinValue = ReadedValue;
		}
	}
	Current = ((MaxValue - MinValue) * 5.0) / 1024.0; // Conversione per uscita analogica
	Current = (Current / 2.0) * 0.707; 	    // Calcolo per RMS
	Current = (Current * 1000) / MV_PER_A;  // Conversione da volt ad ampere
	return Current;
}


void CalcEnergy() // 200ms c.a.
{
	float 	CurrentCalculated;
	CurrentCalculated = CalcCurrent();
	// PowerMeasure = CurrentCalculated * TENSIONE_LINEA;
	for(int cnt = 0; cnt < 15; cnt++)
	{
		EnergyTimeCounter++;
		EnergyInst += PowerMeasureTEST;
		delay(10);
	}
}

void EnergyValueSec()
{
	EnergyMeasured += (EnergyInst / (double)EnergyTimeCounter);	
	EnergyInst = 0.0;
	EnergyTimeCounter = 0;
	EnergyStr = String(EnergyMeasured / 3600.0); // Invio la stringa già formattata per W/h
}
