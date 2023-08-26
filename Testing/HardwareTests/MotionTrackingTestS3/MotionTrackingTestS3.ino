#include "Wire.h"
#include "I2Cdev.h"
#include "MPU6050.h"
#include "TFT_eSPI.h"

MPU6050 accelgyro;
TFT_eSPI tft = TFT_eSPI();

int16_t ax, ay, az;
int16_t gx, gy, gz;

#define ACCEL_THRESHOLD 8000
#define GYRO_THRESHOLD 150
#define ANGLE_THRESHOLD 30

void setup() {
 Wire.begin(21, 16);  // SDA is GPIO 21, SCL is GPIO 16
 Serial.begin(38400);
 tft.begin();
 tft.setRotation(1);

 Serial.println("Initializing I2C devices...");
 accelgyro.initialize();

 Serial.println("Testing device connections...");
 Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");
}

void loop() {
 accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

 // Calculate the magnitude of acceleration
 int totalAccel = sqrt(ax*ax + ay*ay + az*az);

 // Calculate the rate of change of acceleration
 static int prevAccel = 0;
 int accelChange = abs(totalAccel - prevAccel);
 prevAccel = totalAccel;

 // Calculate the angle of orientation
 int angle = atan2(ay, az) * 180 / PI;

 // Output data to serial
 Serial.print("Total Accel: "); Serial.print(totalAccel);
 Serial.print("\tAccel Change: "); Serial.print(accelChange);
 Serial.print("\tAngle: "); Serial.println(angle);

 tft.fillScreen(TFT_BLACK);

 // Check if a fall is detected
 if (totalAccel > ACCEL_THRESHOLD && accelChange > ACCEL_THRESHOLD && abs(angle) > ANGLE_THRESHOLD) {
   tft.setTextColor(TFT_RED);
   tft.drawString("Fall Detected", tft.width()/2, tft.height()/2, 2);
   Serial.println("Fall Detected");
 } else {
   tft.setTextColor(TFT_GREEN);
   tft.drawString("Normal", tft.width()/2, tft.height()/2, 2);
 }

 delay(200);
}
