#include <SPI.h>
#include <Wire.h>
#include "oled_api.h"
#include "ps2btn_api.h"

#define IS_DEBUG false

OLED oled;
PS2Button btn;

void setup()
{
#if (IS_DEBUG)
  Serial.begin(9600);
#endif
  oled.init();
  btn.init();
  oled.printText("Welcome To Scouting");
}

void loop()
{
  delay(1000);
}
