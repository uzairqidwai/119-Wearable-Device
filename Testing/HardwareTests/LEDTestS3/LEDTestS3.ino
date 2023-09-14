#include <TFT_eSPI.h>      // Include the graphics library

#define LED_PIN 13         // Define the pin where the LED is connected

TFT_eSPI tft = TFT_eSPI(); // Create an instance of the TFT_eSPI class

void setup() {
  tft.init();              // Initialize the display
  tft.setRotation(1);      // Set the rotation of the display
  tft.fillScreen(TFT_BLACK); // Set the background color to black
  tft.setTextColor(TFT_WHITE); // Set the text color to white
  tft.setTextSize(2);       // Set the text size to 2
  tft.drawString("LED Flasher", 5, 5, 2); // Draw the text "LED Flasher" at position (5, 5)
  
  pinMode(LED_PIN, OUTPUT); // Set the LED pin as an OUTPUT
}

void loop() {
  digitalWrite(LED_PIN, HIGH); // Turn the LED on
  delay(500);                  // Wait for 500 milliseconds
  digitalWrite(LED_PIN, LOW);  // Turn the LED off
  delay(500);                  // Wait for 500 milliseconds
}
