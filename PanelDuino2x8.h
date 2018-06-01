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



#ifndef PANELDUINO2X8_H
#define PANELDUINO2X8_H

#include "Print.h"
#include <inttypes.h>



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


// buttons
#define BUTTON_UP    (1 << 0)
#define BUTTON_OK    (1 << 1)
#define BUTTON_DOWN  (1 << 2)
#define BUTTON_LEFT  (1 << 4)
#define BUTTON_RIGHT (1 << 3)

// i2c address
#define PCF_BUTTON_ADDR (0x20)
#define PCF_LCD_ADDR    (0x21)

#define RS_BIT (1 << 0)
#define RW_BIT (1 << 1)
#define EN_BIT (1 << 2)

#define COL_COUNT (8)
#define ROW_COUNT (2)

class PanelDuino2x8 : public Print 
{
public:
    void begin();
	uint8_t readButtons();
	
	void clear();
	void home();

	void noDisplay();
	void display();
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

	void createChar(uint8_t, uint8_t[]);
	void setCursor(uint8_t, uint8_t); 
	virtual size_t write(uint8_t);
	void command(uint8_t);
	
private:
	uint8_t _displaycontrol;
	uint8_t _displaymode;
	uint8_t _lastDisplayData;

	void send(uint8_t value, uint8_t mode);
	void write4bits(uint8_t value);
	
	void PCF8574Write(uint8_t addr, uint8_t data);
	uint8_t PCF8574Read(uint8_t addr);
};


#endif // PANELDUINO2X8_H

