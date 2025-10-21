# Sensor Node R2

## Configuration
- **Position**: R2
- **BLE Name**: NanoTFluna-R2
- **Giga R1 Input Pin**: 25

## Upload Instructions
1. Open `nano_33_ble_sensor_R2.ino` in Arduino IDE
2. Connect Arduino Nano 33 BLE via USB
3. Select Board: Tools → Board → Arduino Mbed OS Nano Boards → Arduino Nano 33 BLE
4. Select Port: Tools → Port → (select appropriate port)
5. Click Upload

## Verification
1. Open Serial Monitor (115200 baud)
2. Look for:
   ```
   Sensor Position: R2
   BLE initialized. Device name: NanoTFluna-R2
   ```
3. Use BLE scanner to verify device appears as `NanoTFluna-R2`

## Notes
This is a pre-configured version of the firmware for sensor R2.
The SENSOR_POSITION is already set to "R2" in the code.
