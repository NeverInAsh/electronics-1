This is a fairly straightforward emulation of the Adafruit-modified version of the LiquidCrystal library 
(I know, I'm bad I didn't fork it on GitHub).

It's got the same interface, but it doesn't do i2c, or support directly bit-banging the pins on the 
controller, but for all that, it does now work with the hardware SPI on your micro.

Worth keeping in mind, though, is that it will set the SPI to full speed when created, so it could
interfere with other devices that use the SPI controller. My next step will probably be to make each
function preserve the current state of the SPCR register, set it to full speed, do the work, and then 
restore it so that it doesn't intefere with other devices.
