// 
// 
// 

#include "LiquidCrystalSpi.h"



LiquidCrystalSpi::LiquidCrystalSpi(uint8_t pinNumber)
{
	slaveSelectPin = pinNumber;
	displayfunction = LCD_4BITMODE | LCD_1LINE | LCD_5x8DOTS;
	rs_pin = 1;
	enable_pin = 2;

	// we can't begin() yet :(
	
	
}

void LiquidCrystalSpi::begin(uint8_t cols, uint8_t rows, uint8_t charsize)
{
	
	pinMode(slaveSelectPin, OUTPUT);
	SPI.begin();
	SPIbuff = 0x80; // enables backlight

	if (rows > 1)
	{
		displayfunction |= LCD_2LINE;
	}

	numlines = rows;
	currline = 0;

	// Apparently we need to delay for a while so that the display is ready to receive commands
	delayMicroseconds(50000);

	setPin(rs_pin, LOW);
	setPin(enable_pin, LOW);

	// this is according to the hitachi HD44780 datasheet
    // figure 24, pg 46

    // we start in 8bit mode, try to set 4 bit mode
    write4bits(0x03);
    delayMicroseconds(4500); // wait min 4.1ms

    // second try
    write4bits(0x03);
    delayMicroseconds(4500); // wait min 4.1ms
    
    // third go!
    write4bits(0x03); 
    delayMicroseconds(150);

    // finally, set to 8-bit interface
    write4bits(0x02);

	// finally, set # lines, font size, etc.
	command(LCD_FUNCTIONSET | displayfunction);

	displaycontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF; 
	display();

	clear();

	displaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
	command(LCD_ENTRYMODESET | displaymode);
}

void LiquidCrystalSpi::display() {
	displaycontrol |= LCD_DISPLAYON;
	command(LCD_DISPLAYCONTROL | displaycontrol);
}

void LiquidCrystalSpi::noDisplay() {
	displaycontrol &= ~LCD_DISPLAYON;
	command(LCD_DISPLAYCONTROL | displaycontrol);
}

void LiquidCrystalSpi::noCursor() {
	displaycontrol &= ~LCD_CURSORON;
	command(LCD_DISPLAYCONTROL | displaycontrol);
}

void LiquidCrystalSpi::cursor() {
	displaycontrol |= LCD_CURSORON;
	command(LCD_DISPLAYCONTROL | displaycontrol);
}

void LiquidCrystalSpi::noBlink() {
  displaycontrol &= ~LCD_BLINKON;
  command(LCD_DISPLAYCONTROL | displaycontrol);
}
void LiquidCrystalSpi::blink() {
  displaycontrol |= LCD_BLINKON;
  command(LCD_DISPLAYCONTROL | displaycontrol);
}

void LiquidCrystalSpi::scrollDisplayLeft(void) {
  command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
}
void LiquidCrystalSpi::scrollDisplayRight(void) {
  command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
}

// This is for text that flows Left to Right
void LiquidCrystalSpi::leftToRight(void) {
  displaymode |= LCD_ENTRYLEFT;
  command(LCD_ENTRYMODESET | displaymode);
}

// This is for text that flows Right to Left
void LiquidCrystalSpi::rightToLeft(void) {
  displaymode &= ~LCD_ENTRYLEFT;
  command(LCD_ENTRYMODESET | displaymode);
}

// This will 'right justify' text from the cursor
void LiquidCrystalSpi::autoscroll(void) {
  displaymode |= LCD_ENTRYSHIFTINCREMENT;
  command(LCD_ENTRYMODESET | displaymode);
}

// This will 'left justify' text from the cursor
void LiquidCrystalSpi::noAutoscroll(void) {
  displaymode &= ~LCD_ENTRYSHIFTINCREMENT;
  command(LCD_ENTRYMODESET | displaymode);
}

void LiquidCrystalSpi::clear() {
	command(LCD_CLEARDISPLAY);  // clear display, set cursor position to zero
	delayMicroseconds(2000);  // this command takes a long time!
}

// Allows us to fill the first 8 CGRAM locations
// with custom characters
void LiquidCrystalSpi::createChar(uint8_t location, uint8_t charmap[]) {
  location &= 0x7; // we only have 8 locations 0-7
  command(LCD_SETCGRAMADDR | (location << 3));
  for (int i=0; i<8; i++) {
    write(charmap[i]);
  }
}

void LiquidCrystalSpi::command(uint8_t data)
{
	send(data, LOW);
}

void LiquidCrystalSpi::setBacklight(uint8_t status)
{
	setPin(7, status);
}

void LiquidCrystalSpi::setCursor(uint8_t col, uint8_t row)
{
	int row_offsets[] = { 0x00, 0x40, 0x14, 0x54 };
	if ( row > numlines ) {
		row = numlines-1;    // we count rows starting w/0
	}
  
	command(LCD_SETDDRAMADDR | (col + row_offsets[row]));
}
	
void LiquidCrystalSpi::send(uint8_t value, uint8_t mode)
{
	setPin(rs_pin, mode);
	write4bits(value >> 4);
	write4bits(value);
}

void LiquidCrystalSpi::pulseEnable()
{
	setPin(enable_pin, LOW);
	delayMicroseconds(1);
	setPin(enable_pin, HIGH);
	delayMicroseconds(1);
	setPin(enable_pin, LOW);
	delayMicroseconds(50);
}

void LiquidCrystalSpi::write4bits(uint8_t value)
{
	for (int i = 0; i < 4; i++)
	{
		insertToSpiBuf( 6 - i, (value >> i) & 0x01);
	}
	
	writeByte(SPIbuff);
	pulseEnable();
}

void LiquidCrystalSpi::insertToSpiBuf(uint8_t bit, uint8_t value)
{
	if (value == HIGH)
	{
		SPIbuff |= (1 << bit);
	}
	else
	{
		SPIbuff &= ~(1 << bit);
	}
}

void LiquidCrystalSpi::setPin(uint8_t pin, uint8_t value)
{
	insertToSpiBuf(pin, value);

	writeByte(SPIbuff);
}

void LiquidCrystalSpi::writeByte(uint8_t data)
{
	digitalWrite(slaveSelectPin, LOW);
	SPI.transfer((byte)data);
	digitalWrite(slaveSelectPin, HIGH);
}


size_t LiquidCrystalSpi::write(uint8_t data)
{
	send(data, HIGH);
	return 1;
}



