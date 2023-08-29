#include <TFT_eSPI.h>
#include <ArduinoJson.h>
#include "config.h"
#include "QR.h"  // Assuming QR.h contains a bitmap of the QR code

TFT_eSPI tft = TFT_eSPI();

DynamicJsonDocument doc(2048);
JsonObject current_question;

void drawMainScreen();
void drawScreen();
void executeAction(String action);
void yellowScreen();
void blueScreen();
void redScreen();
void greenScreen();
void printWrappedText(String text, int startY);

void setup() {
    tft.init();
    tft.setRotation(4);

    DeserializationError error = deserializeJson(doc, QUESTIONS_CONFIG);
    if (error) {
        tft.println("Failed to read JSON config");
        return;
    }

    current_question = doc.as<JsonObject>();
    drawMainScreen();

    pinMode(0, INPUT_PULLUP);
    pinMode(14, INPUT_PULLUP);
}

void loop() {
    if (digitalRead(14) == LOW || digitalRead(0) == LOW) {
        delay(200);
        drawScreen();
        while (true) {
            if (digitalRead(14) == LOW) {
                delay(200);
                if (current_question.containsKey("yes")) {
                    current_question = current_question["yes"].as<JsonObject>();
                    drawScreen();
                }
            }
            
            if (digitalRead(0) == LOW) {
                delay(200);
                if (current_question.containsKey("no")) {
                    current_question = current_question["no"].as<JsonObject>();
                    drawScreen();
                }
            }
        }
    }
}

void drawMainScreen() {
    tft.fillScreen(TFT_BLACK);
    tft.drawRect(0, 0, tft.width(), tft.height(), TFT_RED);
    tft.setTextSize(2);
    printWrappedText("Press a button to start the device, or scan the QR Code Below", 20);
    int qrX = (tft.width() - 111) / 2;
    int y = tft.height() / 2;
    tft.pushImage(qrX, y, 111, 111, QR);
}

void drawScreen() {
    tft.fillScreen(TFT_BLACK);
    tft.drawRect(0, 0, tft.width(), tft.height(), TFT_RED);

    if (current_question.containsKey("yes_action")) {
        executeAction(current_question["yes_action"].as<String>());
    }
    if (current_question.containsKey("no_action")) {
        executeAction(current_question["no_action"].as<String>());
    }

    tft.setTextSize(2);
    String question_text = current_question["question"].as<String>();
    printWrappedText(question_text, 20);
}

void executeAction(String action) {
    if (action == "yellowScreen") {
        yellowScreen();
    } else if (action == "blueScreen") {
        blueScreen();
    } else if (action == "redScreen") {
        redScreen();
    } else if (action == "greenScreen") {
        greenScreen();
    }
}

void yellowScreen() {
    tft.fillScreen(TFT_YELLOW);
}

void blueScreen() {
    tft.fillScreen(TFT_BLUE);
}

void redScreen() {
    tft.fillScreen(TFT_RED);
}

void greenScreen() {
    tft.fillScreen(TFT_GREEN);
}

void printWrappedText(String text, int startY) {
    String line = "";
    int wordStartIndex = 0;
    int y = startY;

    for (int i = 0; i < text.length(); i++) {
        if (text[i] == ' ' || i == text.length() - 1) {
            String word = text.substring(wordStartIndex, i + 1);
            String potentialLine = line + word;
            
            if (tft.textWidth(potentialLine) > tft.width()) {
                int x = (tft.width() - tft.textWidth(line)) / 2;
                tft.setCursor(x, y);
                tft.print(line);
                y += tft.fontHeight();
                line = word;
            } else {
                line = potentialLine;
            }

            wordStartIndex = i + 1;
        }
    }
    
    int x = (tft.width() - tft.textWidth(line)) / 2;
    tft.setCursor(x, y);
    tft.print(line);
}
