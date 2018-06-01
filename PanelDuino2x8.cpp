/*
	This is a library for emrelabs PanelDuino-2x8.
	PanelDuino-2x8 is an open source and open hardware project.
	
	If you want to support us please purchasing 
    products from emrelabs:
	------> emrelabs.com
	------> tindie.com/stores/emrelabs
	
	For souce codes and PCB-Schematic design:
	------> github.com/emrelabs
	Thank you! :)
*/


#include "PanelDuino2x8.h"
#include <stdio.h>
#include <string.h>
#include "Arduino.h"
#include <Wire.h>


uint8_t PanelDuino2x8::readButtons()
{
	uint8_t buttonsPos = PCF8574Read(PCF_BUTTON_ADDR);
	
	return (~buttonsPos) & 0x1F;
}


void PanelDuino2x8::begin()
{
	Wire.begin();
	delay(500);

	// Enter 4Bit mode
	write4bits(0x03 << 4);
	delayMicroseconds(4500); // wait min 4.1ms
	write4bits(0x03  << 4);
	delayMicroseconds(4500); // wait min 4.1ms
	write4bits(0x03 << 4);
	delayMicroseconds(150);
	write4bits(0x02 << 4);

	// finally, set # lines, font size, etc.
	command(LCD_FUNCTIONSET | LCD_4BITMODE | LCD_2LINE | LCD_5x8DOTS);

	// turn the display on with no cursor or blinking default
	_displaycontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
	display();

	// clear it off
	clear();

	// Initialize to default text direction (for romance languages)
	_displaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
	// set the entry mode
	command(LCD_ENTRYMODESET | _displaymode);
}

/********** high level commands, for the user! */
void PanelDuino2x8::clear()
{
	command(LCD_CLEARDISPLAY);  // clear display, set cursor position to zero
	delayMicroseconds(2000);  // this command takes a long time!
}

void PanelDuino2x8::home()
{
	command(LCD_RETURNHOME);  // set cursor position to zero
	delayMicroseconds(2000);  // this command takes a long time!
}

void PanelDuino2x8::setCursor(uint8_t col, uint8_t row)
{
	uint8_t rowOffset = (row == 0) ? 0x00 : 0x40;
	command(LCD_SETDDRAMADDR | col | rowOffset);
}

// Turn the display on/off (quickly)
void PanelDuino2x8::noDisplay()
{
	_displaycontrol &= ~LCD_DISPLAYON;
	command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void PanelDuino2x8::display()
{
	_displaycontrol |= LCD_DISPLAYON;
	command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// Turns the underline cursor on/off
void PanelDuino2x8::noCursor()
{
	_displaycontrol &= ~LCD_CURSORON;
	command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void PanelDuino2x8::cursor()
{
	_displaycontrol |= LCD_CURSORON;
	command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// Turn on and off the blinking cursor
void PanelDuino2x8::noBlink()
{
	_displaycontrol &= ~LCD_BLINKON;
	command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void PanelDuino2x8::blink()
{
	_displaycontrol |= LCD_BLINKON;
	command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// These commands scroll the display without changing the RAM
void PanelDuino2x8::scrollDisplayLeft(void)
{
	command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
}
void PanelDuino2x8::scrollDisplayRight(void)
{
	command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
}

// This is for text that flows Left to Right
void PanelDuino2x8::leftToRight(void)
{
	_displaymode |= LCD_ENTRYLEFT;
	command(LCD_ENTRYMODESET | _displaymode);
}

// This is for text that flows Right to Left
void PanelDuino2x8::rightToLeft(void)
{
	_displaymode &= ~LCD_ENTRYLEFT;
	command(LCD_ENTRYMODESET | _displaymode);
}

// This will 'right justify' text from the cursor
void PanelDuino2x8::autoscroll(void)
{
	_displaymode |= LCD_ENTRYSHIFTINCREMENT;
	command(LCD_ENTRYMODESET | _displaymode);
}

// This will 'left justify' text from the cursor
void PanelDuino2x8::noAutoscroll(void)
{
	_displaymode &= ~LCD_ENTRYSHIFTINCREMENT;
	command(LCD_ENTRYMODESET | _displaymode);
}

// Allows us to fill the first 8 CGRAM locations
// with custom characters
void PanelDuino2x8::createChar(uint8_t location, uint8_t charmap[])
{
	location &= 0x7; // we only have 8 locations 0-7
	command(LCD_SETCGRAMADDR | (location << 3));
	for (int i=0; i<8; i++)
	{
		write(charmap[i]);
	}
}

/*********** mid level commands, for sending data/cmds */

inline void PanelDuino2x8::command(uint8_t value)
{
	send(value, 0);
}

inline size_t PanelDuino2x8::write(uint8_t value)
{
	send(value, RS_BIT);
	return 1;
}

/************ low level data pushing commands **********/

// write either command or data
void PanelDuino2x8::send(uint8_t value, uint8_t mode)
{
	write4bits(((value<<0)&0xf0)|mode);
	write4bits(((value<<4)&0xf0)|mode);
}

void PanelDuino2x8::write4bits(uint8_t value)
{	
	PCF8574Write(PCF_LCD_ADDR, value);
	
    PCF8574Write(PCF_LCD_ADDR, value | EN_BIT);	// En high
	delayMicroseconds(2);		    // enable pulse must be >450ns

	PCF8574Write(PCF_LCD_ADDR, value & ~EN_BIT);	// En low
	delayMicroseconds(60);		    // commands need > 37us to settle
}

// PCF8574 funcs
void PanelDuino2x8::PCF8574Write(uint8_t addr, uint8_t data)
{
	Wire.beginTransmission(addr);
	Wire.write(data);
	Wire.endTransmission();
}

uint8_t PanelDuino2x8::PCF8574Read(uint8_t addr)
{
	Wire.requestFrom(addr, (uint8_t) 0x01);
	while (Wire.available() < 1){};
	
	return Wire.read();

}

