#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <Arduino.h>
#include <string>
#include <iostream>
#include <map>
#include <sstream>
#include <iomanip>

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
        int spaceIdx = value.find(' ');
        std::string command = value.substr(startIdx, spaceIdx - startIdx);

        if (command == "ON"){
            std::string relayID = value.substr(spaceIdx + 1);
           
            // switch on the relays
            if (relayID == "1"){
                digitalWrite(RelayPin_1, HIGH);
                RelayStatus_1 = true;
            }
            else if (relayID == "2"){
                digitalWrite(RelayPin_2, HIGH);
                RelayStatus_2 = true;
            }
            else if (relayID == "3"){
                digitalWrite(RelayPin_3, HIGH);
                RelayStatus_3 = true;
            }
            else if (relayID == "4"){
                digitalWrite(RelayPin_4, HIGH);
                RelayStatus_4 = true;
            }
            else if (relayID == "5"){
                digitalWrite(RelayPin_5, HIGH);
                RelayStatus_5 = true;
            }  
            float sensorValue = readSensor();
          
            // Construct JSON message
            std::stringstream jsonMessage;
            jsonMessage << "{";
            jsonMessage << "\"command\": \"" << command << "\", ";
            jsonMessage << "\"relay_status\": {";
            jsonMessage << "\"Relay1\": \"" << (RelayStatus_1 ? "on" : "off") << "\", ";
            jsonMessage << "\"Relay2\": \"" << (RelayStatus_2 ? "on" : "off") << "\", ";
            jsonMessage << "\"Relay3\": \"" << (RelayStatus_3 ? "on" : "off") << "\", ";
            jsonMessage << "\"Relay4\": \"" << (RelayStatus_4 ? "on" : "off") << "\", ";
            jsonMessage << "\"Relay5\": \"" << (RelayStatus_5 ? "on" : "off") << "\"";
            jsonMessage << "}, ";
            jsonMessage << "\"sensor_value\": " << std::fixed << std::setprecision(2) << sensorValue;
            jsonMessage << "}";

            // Send the status message
            pCharacteristic->setValue(jsonMessage.str());
            pCharacteristic->notify();          
        }
  
        else if (command == "OFF"){
            std::string relayID = value.substr(spaceIdx + 1);
            // switch off the relays
            if (relayID == "1"){
                digitalWrite(RelayPin_1, LOW);
                RelayStatus_1 = false;
            }
            else if (relayID == "2"){
                digitalWrite(RelayPin_2, LOW);
                RelayStatus_2 = false;
            }
            else if (relayID == "3"){
                digitalWrite(RelayPin_3, LOW);
                RelayStatus_3 = false;
            }
            else if (relayID == "4"){
                digitalWrite(RelayPin_4, LOW);
                RelayStatus_4 = false;
            }
            else if (relayID == "5"){
                digitalWrite(RelayPin_5, LOW);
                RelayStatus_5 = false;
            }
            float sensorValue = readSensor();
          
            // Construct JSON message
            std::stringstream jsonMessage;
            jsonMessage << "{";
            jsonMessage << "\"command\": \"" << command << "\", ";
            jsonMessage << "\"relay_status\": {";
            jsonMessage << "\"Relay1\": \"" << (RelayStatus_1 ? "on" : "off") << "\", ";
            jsonMessage << "\"Relay2\": \"" << (RelayStatus_2 ? "on" : "off") << "\", ";
            jsonMessage << "\"Relay3\": \"" << (RelayStatus_3 ? "on" : "off") << "\", ";
            jsonMessage << "\"Relay4\": \"" << (RelayStatus_4 ? "on" : "off") << "\", ";
            jsonMessage << "\"Relay5\": \"" << (RelayStatus_5 ? "on" : "off") << "\"";
            jsonMessage << "}, ";
            jsonMessage << "\"sensor_value\": " << std::fixed << std::setprecision(2) << sensorValue;
            jsonMessage << "}";

            // Send the status message
            pCharacteristic->setValue(jsonMessage.str());
            pCharacteristic->notify();
        }

        else if (command == "Status") {
            float sensorValue = readSensor();
          
            // Construct JSON message
            std::stringstream jsonMessage;
            jsonMessage << "{";
            jsonMessage << "\"command\": \"" << command << "\", ";
            jsonMessage << "\"relay_status\": {";
            jsonMessage << "\"Relay1\": \"" << (RelayStatus_1 ? "on" : "off") << "\", ";
            jsonMessage << "\"Relay2\": \"" << (RelayStatus_2 ? "on" : "off") << "\", ";
            jsonMessage << "\"Relay3\": \"" << (RelayStatus_3 ? "on" : "off") << "\", ";
            jsonMessage << "\"Relay4\": \"" << (RelayStatus_4 ? "on" : "off") << "\", ";
            jsonMessage << "\"Relay5\": \"" << (RelayStatus_5 ? "on" : "off") << "\"";
            jsonMessage << "}, ";
            jsonMessage << "\"sensor_value\": " << std::fixed << std::setprecision(2) << sensorValue;
            jsonMessage << "}";

            // Send the status message
            pCharacteristic->setValue(jsonMessage.str());
            pCharacteristic->notify();
        }
        else {
            pCharacteristic->setValue("Unknown command");
            pCharacteristic->notify();
        }  
    }
};

class MyServerCallbacks : public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
        Serial.println("Connected");
    }

    void onDisconnect(BLEServer* pServer) {
        Serial.println("Disconnected. Restarting advertising...");
        BLEDevice::startAdvertising();
    }
};

void setup() {
    Serial.begin(115200);

    pinMode(RelayPin_1, OUTPUT);
    pinMode(RelayPin_2, OUTPUT);
    pinMode(RelayPin_3, OUTPUT);
    pinMode(RelayPin_4, OUTPUT);
    pinMode(RelayPin_5, OUTPUT);

    // initialy Set all relay pins to LOW
    digitalWrite(RelayPin_1, LOW);
    digitalWrite(RelayPin_2, LOW);
    digitalWrite(RelayPin_3, LOW);
    digitalWrite(RelayPin_4, LOW);
    digitalWrite(RelayPin_5, LOW);
    
    BLEDevice::init("ESP32_BLE_Serial");
    pServer = BLEDevice::createServer();
    pServer->setCallbacks(new MyServerCallbacks());  
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
    // delay(2000);
}
