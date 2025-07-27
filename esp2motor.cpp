#include <SCServo.h>

SMS_STS st;

#define S_RXD 20    // RX pin on ESP32-C3 Super Mini
#define S_TXD 21   // TX pin on ESP32-C3 Super Mini

void setup() {
  Serial.begin(115200); // Serial monitor for debugging
  Serial.println("start");
  Serial1.begin(1000000, SERIAL_8N1, S_RXD, S_TXD); // UART for servo driver
  st.pSerial = &Serial1;
  delay(1000); // Wait for Serial1 to initialize

  Serial.println("ESP32-C3 Servo Control Started");

  // Ping servo to check if ID 1 is connected
  if (st.Ping(1) == 1) {
    Serial.println("Servo ID 1 detected");
  } else {
    Serial.println("Servo ID 1 not found. Scanning for servos...");
    scanServos(); // Scan for connected servo IDs
  }
}

void loop() {
  Serial.println("Moving to position 4095...");
  st.WritePosEx(2, 4095, 3400, 50); // Move to max position (4095), speed 3400, accel 50
  delay(2000);

  Serial.println("Moving to position 2000...");
  st.WritePosEx(2, 2000, 1500, 50); // Move to mid position (2000), speed 1500, accel 50
  delay(2000);

  // Optional: Read and display servo feedback
  int16_t pos = st.ReadPos(1); // Read current position
  if (pos != -1) {
    Serial.print("Current Position: ");
    Serial.println(pos);
  } else {
    Serial.println("Failed to read position");
  }


}

// Function to scan for connected servo IDs (0 to 20 by default)
void scanServos() {
  for (int id = 0; id <= 20; id++) {
    if (st.Ping(id) == id) {
      Serial.print("Found servo with ID: ");
      Serial.println(id);
    }
  }
}