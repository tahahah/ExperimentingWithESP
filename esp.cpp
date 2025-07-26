
#include <NimBLEDevice.h>

#define SERVICE_UUID        "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_RX "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_TX "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"


BLECharacteristic* pTxCharacteristic = NULL;
NimBLEServer* pServer = NULL;
bool deviceConnected = false;
bool oldDeviceConnected = false;

class MyServerCallbacks: public NimBLEServerCallbacks {
    void onConnect(NimBLEServer* pServer,NimBLEConnInfo& connInfo) override {
        deviceConnected = true;
    };
    void onDisconnect(NimBLEServer *pServer, NimBLEConnInfo &connInfo, int reason) override {
        Serial.println("onDisconnect triggered at: " + String(millis()) + "ms");
        deviceConnected = false;
    }
};

class MyCallbacks : public NimBLECharacteristicCallbacks {
    void onWrite(NimBLECharacteristic* 	pCharacteristic,NimBLEConnInfo& connInfo )  override {
        Serial.println("onWrite triggered at: " + String(millis()) + "ms");
        std::string rxValue = pCharacteristic->getValue();
        if (rxValue.length() > 0) {
            Serial.print("Received: ");
            Serial.println(rxValue.c_str());
        } else {
            Serial.println("Received empty data");
        }
    }
};

void setup() {
    Serial.begin(115200);
    Serial.println("Starting BLE UART Server...");

    // Initialize NimBLE
    NimBLEDevice::init("ESP32C3_UART");
    NimBLEDevice::setPower(ESP_PWR_LVL_P9); // Set BLE power level (+9dBm)

    // Create BLE Server
    pServer = NimBLEDevice::createServer();
    pServer->setCallbacks(new MyServerCallbacks());

    // Create BLE Service
    BLEService* pService = pServer->createService(SERVICE_UUID);

    // Create TX Characteristic (Notify)
    pTxCharacteristic = pService->createCharacteristic(
        CHARACTERISTIC_UUID_TX,
        NIMBLE_PROPERTY::NOTIFY
    );

    // Create RX Characteristic (Write)
    BLECharacteristic* pRxCharacteristic = pService->createCharacteristic(
        CHARACTERISTIC_UUID_RX,
        NIMBLE_PROPERTY::WRITE
    );
    pRxCharacteristic->setCallbacks(new MyCallbacks());

    // Start the service
    pService->start();

    // Start advertising
    NimBLEAdvertising* pAdvertising = NimBLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UUID);
    
    pAdvertising->start();
    Serial.println("BLE Advertising started");
}

void loop() {
    // Handle connection state changes
    if (deviceConnected && !oldDeviceConnected) {
        oldDeviceConnected = deviceConnected;
        Serial.println("Client connected, ready to send/receive data");
    }
    if (!deviceConnected && oldDeviceConnected) {
        oldDeviceConnected = deviceConnected;
        Serial.println("Client disconnected, restarting advertising");
        pServer->startAdvertising();
    }

    // Send periodic data to client if connected
    if (deviceConnected) {
        String message = "Hello from ESP32-C3: " + String(millis() / 1000) + "s";
        pTxCharacteristic->setValue(message.c_str());
        pTxCharacteristic->notify();
        Serial.println("Sent: " + message);
        delay(1000); // Send every second
    }

    delay(100); // Small delay to prevent watchdog issues
}