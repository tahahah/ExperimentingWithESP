#include <NimBLEDevice.h>

// Define the LED pin (GPIO8 for ESP32-C3 SuperMini)
#define LED_BUILTIN 8

void setup() {
  // Initialize Serial for debugging
  Serial.begin(115200);
  while (!Serial) {
    delay(10);
  }
  Serial.println("Starting Simple BLE Sketch V2");

  // Initialize the LED pin
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH); // LED OFF initially
  Serial.println("LED Initialized");

  // Initialize NimBLE. The name given here is the default.
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

  // Get the advertising object
  NimBLEAdvertising* pAdvertising = NimBLEDevice::getAdvertising();
  
  // Explicitly set the name in the advertising packet
  pAdvertising->setName("TahasESP");
  
  // Add the service UUID to the advertisement
  pAdvertising->addServiceUUID(pService->getUUID());

  // Start advertising
  pAdvertising->start();
  Serial.println("BLE Advertising started");
  
  digitalWrite(LED_BUILTIN, LOW); // Turn LED ON to indicate setup is complete
  Serial.println("Setup complete. LED ON.");
}

void loop() {
  // Nothing to do here for this simple sketch
  delay(2000);
}
