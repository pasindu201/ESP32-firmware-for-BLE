//================================================================
// FILE NAME          : Firmware.c
// COPYRIGHTS         : ZERO TECHNOLOGIES
// DISCRPTION         : hitech-controller-firmware
// HARDWARE           : ESP 32 
//================================================================

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <ArduinoJson.h>
#include <Arduino.h>
#include <string>
#include <iostream>
#include <map>
#include <sstream>
#include <iomanip>

#define SERVICE_UUID        "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"

// ESP 32 GPIOPins for controlling relays and reading sensor value.
#define Relay_1                1
#define Relay_2                2
#define Relay_3                3
#define Relay_4                4
#define Relay_5                5

#define Sensor_1              32
#define Sensor_2              33

// Define the connected devices to each relay and sensor.
//================================================================
std::string relays[] = {"Exhaust_fan", "light_1", "light_2"};
std::string sensors[] = {"Water_level", "temperature"};

int number_of_relays = sizeof(relays) / sizeof(relays[0]);
int number_of_sensors = sizeof(sensors) / sizeof(sensors[0]); 

// Tracking relay status
bool relay_status[number_of_relays] = {false};

//================================================================

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
            for (int relay_id=0, relay_id<number_of_relays, relay_id++) {
                jsonMessage << "\"Relay" + std::to_string(relay_id) + "\"": "{"id\":" + std::to_string(relay_id) +", \"type:\"" + relays[relay_id] + "," + "\"status\":" << (relay_status[relay_id] ? "on" : "off") << "\", ";
            }
            jsonMessage << "}, ";
            jsonMessage << "\"sensor_values\": {";
            for (int sensor_id=0, sensor_id<number_of_sensors, sensor_id++) {
                jsonMessage << "\"Sensor" + std::to_string(sensor_id) + "\"": "{"id\":" + std::to_string(sensor_id) +", \"type:\"" + sensors[sensor_id] + "," + "\"value\":" << std::to_string(sensor_values[sensor_id]) << "\", ";
            }
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
            for (int relay_id=0, relay_id<number_of_relays, relay_id++) {
                jsonMessage << "\"Relay" + std::to_string(relay_id) + "\"": "{"id\":" + std::to_string(relay_id) +", \"type:\"" + relays[relay_id] + "," + "\"status\":" << (relay_status[relay_id] ? "on" : "off") << "\", ";
            }
            jsonMessage << "}, ";
            jsonMessage << "\"sensor_values\": {";
            for (int sensor_id=0, sensor_id<number_of_sensors, sensor_id++) {
                jsonMessage << "\"Sensor" + std::to_string(sensor_id) + "\"": "{"id\":" + std::to_string(sensor_id) +", \"type:\"" + sensors[sensor_id] + "," + "\"value\":" << std::to_string(sensor_values[sensor_id]) << "\", ";
            }
            jsonMessage << "}";

            // Send the status message
            pCharacteristic->setValue(jsonMessage.str());
            pCharacteristic->notify();
        }

        else if (command == "Status") {
            // send the status about all the relays and sensor values.
            float sensorValue = readSensor();
          
            // Construct JSON message
            std::stringstream jsonMessage;
            jsonMessage << "{";
            jsonMessage << "\"command\": \"" << command << "\", ";
            jsonMessage << "\"relay_status\": {";
            for (int relay_id=0, relay_id<number_of_relays, relay_id++) {
                jsonMessage << "\"Relay" + std::to_string(relay_id) + "\"": "{"id\":" + std::to_string(relay_id) +", \"type:\"" + relays[relay_id] + "," + "\"status\":" << (relay_status[relay_id] ? "on" : "off") << "\", ";
            }
            jsonMessage << "}, ";
            jsonMessage << "\"sensor_values\": {";
            for (int sensor_id=0, sensor_id<number_of_sensors, sensor_id++) {
                jsonMessage << "\"Sensor" + std::to_string(sensor_id) + "\"": "{"id\":" + std::to_string(sensor_id) +", \"type:\"" + sensors[sensor_id] + "," + "\"value\":" << std::to_string(sensor_values[sensor_id]) << "\", ";
            }
            jsonMessage << "}";

            // Send the status message
            pCharacteristic->setValue(jsonMessage.str());
            pCharacteristic->notify();
        }

        else if (command == "Devices"){
            // Send the connected devices for each sensor.
            std::stringstream jsonMessage;
            jsonMessage << "{\"Relays\": {";
            for (int relay_id = 0, relay_id < number_of_relays, relay_id++) {
                jsonMessage << "\"Relay" + std::to_string(relay_id) + "\":" + relays[relay_id] + ","
            }
            jsonMessage << "},";

            jsonMessage << "{\"sensors\": {";
            for (int sensor_id = 0, sensor_id < number_of_sensors, sensor_id++) {
                jsonMessage << "\"Sensor" + std::to_string(sensor_id) + "\":" + sensors[sensor_id] + ","
            }
            jsonMessage << "}}";

            // Send the JSON message
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
