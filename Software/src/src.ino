#include <TFT_eSPI.h>      // Include the graphics library
#define LED_PIN 13         // Define the pin where the LED is connected
#include "alarm.h"

TFT_eSPI tft = TFT_eSPI(); // Create an instance of the TFT_eSPI class


void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:
  soundAlarmAndStrobeLED();

}
