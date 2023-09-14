#include <TFT_eSPI.h>
#include "pin_config.h"

TFT_eSPI tft = TFT_eSPI();

const int batteryPin = 4;
const float V_REF = 3.3;
const float RESOLUTION = 4095.0;
const float MAX_BATTERY_VOLTAGE = 4.2;
const float MIN_BATTERY_VOLTAGE = 3.0;

void setup() {
  tft.begin();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
  pinMode(batteryPin, INPUT);

  //Battery Setup
  pinMode(PIN_POWER_ON, OUTPUT);
  digitalWrite(PIN_POWER_ON, HIGH);
}

void loop() {
  float batteryVoltage = readBattery();
  float batteryPercentage = mapVoltageToPercentage(batteryVoltage);
  
  displayBattery(batteryPercentage);
  delay(5000);
}

float readBattery() {
  int adcReading = analogRead(batteryPin);
  float voltage = ((adcReading * 2 * V_REF * 1000) / RESOLUTION) / 1000;
  return voltage;
}

float mapVoltageToPercentage(float voltage) {
  if (voltage >= MAX_BATTERY_VOLTAGE) return 100.0;
  if (voltage <= MIN_BATTERY_VOLTAGE) return 0.0;
  return (voltage - MIN_BATTERY_VOLTAGE) * 100.0 / (MAX_BATTERY_VOLTAGE - MIN_BATTERY_VOLTAGE);
}

void displayBattery(float percentage) {
  tft.fillRect(10, 10, 40, 20, TFT_BLACK);  // Clear previous rectangle
  
  tft.drawRect(10, 10, 40, 20, TFT_WHITE);  // Battery outline
  tft.fillRect(52, 15, 3, 10, TFT_WHITE);   // Battery nib
  
  int fillWidth = (int) (percentage * 0.38);
  tft.fillRect(11, 11, fillWidth, 18, TFT_GREEN);
  
  tft.fillRect(60, 15, 50, 20, TFT_BLACK);  // Clear previous percentage
  tft.setCursor(60, 15);
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(2);
  tft.print(percentage, 0);
  tft.print("%");
}
