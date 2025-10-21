# Sensor Node L8

## Configuration
- **Position**: L8
- **BLE Name**: NanoTFluna-L8
- **Giga R1 Input Pin**: 39

## Upload Instructions
1. Open `nano_33_ble_sensor_L8.ino` in Arduino IDE
2. Connect Arduino Nano 33 BLE via USB
3. Select Board: Tools → Board → Arduino Mbed OS Nano Boards → Arduino Nano 33 BLE
4. Select Port: Tools → Port → (select appropriate port)
5. Click Upload

## Verification
1. Open Serial Monitor (115200 baud)
2. Look for:
   ```
   Sensor Position: L8
   BLE initialized. Device name: NanoTFluna-L8
   ```
3. Use BLE scanner to verify device appears as `NanoTFluna-L8`

## Notes
This is a pre-configured version of the firmware for sensor L8.
The SENSOR_POSITION is already set to "L8" in the code.
