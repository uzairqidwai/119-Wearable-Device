#include <SoftwareSerial.h>
#include "phonenumber.h"

SoftwareSerial mySerial(1, 2); // RX, TX
const int buttonPin1 = 0; // the number of the pushbutton pin
const int buttonPin2 = 14; // the number of the pushbutton pin
int buttonState1 = 0; // variable for reading the pushbutton status
int buttonState2 = 0; // variable for reading the pushbutton status
String location = "";

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);
  pinMode(buttonPin1, INPUT_PULLUP);
  pinMode(buttonPin2, INPUT_PULLUP);
}

void loop() {
  buttonState1 = digitalRead(buttonPin1);
  buttonState2 = digitalRead(buttonPin2);
  if (buttonState1 == LOW || buttonState2 == LOW) {
    getLocation();
    sendMessage();
  }
  if (mySerial.available()) {
    Serial.write(mySerial.read());
  }
}

void getLocation() {
  sendATCommand("AT+GPS=1");
  delay(1000);
  for(int i=0; i<10; i++){
    mySerial.flush();
    String response = sendATCommand("AT+LOCATION=2");
    if (response.indexOf("ERROR") < 0) {
      location = response.substring(response.indexOf("\r\n") + 2); // skip the first line of the response
      break;
    }
    delay(1000);
  }
}

void sendMessage() {
  while (!sendATCommand("AT")) {
    delay(500);
  }
  sendATCommand("AT+CMGF=1");
  delay(100);
  sendATCommand("AT+CMGS=\"" PHONE_NUMBER "\"");
  delay(1000);
  mySerial.print("This is a test");
  if(location.length() > 0){
    mySerial.print(" Location: ");
    mySerial.print(location);
  }
  mySerial.write(26); // Sending Ctrl+Z
}

String sendATCommand(char* command) {
  mySerial.print(command);
  mySerial.print("\r\n");
  delay(100);
  String response = "";
  while (mySerial.available()) {
    char c = mySerial.read();
    response += c;
    Serial.write(c);
  }
  return response;
}
