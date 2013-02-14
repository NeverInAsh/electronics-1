#include <SPI.h>
#include <LiquidCrystalSpi.h>


// Create an instance of the display controller class, using Pin 10 as the
// CS pin. You'll see that it behaves pretty much the same as the LiquidCrystal 
// class, it's just been optimized a little because it doesn't have to do
// i2c or bit-banging.
LiquidCrystalSpi lcd(10);

void setup()
{

  // Setup the LCD as having 20 cols and 4 rows
  lcd.begin(20, 4);
  
  // Turn on the backlight
  lcd.setBacklight(HIGH);
  
  // Set the cursor to the start
  lcd.setCursor(0,0);
  
  // And print some text
  lcd.print("hello, world!");
}

void loop()
{
	// Set the cursor to the start of the 2nd line
	lcd.setCursor(0,1);
	
	// Print the number of seconds since the micro powered up
	lcd.print(millis()/1000);
	
	// Wait a while.
	delay(1000); 
	
	// Just like normal see?
}
