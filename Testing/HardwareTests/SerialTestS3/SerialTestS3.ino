
//This script just sends a test message over the UART1 interface to ensure that the connection is working. 
//Be sure to set baud to 9600
void setup() {
  Serial1.begin(9600, SERIAL_8N1, 18, 17); // UART1 with RX on GPIO 18 and TX on GPIO 17
  Serial1.println("UART1 Initialized.");
}

void loop() {
  Serial1.println("Testing UART1.");
  delay(1000);  // 1-second delay
}
