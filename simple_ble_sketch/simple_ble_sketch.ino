#include <NimBLEDevice.h>

// Define the LED pin (GPIO8 for ESP32-C3 SuperMini)
#define LED_BUILTIN 8

void setup() {
  // Initialize Serial for debugging
  Serial.begin(115200);
  while (!Serial) {
    delay(10);
  }
  Serial.println("Starting Simple BLE Sketch");

  // Initialize the LED pin
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH); // LED OFF initially
  Serial.println("LED Initialized");

  // Initialize NimBLE
  NimBLEDevice::init("SimpleBLE");
  Serial.println("NimBLE initialized");

  // Create BLE Server
  NimBLEServer* pServer = NimBLEDevice::createServer();
  Serial.println("BLE Server created");

  // Create BLE Service
  NimBLEService* pService = pServer->createService("AABB"); // Simple UUID
  Serial.println("BLE Service created");

  // Start the service
  pService->start();
  Serial.println("BLE Service started");

  // Start advertising
  NimBLEAdvertising* pAdvertising = NimBLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(pService->getUUID());
  pAdvertising->start();
  Serial.println("BLE Advertising started");
  
  digitalWrite(LED_BUILTIN, LOW); // Turn LED ON to indicate setup is complete
  Serial.println("Setup complete. LED ON.");
}

void loop() {
  // Nothing to do here for this simple sketch
  delay(2000);
}
