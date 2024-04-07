## Hitech Controler Firmware

### Overview

This firmware is designed for an ESP32 microcontroller to enable Bluetooth control of devices within a caravan. The firmware establishes a Bluetooth Low Energy (BLE) server that can be connected to by a compatible mobile app. Through the app, users can send commands to control relays connected to devices in the caravan, such as lights or appliances. Additionally, the firmware provides real-time feedback on relay status and sensor data.

### Features

- Bluetooth Connectivity: Utilizes BLE to establish a connection with a mobile app for remote control.
- Relay Control: Allows users to turn on/off up to five relays connected to devices within the caravan.
- Sensor Data: Reads sensor data (analog input) and includes it in the feedback sent to the mobile app.
- Real-time Status Updates: Sends real-time status updates including relay states and sensor readings.

### Hardware Setup

- Relay Pins (1-5): Connect to the control inputs of the devices you want to control (e.g., lights, appliances).
- Sensor Pin: Connects to an analog sensor (e.g., a potentiometer) to provide sensor data.
  Bluetooth Commands
- Command Format: Commands are sent in the format COMMAND RELAY_ID (e.g., "ON 1", "OFF 2", "Status"). ON: Turns on the specified relay.
  OFF: Turns off the specified relay.
- Status: Requests the current status of all relays and the sensor.

## How to set-up

### Hardware Requirements
- ESP32 microcontroller
- Relays 
- Sensors (LED, Temporature sensor)
- Any device you want to connect to realys (LEDs, Fan....) 

### Software Requirements
- Arduino IDE
- ESP32 board support installed in Arduino IDE
- Necessary libraries: BLEDevice, BLEUtils, BLEServer, ArduinoJson

### Installation
1. Clone or download the firmware repository to your local machine.
2. Open the `Firmware.ino` file in Arduino IDE.
3. Install the required libraries if not already installed.
4. Select the appropriate ESP32 board from the Arduino IDE's board manager.
5. Compile and upload the firmware to your ESP32 device.

## Usage
1. Power on your ESP32 device.
2. Connect to the ESP32 via a BLE-capable device (e.g., smartphone, tablet, or another microcontroller).
3. Use a BLE application capable of writing to characteristics to send commands to the ESP32.( e.g. nRF connecter)
   - Supported commands:
     - `ON <relayID>`: Turns on the specified relay (e.g., `ON 1` turns on Relay 1).
     - `OFF <relayID>`: Turns off the specified relay (e.g., `OFF 1` turns off Relay 1).
     - `Status`: Retrieves the status of all relays and the sensor value.
     - `Devices`: Retrieves the names of devices connected to each relay and the sensor name.
4. The ESP32 will respond to commands by updating relay statuses and sending status or device information back via BLE.


## Customization
- Modify device names (`device1` to `device5`) and the sensor name (`sensor1`) as per your setup.
- Adjust pin assignments (`RelayPin_1` to `RelayPin_5` and `sensorPin`) if connected differently.

## Troubleshooting
- If encountering issues, ensure the BLE device is properly connected and within range.
- Check the serial monitor for debug messages to identify any errors or unexpected behavior.

