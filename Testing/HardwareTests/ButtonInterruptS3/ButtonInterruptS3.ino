const unsigned long debounceDelay = 500;  // Debounce time in milliseconds
volatile unsigned long lastInterruptTime14 = 0;  // Last time the interrupt was triggered for pin 14
volatile unsigned long lastInterruptTime0 = 0;   // Last time the interrupt was triggered for pin 0

void setup() {
  pinMode(14, INPUT_PULLUP);
  pinMode(0, INPUT_PULLUP);
  
  attachInterrupt(digitalPinToInterrupt(14), handleInterruptPin14, FALLING);
  attachInterrupt(digitalPinToInterrupt(0), handleInterruptPin0, FALLING);

  Serial.begin(9600);
}

void loop() {
  // Your main code here
}

void handleInterruptPin14() {
  unsigned long currentMillis = millis();
  if (currentMillis - lastInterruptTime14 > debounceDelay) {
    Serial.println("Pin 14 went LOW!");
    lastInterruptTime14 = currentMillis;
  }
}

void handleInterruptPin0() {
  unsigned long currentMillis = millis();
  if (currentMillis - lastInterruptTime0 > debounceDelay) {
    Serial.println("Pin 0 went LOW!");
    lastInterruptTime0 = currentMillis;
  }
}
