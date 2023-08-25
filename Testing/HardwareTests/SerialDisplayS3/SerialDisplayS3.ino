#include <TFT_eSPI.h>  // Graphics library for ST7735 driver chip
#include "pin_config.h"

TFT_eSPI tft = TFT_eSPI();  // Create an instance of the TFT library

void setup(void) {
    Serial.begin(115200);  // Initialize serial communication for debugging
    Serial1.begin(9600, SERIAL_8N1, 18, 17); // Initialize UART1 with RX on GPIO 18 and TX on GPIO 17

    // Initialize the TFT screen
    tft.init();
    tft.setRotation(1);  // Set screen rotation. Adjust as needed.
    tft.fillScreen(TFT_BLACK);  // Fill the screen with a black background

    // Set text properties
    tft.setTextColor(TFT_WHITE);  // Set text color to white
    tft.setTextSize(2);  // Set text size
    tft.setCursor(10, 10);  // Set the cursor position
}

void loop() {
    if(Serial1.available()) {  // If data is available on Serial1
        String data = Serial1.readString();  // Read the data from Serial1
        
        // Output the data on the TFT display
        tft.println(data);

        // Send the data out on the standard Serial
        Serial.println(data);
    }

    delay(10);  // Short delay for stability
}
