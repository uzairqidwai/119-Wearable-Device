#include "config.h"  // Include the external config.h file
#include <TFT_eSPI.h>
#include <ArduinoJson.h>  // Library to parse JSON

TFT_eSPI tft = TFT_eSPI();  // Create TFT object

DynamicJsonDocument doc(2048);  // Adjust the size based on your config size
JsonObject current_question;

void setup() {
    tft.init();
    tft.setRotation(4);  // Vertical orientation

    // Parse the JSON config from QUESTIONS_CONFIG constant (from config.h)
    DeserializationError error = deserializeJson(doc, QUESTIONS_CONFIG);
    if (error) {
        tft.println("Failed to read JSON config");
        return;
    }

    current_question = doc.as<JsonObject>();
    drawScreen();
    
    pinMode(0, INPUT_PULLUP);  // Setting up button on GPIO 0
    pinMode(14, INPUT_PULLUP); // Setting up button on GPIO 14
}

void loop() {
    if (digitalRead(14) == LOW) {  // 'Yes' button pressed
        delay(200);  // Debouncing
        if (current_question.containsKey("yes")) {
            current_question = current_question["yes"].as<JsonObject>();
            drawScreen();
        }
    }
    
    if (digitalRead(0) == LOW) {  // 'No' button pressed
        delay(200);  // Debouncing
        if (current_question.containsKey("no")) {
            current_question = current_question["no"].as<JsonObject>();
            drawScreen();
        }
    }
}

void drawScreen() {
    // Clear screen
    tft.fillScreen(TFT_BLACK);

    // Draw red border
    tft.drawRect(0, 0, tft.width(), tft.height(), TFT_RED);

    // Check for actions and execute them
    if (current_question.containsKey("yes_action")) {
        executeAction(current_question["yes_action"].as<String>());
    }
    if (current_question.containsKey("no_action")) {
        executeAction(current_question["no_action"].as<String>());
    }

    // Reset to default Arduino font
    tft.setTextSize(2);  // Adjust the size as needed

    // Display the current question centered
    String question_text = current_question["question"].as<String>();
    printWrappedText(question_text, 20);
}

void executeAction(String action) {
    if (action == "yellowScreen") {
        yellowScreen();
    } else if (action == "blueScreen") {
        blueScreen();
    }
}

void yellowScreen() {
    tft.fillScreen(TFT_YELLOW);
}

void blueScreen() {
    tft.fillScreen(TFT_BLUE);
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
