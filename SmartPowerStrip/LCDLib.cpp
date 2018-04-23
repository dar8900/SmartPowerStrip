#include "LCDLib.h"
#include "SmartPowerStrip.h"
LiquidCrystal_I2C lcd_main(0x27, 20,4);

uint8_t AlarmIcon[]
{
	0x0E,
	0x15,
	0x15,
	0x15,
	0x11,
	0x15,
	0x11,
	0x0E
};

void LCDInit()
{
	lcd_main.begin();	
}

void ClearLCD()
{
    //delay(10);
    lcd_main.clear();
    //lcd_main.home();
}

void LCDMoveCursor(short row, short col)
{
	lcd_main.setCursor(col, row);
}

void BlinkDisplay(short NumTimes)
{
	short num = 0;
	for(num = 0; num < NumTimes; num++)
	{
		LCDDisplayOn();
		delay(400);
		LCDDisplayOff();
		delay(400);		
	}
	LCDDisplayOn();
}

void LCDShowPopUp(String Avviso)
{
	ClearLCD();
	LCDPrintString(2, CENTER_ALIGN, Avviso);
	BlinkDisplay(5);
	delay(2000);
	ClearLCD();
}


void LcdTimeWrite(int Time2Write)
{
  int cnt = Time2Write;
  while(cnt != 0)
  {
    LCDPrintValue(1, CENTER_ALIGN, cnt);
    delay(1000);
    ClearLCD();
    cnt--;
  }  

  LCDPrintString(1, CENTER_ALIGN, "The light is going");
  LCDPrintString(2, CENTER_ALIGN, "to turn off");
  delay(3000);
  ClearLCD();
}

// Utilizzano un oggetto di tipo LCD
void LCDPrintString(short row, short col, String string) 
{
  if(row > MAX_LCD_ROW || string.length() > 20) 
  {
	lcd_main.clear();
	if(string.length() > 20)
	{
		col = CENTER_ALIGN;
		string = "STRING TOO BIG";  		
	}
	else
	{
		col = CENTER_ALIGN;
		row = 3;
		string = "OVER DIMENSION"; 		
	}
  
  }
  switch(col)
  {
    case CENTER_ALIGN:
      col = ((MAX_LCD_COL+1) - string.length()) / 2;
      break;
    case RIGHT_ALIGN:
      col = (MAX_LCD_COL+1) - string.length();
      break;
    case LEFT_ALIGN:
      col = 0;
      break;
    default:
      lcd_main.home();
      break;
  }
  lcd_main.setCursor(col, row);
  lcd_main.print(string);

}

void LCDPrintValue(short row, short col, short value)
{
  String ValStr = String(value);
  if(row > MAX_LCD_ROW || ValStr.length() > 20)
  {
	lcd_main.clear();
	col = CENTER_ALIGN;
	row = 3;
    ValStr = "OVER DIMENSION"; 
    return;
  }
  
  switch(col)
  {
    case CENTER_ALIGN:
      col = ((MAX_LCD_COL+1) - ValStr.length()) / 2;
      break;
    case RIGHT_ALIGN:
      col = (MAX_LCD_COL+1) - ValStr.length();
      break;
    case LEFT_ALIGN:
      col = 0;
      break;
    default:
      lcd_main.home();
      break;
  }
  lcd_main.setCursor(col, row);
  lcd_main.print(ValStr);
}

bool LCDCreateIcon(uint8_t Icon[], short IconNum)
{
	bool Created = false;
	if(IconNum < 7)
	{
		lcd_main.createChar(IconNum, Icon);
		Created = true;
	}
	else
	{
		Created = false;
	}
	return Created;
}

void LCDShowIcon(short IconNum)
{
	lcd_main.write(IconNum);	
}

void LCDPrintLineVoid(short row)
{
  lcd_main.setCursor(0, row);
  lcd_main.print("                    ");
}

void LCDDisplayOn()
{
	lcd_main.backlight();
}

void LCDDisplayOff()
{
	lcd_main.noBacklight();
}

void LCDBlink()
{
	lcd_main.blink();
}

void LCDNoBlink()
{
	lcd_main.noBlink();
}