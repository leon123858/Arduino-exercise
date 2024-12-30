#include <SPI.h>
#include <Wire.h>
#include "controller.h"

#define IS_DEBUG false

Controller *controller = nullptr;

void setup()
{
#if (IS_DEBUG)
	Serial.begin(9600);
#endif
	controller = new Controller();
}

void loop()
{
	controller->Serve();
}
