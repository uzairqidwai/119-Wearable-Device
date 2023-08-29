#include <SoftwareSerial.h>

SoftwareSerial mySerial(1, 2); // RX, TX

void setup() {
  Serial.begin(115200);
  mySerial.begin(115200);
}

void loop() {
  if (Serial.available()) {
    mySerial.write(Serial.read());
  }
  if (mySerial.available()) {
    Serial.write(mySerial.read());
  }
}
