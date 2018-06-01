
/* emrelabs PanelDuino-2x8
 * example codes
 * 
 *     Please select:
 *     Arduino Pro Mini
 *     5V - 16Mhz
 * 
 * emrelabs.com 
 */

#include <Wire.h>
#include <PanelDuino2x8.h>


PanelDuino2x8 panelDuino;

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);

  // init lcd & buttons
  panelDuino.begin();

  //Print a message.
  panelDuino.print("emrelabs");
  panelDuino.setCursor(0, 1);
  panelDuino.print(".com    ");

  delay(2000);

  panelDuino.setCursor(0, 0);
  panelDuino.print("Button: ");
  panelDuino.setCursor(0, 1);
  panelDuino.print("        ");

}

void loop()
{

  // read buttons
  uint8_t buttons = panelDuino.readButtons();

  panelDuino.setCursor(0, 1);

  if(buttons & BUTTON_LEFT)
  {    
    panelDuino.print("LEFT    ");
  }
  if(buttons & BUTTON_RIGHT)
  {
    panelDuino.print("RIGHT   ");
  }
  if(buttons & BUTTON_UP)
  {
    panelDuino.print("UP      ");
  }
  if(buttons & BUTTON_DOWN)
  {
    panelDuino.print("DOWN    ");
  }
  if(buttons & BUTTON_OK)
  {
    panelDuino.print("OK      ");
  }
  
  if(buttons == 0)
  {
       panelDuino.print("        ");
  }

}
