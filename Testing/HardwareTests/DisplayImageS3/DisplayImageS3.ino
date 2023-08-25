//This image displays a qr code pointing to osu.edu

#include <TFT_eSPI.h>
#include "QR.h"  // Assuming you've stored your image data in this header file

TFT_eSPI tft = TFT_eSPI();

void setup() {
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_WHITE);

  // Draw the image at position (0, 0)
  tft.pushImage(0, 0, 111, 111, QR);
}

void loop() {
  // Nothing here for this example
}
