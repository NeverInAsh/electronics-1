// LiquidCrystalSpi.h

#ifndef _LIQUIDCRYSTALSPI_h
#define _LIQUIDCRYSTALSPI_h

#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

#include <SPI.h>
#include "Print.h"


// commands
#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

// flags for display entry mode
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// flags for display on/off control
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

// flags for display/cursor shift
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE 0x00
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00

// flags for function set
#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_1LINE 0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

class LiquidCrystalSpi : public Print
{
 private:
	uint8_t slaveSelectPin;
	
	void setPin(uint8_t pin, uint8_t value);
	void insertToSpiBuf(uint8_t bit, uint8_t value);
	void pulseEnable();
	void write4bits(uint8_t value);
	void send(uint8_t, uint8_t);

	uint8_t rs_pin; // LOW: command.  HIGH: character.
	uint8_t enable_pin; // activated by a HIGH pulse.

	uint8_t displayfunction;
	uint8_t displaycontrol;
	uint8_t displaymode;

	uint8_t initialized;

	uint8_t numlines, currline;

	uint8_t SPIbuff;

 public:
	LiquidCrystalSpi(uint8_t);
	void writeByte(uint8_t);
	void begin(uint8_t cols, uint8_t rows, uint8_t charsize = LCD_5x8DOTS);
	void command(uint8_t);
	
	void display();
	void noDisplay();
	void clear();
	void noBlink();
	void blink();
	void noCursor();
	void cursor();
	void scrollDisplayLeft();
	void scrollDisplayRight();
	void leftToRight();
	void rightToLeft();
	void autoscroll();
	void noAutoscroll();

	void setBacklight(uint8_t status);
	void setCursor(uint8_t, uint8_t);
	void createChar(uint8_t, uint8_t[]);



#if ARDUINO >= 100
	virtual size_t write(uint8_t);
#else
	virtual void write(uint8_t);
#endif
};



#endif

