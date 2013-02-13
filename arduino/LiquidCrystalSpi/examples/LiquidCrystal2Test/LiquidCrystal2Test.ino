#include <SPI.h>
#include "LiquidCrystalSpi.h"

LiquidCrystalSpi lcd(10);

int count = 0;

void setup()
{

  lcd.begin(20, 4);
  lcd.setBacklight(HIGH);
  lcd.setCursor(0,0);
  lcd.print("hello, world!");
}

void loop()
{
	lcd.setCursor(0,1);
	lcd.print(millis()/1000);
	delay(1000);
  
  
  
  
}
