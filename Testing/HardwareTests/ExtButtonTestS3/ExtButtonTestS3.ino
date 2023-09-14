#include <TFT_eSPI.h> // Hardware-specific library for ESP32
#include <Wire.h>
#include <Button2.h>

TFT_eSPI tft = TFT_eSPI();  // Invoke library
Button2 btn = Button2(12);   // Define pin where the button is connected

int counter = 0;

void setup() {
  pinMode(12, INPUT_PULLUP);  // Enable internal pull-up resistor
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(2);
  btn.setPressedHandler([](Button2 & b) {
    counter++;
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(40, 80);
    tft.print("Counter: ");
    tft.println(counter);
  });
}

void loop() {
  btn.loop();
}
