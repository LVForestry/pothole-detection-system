# Sensor Node R5

## Configuration
- **Position**: R5
- **BLE Name**: NanoTFluna-R5
- **Giga R1 Input Pin**: 31

## Upload Instructions
1. Open `nano_33_ble_sensor_R5.ino` in Arduino IDE
2. Connect Arduino Nano 33 BLE via USB
3. Select Board: Tools → Board → Arduino Mbed OS Nano Boards → Arduino Nano 33 BLE
4. Select Port: Tools → Port → (select appropriate port)
5. Click Upload

## Verification
1. Open Serial Monitor (115200 baud)
2. Look for:
   ```
   Sensor Position: R5
   BLE initialized. Device name: NanoTFluna-R5
   ```
3. Use BLE scanner to verify device appears as `NanoTFluna-R5`

## Notes
This is a pre-configured version of the firmware for sensor R5.
The SENSOR_POSITION is already set to "R5" in the code.
