#ifndef CONFIG_H
#define CONFIG_H

#include <driver/i2s.h>
#include <math.h>
#include <TFT_eSPI.h>

#define SAMPLE_RATE 44100
#define PIN_I2S_BCLK 3
#define PIN_I2S_LRC 10
#define PIN_I2S_DIN 11
#define PI 3.14159265
#define LED_PIN 13

extern TFT_eSPI tft;  // Declare tft instance for external usage (defined in the .c file)

// Function declarations
void soundAlarm();
void strobeLED();
void soundAlarmAndStrobeLED();

#endif // ALARM_LED_FUNCTIONS_H
