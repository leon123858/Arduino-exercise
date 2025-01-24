#include <SPI.h>
#include <Wire.h>
#include "controller.h"

#define IS_DEBUG false

/*
 * This line initializes a pointer to Controller class object, which will control system functions.
 */
Controller *controller = nullptr;

/**
 * Initialize function called once when the program starts
 */
void setup()
{
#if (IS_DEBUG)
	// Initializes Serial communication at baud rate of 9600 for debugging purposes
	Serial.begin(9600);
#endif
	controller = new Controller();
}

/**
 * Main loop that runs continuously while the program is running
 */
void loop()
{
	controller->Serve();
}
