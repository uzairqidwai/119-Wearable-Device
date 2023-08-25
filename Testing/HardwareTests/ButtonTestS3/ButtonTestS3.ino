#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI();  // Create TFT object

const int LEFT_BUTTON_PIN = 0;  // Placeholder, adjust if necessary
const int RIGHT_BUTTON_PIN = 14;  // Placeholder, adjust if necessary

void setup() {
    tft.init();
    tft.setRotation(4);  // Vertical orientation
    
    pinMode(LEFT_BUTTON_PIN, INPUT_PULLUP);
    pinMode(RIGHT_BUTTON_PIN, INPUT_PULLUP);
}

void loop() {
    if (digitalRead(LEFT_BUTTON_PIN) == LOW) {  // Button pressed
        tft.fillScreen(TFT_BLUE);
        delay(300);  // Debounce delay
    }

    if (digitalRead(RIGHT_BUTTON_PIN) == LOW) {  // Button pressed
        tft.fillScreen(TFT_RED);
        delay(300);  // Debounce delay
    }
}

// (The rest of your code, including the drawScreen function, remains unchanged)
