# Sensor Node R7

## Configuration
- **Position**: R7
- **BLE Name**: NanoTFluna-R7
- **Giga R1 Input Pin**: 35

## Upload Instructions
1. Open `nano_33_ble_sensor_R7.ino` in Arduino IDE
2. Connect Arduino Nano 33 BLE via USB
3. Select Board: Tools → Board → Arduino Mbed OS Nano Boards → Arduino Nano 33 BLE
4. Select Port: Tools → Port → (select appropriate port)
5. Click Upload

## Verification
1. Open Serial Monitor (115200 baud)
2. Look for:
   ```
   Sensor Position: R7
   BLE initialized. Device name: NanoTFluna-R7
   ```
3. Use BLE scanner to verify device appears as `NanoTFluna-R7`

## Notes
This is a pre-configured version of the firmware for sensor R7.
The SENSOR_POSITION is already set to "R7" in the code.
