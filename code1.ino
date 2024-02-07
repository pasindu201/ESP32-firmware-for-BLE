#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <Arduino.h>  
#include <vector>
#include <string>

#define SERVICE_UUID        "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
#define RelayPin_1            18
#define RelayPin_2            19
#define RelayPin_3            21
#define RelayPin_4            22
#define RelayPin_5            23
#define sensorPin             35

bool RelayStatus_1 = false;
bool RelayStatus_2 = false;
bool RelayStatus_3 = false;
bool RelayStatus_4 = false;
bool RelayStatus_5 = false;

BLEServer *pServer = nullptr;
BLECharacteristic *pCharacteristic = nullptr;

float readSensor() {
   
    int sensorValue = analogRead(sensorPin);
    return sensorValue;
}

class MyCallbacks : public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
        std::string value = pCharacteristic->getValue();

        Serial.print("Received message: ");
        Serial.println(value.c_str());

        // Split the value by spaces
        int startIdx = 0;
        int endIdx = value.find(' ');
        std::string command = value.substr(startIdx, endIdx - startIdx);

        std::vector<std::string> relays;

        do {
            // Find the next space character
            startIdx = endIdx + 1;
            endIdx = value.find(' ', startIdx);

            // Extract each word
            std::string word = value.substr(startIdx, endIdx - startIdx);

            relays.push_back(word);
        } while (endIdx != -1);
        if (command == "ON"){
          // switch on the relays
          for (const auto& relayID : relays) {
              if (relayID == "Relay1"){
                digitalWrite(RelayPin_1, HIGH);
                RelayStatus_1 = true;
              }
              else if (relayID == "Relay2"){
                digitalWrite(RelayPin_2, HIGH);
                RelayStatus_2 = true;
              }
              else if (relayID == "Relay3"){
                digitalWrite(RelayPin_3, HIGH);
                RelayStatus_3 = true;
              }
             }
        }
  
        else if (command == "OFF"){
          // switch off the relays
          for (const auto& relayID : relays) {
              if (relayID == "Relay1"){
                digitalWrite(RelayPin_1, LOW);
                RelayStatus_1 = false;
              }
              else if (relayID == "Relay2"){
                digitalWrite(RelayPin_2, LOW);
                RelayStatus_2 = false;
              }
              else if (relayID == "Relay3"){
                digitalWrite(RelayPin_3, LOW);
                RelayStatus_3 = false;
              }
             }
        }
        else if (command == "Status") {
          float sensorValue = readSensor();
          
          // Construct message with relay status
          std::string statusMessage = "Relay Status: ";
          statusMessage += "Relay1 ";
          statusMessage += (RelayStatus_1 ? "on " : "off ");
          statusMessage += "Relay2 ";
          statusMessage += (RelayStatus_2 ? "on " : "off ");
          statusMessage += "Relay3 ";
          statusMessage += (RelayStatus_3 ? "on " : "off ");
          statusMessage += "Relay4 ";
          statusMessage += (RelayStatus_4 ? "on " : "off ");
          statusMessage += "Relay5 ";
          statusMessage += (RelayStatus_5 ? "on " : "off ");

          statusMessage += "SensorValue: ";
          statusMessage += std::to_string(sensorValue);

          // Send the status message
          pCharacteristic->setValue(statusMessage);
          pCharacteristic->notify();
        }  
    }
};

void setup() {
    Serial.begin(115200);

    pinMode(RelayPin_1, OUTPUT);
    pinMode(RelayPin_2, OUTPUT);
    pinMode(RelayPin_3, OUTPUT);
    pinMode(RelayPin_4, OUTPUT);
    pinMode(RelayPin_5, OUTPUT);

    // Set all relay pins to LOW
    digitalWrite(RelayPin_1, LOW);
    digitalWrite(RelayPin_2, LOW);
    digitalWrite(RelayPin_3, LOW);
    digitalWrite(RelayPin_4, LOW);
    digitalWrite(RelayPin_5, LOW);
    
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

void loop() {
    // Your main code can go here if needed
    delay(2000);
}
