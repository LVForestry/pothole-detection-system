# Sensor Node L1

## Configuration
- **Position**: L1
- **BLE Name**: NanoTFluna-L1
- **Giga R1 Input Pin**: 53

## Upload Instructions
1. Open `nano_33_ble_sensor_L1.ino` in Arduino IDE
2. Connect Arduino Nano 33 BLE via USB
3. Select Board: Tools → Board → Arduino Mbed OS Nano Boards → Arduino Nano 33 BLE
4. Select Port: Tools → Port → (select appropriate port)
5. Click Upload

## Verification
1. Open Serial Monitor (115200 baud)
2. Look for:
   ```
   Sensor Position: L1
   BLE initialized. Device name: NanoTFluna-L1
   ```
3. Use BLE scanner to verify device appears as `NanoTFluna-L1`

## Notes
This is a pre-configured version of the firmware for sensor L1.
The SENSOR_POSITION is already set to "L1" in the code.
