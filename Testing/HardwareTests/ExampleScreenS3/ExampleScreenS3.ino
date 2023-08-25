#include <TFT_eSPI.h>
#include "QR.h"  // Assuming QR.h contains a bitmap of the QR code

TFT_eSPI tft = TFT_eSPI();  // Create TFT object

void setup() {
    tft.init();
    tft.setRotation(4);  // Vertical orientation
    drawScreen();
}

void loop() {
    // Your loop code here (e.g., button handling)
}

void drawScreen() {
    // Clear screen
    tft.fillScreen(TFT_BLACK);

    // Draw red border
    tft.drawRect(0, 0, tft.width(), tft.height(), TFT_RED);

    // Reset to default Arduino font
    tft.setTextSize(2);  // You can adjust the size as needed

    // Display the text centered
    printWrappedText("Press a button to start the device, or scan the QR Code Below", 20);

    // Display the QR code
    int qrX = (tft.width() - 111) / 2;  // Centering the QR code horizontally
    int y = tft.height() / 2;           // Start drawing from the vertical center
    tft.pushImage(qrX, y, 111, 111, QR);
}

void printWrappedText(String text, int startY) {
    String line = "";
    int wordStartIndex = 0;
    int y = startY;

    for (int i = 0; i < text.length(); i++) {
        if (text[i] == ' ' || i == text.length() - 1) {  // End of a word
            String word = text.substring(wordStartIndex, i + 1);
            String potentialLine = line + word;  // Line that might be printed
            
            // If this word doesn't fit on the current line, print the line and move to the next line
            if (tft.textWidth(potentialLine) > tft.width()) {
                int x = (tft.width() - tft.textWidth(line)) / 2;
                tft.setCursor(x, y);
                tft.print(line);
                y += tft.fontHeight();
                line = word;  // Start the next line with the current word
            } else {
                line = potentialLine;  // Add the word to the line
            }

            wordStartIndex = i + 1;  // Start of the next word
        }
    }
    
    // Print the last line
    int x = (tft.width() - tft.textWidth(line)) / 2;
    tft.setCursor(x, y);
    tft.print(line);
}
