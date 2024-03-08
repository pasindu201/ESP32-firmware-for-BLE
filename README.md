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

### BLE Service UUID

- Service UUID: 6E400001-B5A3-F393-E0A9-E50E24DCCA9E
- Characteristic UUID: 6E400002-B5A3-F393-E0A9-E50E24DCCA9E
