#include <Wire.h>
#include <I2Cdev.h>
#include <MPU6050.h>
#include <TFT_eSPI.h>

MPU6050 accelgyro;
TFT_eSPI tft = TFT_eSPI();

int16_t ax, ay, az;
int16_t gx, gy, gz;

#define ACCEL_THRESHOLD 8000
#define TIME_WINDOW 10  // in loop cycles, change as required

int accelHistory[TIME_WINDOW][3];
int currentindex = 0;

void setup() {
  Wire.begin(21, 16);  // SDA is GPIO 21, SCL is GPIO 16
  Serial.begin(38400);
  tft.begin();
  tft.setRotation(1);

  Serial.println("Initializing I2C devices...");
  accelgyro.initialize();

  Serial.println("Testing device connections...");
  Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");

  for (int i = 0; i < TIME_WINDOW; i++) {
    accelHistory[i][0] = 0;
    accelHistory[i][1] = 0;
    accelHistory[i][2] = 0;
  }
}

void loop() {
  accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  // Store the current acceleration values in the history
  accelHistory[currentindex][0] = ax;
  accelHistory[currentindex][1] = ay;
  accelHistory[currentindex][2] = az;
  currentindex = (currentindex + 1) % TIME_WINDOW;

  // Calculate the features
  int mean[3] = {0, 0, 0};
  for (int i = 0; i < TIME_WINDOW; i++) {
    mean[0] += accelHistory[i][0];
    mean[1] += accelHistory[i][1];
    mean[2] += accelHistory[i][2];
  }
  mean[0] /= TIME_WINDOW;
  mean[1] /= TIME_WINDOW;
  mean[2] /= TIME_WINDOW;

  int variance[3] = {0, 0, 0};
  for (int i = 0; i < TIME_WINDOW; i++) {
    variance[0] += (accelHistory[i][0] - mean[0]) * (accelHistory[i][0] - mean[0]);
    variance[1] += (accelHistory[i][1] - mean[1]) * (accelHistory[i][1] - mean[1]);
    variance[2] += (accelHistory[i][2] - mean[2]) * (accelHistory[i][2] - mean[2]);
  }

  int stdDev[3];
  stdDev[0] = sqrt(variance[0] / TIME_WINDOW);
  stdDev[1] = sqrt(variance[1] / TIME_WINDOW);
  stdDev[2] = sqrt(variance[2] / TIME_WINDOW);

  // Output data to serial
  Serial.print("Standard Deviation - X: "); Serial.print(stdDev[0]);
  Serial.print("\tY: "); Serial.print(stdDev[1]);
  Serial.print("\tZ: "); Serial.println(stdDev[2]);

  tft.fillScreen(TFT_BLACK);

  // Check if a fall is detected
  if (stdDev[0] > ACCEL_THRESHOLD || stdDev[1] > ACCEL_THRESHOLD || stdDev[2] > ACCEL_THRESHOLD) {
    tft.setTextColor(TFT_RED);
    tft.drawString("Fall Detected", tft.width()/2, tft.height()/2, 2);
    Serial.println("Fall Detected");
  } else {
    tft.setTextColor(TFT_GREEN);
    tft.drawString("Normal", tft.width()/2, tft.height()/2, 2);
  }

  delay(200);
}
