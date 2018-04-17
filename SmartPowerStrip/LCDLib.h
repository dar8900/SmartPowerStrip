#include <LiquidCrystal_I2C.h> // Libreria LCD I2C
#include <Arduino.h>

#define MAX_LCD_ROW    3
#define MAX_LCD_COL   19

#define CENTER_ALIGN  10
#define RIGHT_ALIGN    19
#define LEFT_ALIGN     0



void LCDInit(void);
void LCDMoveCursor(short row, short col);
void ClearLCD(void);
void LcdTimeWrite(int Time2Write);
void LCDPrintString(short row, short col, String string);
void LCDPrintValue(short row, short col, short value);
void LCDPrintLineVoid(short row);
bool LCDCreateIcon(uint8_t Icon[], short IconNum);
void LCDShowIcon(short IconNum);
void LCDDisplayOn(void);
void LCDDisplayOff(void);
void LCDBlink(void);
void LCDNoBlink(void);
