#ifndef EEPROM_ARD_H
#define EEPROM_ARD_H

#include <Arduino.h>

#undef  NEW_EEPROM

#ifndef NEW_EEPROM
#define MAX_EEPROM_DIM	      EEPROM.length()
#else
#define MAX_EEPROM_DIM_STM			    32766
#endif

#define MAX_EEPROM_DIM_NODEMCU			  512

#define MAX_CELL_EEPROM	                  254

typedef enum
{
	RELE_1_ADDR = 0,					
	RELE_2_ADDR,					
	RELE_3_ADDR,					
	RELE_4_ADDR,								
	RELE_5_ADDR,                    
	RELE_6_ADDR,                    
	RELE_7_ADDR,                    
	RELE_8_ADDR                     
}RELE_EEPROM_ADDR;

#define FLAG_VALUE_ADDR					   50

#define FIRST_START_CHECK_ADDR			  100


void EepromUpdate(short address, short value);
void EepromInit(void);
int WriteMemory(short address, short value);
bool ReadMemory(short address, short numReg, short *value);
bool ClearMemory(void);
bool IsMemoryEmpty(void);




#endif
