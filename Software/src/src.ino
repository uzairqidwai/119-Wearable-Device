#include <driver/i2s.h>
#include <math.h>
#include <TFT_eSPI.h>
#include <ArduinoJson.h>
#include "questions.h"
#include "QR.h"  // Assuming QR.h contains a bitmap of the QR code
#include "pin_config.h"

#define SAMPLE_RATE 44100
#define PIN_I2S_BCLK 3
#define PIN_I2S_LRC 10
#define PIN_I2S_DIN 11
#define PI 3.14159265
#define LED_PIN 13

unsigned long previousMillis = 0; 
const long interval = 500;  // interval at which to blink (milliseconds)
bool alarmIsActive = true;  // Global flag
bool startQuestionFlow = false;  // Global flag

bool leftButton = false;  // Left Button Press Flag
bool rightButton = false;  // Right Button Press Flag

const unsigned long debounceDelay = 500;  // Debounce time in milliseconds
volatile unsigned long lastInterruptTime14 = 0;  // Last time the interrupt was triggered for pin 14
volatile unsigned long lastInterruptTime0 = 0;   // Last time the interrupt was triggered for pin 0

TFT_eSPI tft = TFT_eSPI();

DynamicJsonDocument doc(2048);
JsonObject current_question;


void setup() {
  Serial.begin(115200);  // Initialize serial communication at 115200 baud rate
  setupSirenAndLED();
  setupQR();
  pinMode(14, INPUT_PULLUP); //Right Button
  pinMode(0, INPUT_PULLUP); //Left Button
  attachInterrupt(digitalPinToInterrupt(14), handleInterruptRight, FALLING);
  attachInterrupt(digitalPinToInterrupt(0), handleInterruptLeft, FALLING);

}

void loop() {
  alarmState();
  displayQuestions();
}


void handleInterruptRight() {
  unsigned long currentMillis = millis();
  if (currentMillis - lastInterruptTime14 > debounceDelay) {
    Serial.println("Right went LOW!");
    lastInterruptTime14 = currentMillis;
  }
    //If button pressed, turn alarm off & start question flow
  if (alarmIsActive) {
    alarmIsActive = false; // Stop the alarm and LED
    startQuestionFlow = true;
    stopAlarm(); 
    Serial.println("Right went LOW #1");
  }
  else{
    Serial.println("Right went LOW #2");
    rightButton = true;
    displayQuestions();
  }
}

void handleInterruptLeft() {
  unsigned long currentMillis = millis();
  if (currentMillis - lastInterruptTime0 > debounceDelay) {
    lastInterruptTime0 = currentMillis;
  }
    //If button pressed, turn alarm off & start question flow
  if (alarmIsActive) {
    Serial.println("Left went LOW #1");
    alarmIsActive = false; // Stop the alarm and LED
    startQuestionFlow = true; 
    stopAlarm();
  }
  else{
    Serial.println("Left went LOW #2");
    leftButton = true;
    displayQuestions();
  }
}


void alarmState (){

  //If alarm active, play sound and strobe
  if (alarmIsActive) {
    playTone(440.0, 0.5);  // Play 440Hz tone for .5 second
    playTone(880.0, 0.5);  // Play 880Hz tone for .5 second
    blinkLED();
  }
  //If alarm deactivated, turn it off.
  else{
    stopAlarm();
  }
}

void stopAlarm() {
  Serial.println("stopping");
    i2s_zero_dma_buffer(I2S_NUM_0);
    i2s_stop(I2S_NUM_0);  // Stop the I2S driver
    digitalWrite(LED_PIN, LOW);  // Turn the LED off
}

void setupQR() {
    tft.init();
    tft.setRotation(4);

    DeserializationError error = deserializeJson(doc, QUESTIONS_CONFIG);
    if (error) {
        tft.println("Failed to read JSON config");
        return;
    }

    current_question = doc.as<JsonObject>();
    drawMainScreen();

    //Battery Setup
    pinMode(PIN_POWER_ON, OUTPUT);
    digitalWrite(PIN_POWER_ON, HIGH);
}

void displayQuestions() {
  if (startQuestionFlow) {
    delay(200);
    drawScreen();
    while (true) {
      if (rightButton) {
        delay(200);
        rightButton = false;
        if (current_question.containsKey("yes")) {
          current_question = current_question["yes"].as<JsonObject>();
          drawScreen();
        }
      }

      if (leftButton) {
        delay(200);
        leftButton = false;
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

void setupSirenAndLED() {
  // Configure I2S
    i2s_config_t i2s_config = {
        .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX),
        .sample_rate = SAMPLE_RATE,
        .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
        .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
        .communication_format = (i2s_comm_format_t)(I2S_COMM_FORMAT_I2S | I2S_COMM_FORMAT_I2S_MSB),
        .dma_buf_count = 2,
        .dma_buf_len = 128,
        .use_apll = false
    };

    i2s_pin_config_t pin_config = {
        .bck_io_num = PIN_I2S_BCLK,
        .ws_io_num = PIN_I2S_LRC,
        .data_out_num = PIN_I2S_DIN,
        .data_in_num = -1   // Not used
    };

    i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
    i2s_set_pin(I2S_NUM_0, &pin_config);
    i2s_set_clk(I2S_NUM_0, SAMPLE_RATE, I2S_BITS_PER_SAMPLE_16BIT, I2S_CHANNEL_STEREO);
  
  // Setup for LED
  pinMode(LED_PIN, OUTPUT);

}


void playTone(float frequency, float duration) {
    int sampleCount = duration * SAMPLE_RATE;
    int16_t sample;
    size_t bytes_written;

    for (int i = 0; i < sampleCount; i++) {
        double t = (double)i / SAMPLE_RATE;  // Current time in seconds
        sample = 32767 * sin(2.0 * PI * frequency * t);  // Generate the sine wave sample

        // Use i2s_write_expand with the correct arguments
        i2s_write_expand(I2S_NUM_0, &sample, sizeof(int16_t), 16, I2S_BITS_PER_SAMPLE_16BIT, &bytes_written, portMAX_DELAY);
      }
}


void blinkLED() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    digitalWrite(LED_PIN, !digitalRead(LED_PIN));
  }
}
