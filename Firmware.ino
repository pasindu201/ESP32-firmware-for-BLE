//================================================================
// FILE NAME          : HITECH_Firmware
// COPYRIGHTS         : ZERO TECHNOLOGIES
// DISCRPTION         : hitech-controller-firmware
// HARDWARE           : ESP 32 
//================================================================

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <ArduinoJson.h>
#include <SPIFFS.h>
#include <string>

#define SERVICE_UUID        "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"

// ESP 32 GPIOPins for controlling relays and reading sensor value.
#define Relay_1          2
#define Relay_2          4
#define Relay_3          5
#define Relay_4         12
#define Relay_5         13
#define Relay_6         14

#define Sensor_1        18
#define Sensor_2        19
#define Sensor_3        21
#define Sensor_4        22
#define Sensor_5        23
#define Sensor_6        26

int number_of_relays = 6;
int number_of_sensors = 6;

BLEServer *pServer = nullptr;
BLECharacteristic *pCharacteristic = nullptr;

class MyCallbacks : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *pCharacteristic) {
    std::string value = pCharacteristic->getValue().c_str();

    Serial.print("Received message: ");
    Serial.println(value.c_str());

    // Split the value by spaces
    int startIdx = 0;
    int spaceIdx = value.find(' ');
    String command = value.substr(startIdx, spaceIdx - startIdx).c_str();  

    float sensor_values[6] = {analogRead(Sensor_1), analogRead(Sensor_2), analogRead(Sensor_3), analogRead(Sensor_4), analogRead(Sensor_5), analogRead(Sensor_6)};
 
    if (command == "ON" || command == "OFF" || command == "STATUS"){
      String relayID = value.substr(spaceIdx + 1).c_str();;

      // Open the config.json file
      File fileConfig = SPIFFS.open("/config.json", "r+");
      DynamicJsonDocument configJson(512);
      deserializeJson(configJson, fileConfig);

      fileConfig.seek(0);
      fileConfig.print("");

      if(command == "ON"){
        if(relayID == "1") digitalWrite(Relay_1, HIGH);
        else if(relayID == "2") digitalWrite(Relay_2, HIGH);
        else if(relayID == "3") digitalWrite(Relay_3, HIGH);
        else if(relayID == "4") digitalWrite(Relay_4, HIGH);
        else if(relayID == "5") digitalWrite(Relay_5, HIGH);
        else if(relayID == "6") digitalWrite(Relay_6, HIGH);
        configJson[relayID] = true;
      } 
      else if(command == "OFF"){
        if (relayID == "all") {
          digitalWrite(Relay_1, LOW);
          digitalWrite(Relay_2, LOW);
          digitalWrite(Relay_3, LOW);
          digitalWrite(Relay_4, LOW);
          digitalWrite(Relay_5, LOW);
          digitalWrite(Relay_6, LOW);
          configJson["1"] = false;
          configJson["2"] = false;
          configJson["3"] = false;
          configJson["4"] = false;
          configJson["5"] = false;
          configJson["6"] = false;
        } else {
          if(relayID == "1") digitalWrite(Relay_1, LOW);
          else if(relayID == "2") digitalWrite(Relay_2, LOW);
          else if(relayID == "3") digitalWrite(Relay_3, LOW);
          else if(relayID == "4") digitalWrite(Relay_4, LOW);
          else if(relayID == "5") digitalWrite(Relay_5, LOW);
          else if(relayID == "6") digitalWrite(Relay_6, LOW);
          configJson[relayID] = false;
        }    
      }
      serializeJson(configJson, fileConfig);
      fileConfig.close();

      // Open the devices.json file and initialize relays and sensors
      File fileDevices = SPIFFS.open("/devices.json", "r");
      DynamicJsonDocument devices(512);
      deserializeJson(devices, fileDevices);
      fileDevices.close();
     
      // Construct JSON message for response
      StaticJsonDocument<1024> response;
      JsonObject relay_status = response.createNestedObject("relays");
      for (int i = 1; i <= number_of_relays; i++) {
        JsonObject relay = relay_status.createNestedObject(String(i));
        relay["type"] = String(devices[String(i)]);
        relay["status"] = configJson[String(i)];
      }

      JsonObject sensor_values_obj = response.createNestedObject("sensors");
      for (int i = 1; i <= number_of_sensors; i++) {
        JsonObject sensor = sensor_values_obj.createNestedObject(String(i+10));
        sensor["type"] = String(devices[String(i+10)]);
        sensor["value"] = sensor_values[i-1];
      }

      char buffer[1024];
      size_t n = serializeJson(response, buffer);
      pCharacteristic->setValue(reinterpret_cast<uint8_t*>(buffer), n);
      pCharacteristic->notify();
    }  
    else {
      Serial.print("Invalid command");
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

  pinMode(Relay_1, OUTPUT);
  pinMode(Relay_2, OUTPUT);
  pinMode(Relay_3, OUTPUT);
  pinMode(Relay_4, OUTPUT);
  pinMode(Relay_5, OUTPUT);
  pinMode(Relay_6, OUTPUT);

  pinMode(Sensor_1, INPUT);
  pinMode(Sensor_2, INPUT);
  pinMode(Sensor_3, INPUT);
  pinMode(Sensor_4, INPUT);
  pinMode(Sensor_5, INPUT);
  pinMode(Sensor_6, INPUT);

  // Initialize SPIFFS
  if (!SPIFFS.begin(true)) {
    Serial.println("SPIFFS Mount Failed");
    return;
  }
  // Read the JSON object from the file
  File fileConfig = SPIFFS.open("/config.json", "r");
  DynamicJsonDocument config(512);
  deserializeJson(config, fileConfig);
  
  // Set the initial relay status
  digitalWrite(Relay_1, config["1"] ? HIGH : LOW);
  digitalWrite(Relay_2, config["2"] ? HIGH : LOW);
  digitalWrite(Relay_3, config["3"] ? HIGH : LOW);
  digitalWrite(Relay_4, config["4"] ? HIGH : LOW);
  digitalWrite(Relay_5, config["5"] ? HIGH : LOW);
  digitalWrite(Relay_6, config["6"] ? HIGH : LOW);

  fileConfig.close();
  
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
  delay(2000);
}