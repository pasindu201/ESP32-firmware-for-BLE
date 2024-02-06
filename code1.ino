#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

#define SERVICE_UUID        "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
#define RelayPin_1            22  // Assuming the relay is connected to GPIO 22

BLEServer *pServer = nullptr;
BLECharacteristic *pCharacteristic = nullptr;

class MyCallbacks : public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
        std::string value = pCharacteristic->getValue();
        Serial.print("Received message: ");
        Serial.println(value.c_str());

        if (value.length() > 0) {
            if (value[0] == 'SWITCH ON') {
                digitalWrite(RelayPin_1, HIGH);
                Serial.println("Setting pin 22 HIGH");
            } else if (value[0] == 'SWITCH OFF') {
                digitalWrite(RelayPin_1, LOW);
                Serial.println("Setting pin 22 LOW");
            }
        }
    }
};

void setup() {
    Serial.begin(115200);

    pinMode(RelayPin, OUTPUT);
    
    BLEDevice::init("ESP32_BLE_Serial");
    pServer = BLEDevice::createServer();
    BLEService *pService = pServer->createService(SERVICE_UUID);
    
    pCharacteristic = pService->createCharacteristic(
                          CHARACTERISTIC_UUID,
                          BLECharacteristic::PROPERTY_READ |
                          BLECharacteristic::PROPERTY_WRITE
                      );

    pCharacteristic->setCallbacks(new MyCallbacks());
    
    pService->start();

    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(pService->getUUID());
    pAdvertising->setScanResponse(true);
    pAdvertising->setMinPreferred(0x06);
    pAdvertising->setMinPreferred(0x12);
    
    BLEDevice::startAdvertising();
    
    Serial.println("Waiting for a connection...");
}
