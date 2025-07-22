#include <Arduino.h>

#define SERVO_BAUD 1000000
#define SERVO_PIN  21          // Single pin for both TX and RX

HardwareSerial& BUS = Serial1;

void setup() {
  // Initialize the USB serial connection to the PC
  // The baud rate here is mostly for the serial monitor, 
  // as native USB adjusts automatically.
  Serial.begin(SERVO_BAUD);    

  // Initialize the hardware serial (UART1) in half-duplex mode on the specified pin
  BUS.begin(SERVO_BAUD, SERIAL_8N1, SERVO_PIN, SERVO_PIN, /*invert=*/false, /*halfDuplex=*/true); 
}

void loop() {
  // Forward bytes from USB (PC) to the Servo Bus
  while (Serial.available()) {
    BUS.write(Serial.read());
  }
  
  // Forward bytes from the Servo Bus back to USB (PC)
  while (BUS.available()) {
    Serial.write(BUS.read());
  }
}