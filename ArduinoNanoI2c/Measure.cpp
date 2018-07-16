#include "ArduinoNanoI2c.h"
#include "Measure.h"

const float TensioneLinea  = 230.0;
const float MVoltPerAmpere = 100.0;

float 			  CurrentCalculated;
float 		      EnergyMeasured;
static float      EnergyAcc;
static uint16_t   EnergyAccCnt;
float   	      PowerMeasure;

String		      EnergyStr;
String		      CurrentStr;

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
	float mVolt = 0.0, Current = 0.0;
	uint32_t CurrentAcc = 0;
	uint16_t ReadedValue = 0, AdcOffset = ZERO_CURRENT_ANALOG_VALUE;
	// uint8_t TmpCnt = 0;
	// uint16_t MaxVal = 0, MinVal = 1023;
	
	for(int cnt = 0; cnt < N_CAMPIONI_CORRENTE; cnt++) // legge per 200 ms (10 periodi di rete a 50 Hz)
	{	
		ReadedValue = (analogRead(A0) - AdcOffset);
		CurrentAcc += (uint32_t)(ReadedValue * ReadedValue);		
	}
	mVolt = ((sqrt((float)CurrentAcc / (float)N_CAMPIONI_CORRENTE))) * 4.9;
	Current = (mVolt / MVoltPerAmpere);
	
	// mVolt = ((MaxVal - MinVal) * 5.0) / 1023.0;
	// mVolt = (mVolt / 2.0) * 0.707;
	// Current = (mVolt * 1000.0) / MV_PER_A;

	return Current;
}

// static float CalcCurrent()
// {
	// float Current = 0.0;
	// float mVolt = 0.0;
	// uint16_t ReadedValue = 0;
	// uint8_t TmpCnt = 0;
	// uint16_t MaxVal = 0, MinVal = 1023;
	
	// for(int cnt = 0; cnt < N_CAMPIONI_CORRENTE; cnt++) // legge per 80 ms (4 periodi di rete a 50 Hz)
	// {	
		// ReadedValue = analogRead(A0);
       // // see if you have a new maxValue
       // if (ReadedValue > MaxVal) 
       // {
           // /*record the maximum sensor value*/
           // MaxVal = ReadedValue;
       // }
       // if (ReadedValue < MinVal) 
       // {
           // /*record the maximum sensor value*/
           // MinVal = ReadedValue;
       // }
	// }
	// mVolt = ((MaxVal - MinVal) * 5.0) / 1023.0;
	// mVolt = (mVolt / 2.0) * 0.707;
	// Current = (mVolt * 1000.0) / MV_PER_A;

	// return Current;
// }


void CalcEnergy() // 100ms ca
{
	CurrentCalculated = CalcCurrent();
	PowerMeasure = CurrentCalculated * (float)TENSIONE_LINEA;
	EnergyAcc += PowerMeasure;
	EnergyAccCnt++;
}

void EnergyValueSec()
{
	EnergyMeasured += (EnergyAcc / (float)EnergyAccCnt);	
	EnergyAccCnt = 0;
	EnergyAcc = 0.0;
	EnergyStr = String(EnergyMeasured); // Invio la stringa formattata in W/s
	CurrentStr = String(CurrentCalculated);
}
