# Sensor Node L3

## Configuration
- **Position**: L3
- **BLE Name**: NanoTFluna-L3
- **Giga R1 Input Pin**: 49

## Upload Instructions
1. Open `nano_33_ble_sensor_L3.ino` in Arduino IDE
2. Connect Arduino Nano 33 BLE via USB
3. Select Board: Tools → Board → Arduino Mbed OS Nano Boards → Arduino Nano 33 BLE
4. Select Port: Tools → Port → (select appropriate port)
5. Click Upload

## Verification
1. Open Serial Monitor (115200 baud)
2. Look for:
   ```
   Sensor Position: L3
   BLE initialized. Device name: NanoTFluna-L3
   ```
3. Use BLE scanner to verify device appears as `NanoTFluna-L3`

## Notes
This is a pre-configured version of the firmware for sensor L3.
The SENSOR_POSITION is already set to "L3" in the code.
