# Pre-configured Sensor Examples

This directory contains pre-configured firmware for each of the 16 sensor positions.
Each subdirectory contains a ready-to-upload sketch for a specific sensor position.

## Directory Structure

### Right Side Sensors (R1-R8)
| Directory | BLE Name | Giga Pin | Description |
|-----------|----------|----------|-------------|
| R1/ | NanoTFluna-R1 | 23 | Rightmost sensor |
| R2/ | NanoTFluna-R2 | 25 | |
| R3/ | NanoTFluna-R3 | 27 | |
| R4/ | NanoTFluna-R4 | 29 | |
| R5/ | NanoTFluna-R5 | 31 | |
| R6/ | NanoTFluna-R6 | 33 | |
| R7/ | NanoTFluna-R7 | 35 | |
| R8/ | NanoTFluna-R8 | 37 | Inner right |

### Left Side Sensors (L8-L1)
| Directory | BLE Name | Giga Pin | Description |
|-----------|----------|----------|-------------|
| L8/ | NanoTFluna-L8 | 39 | Inner left |
| L7/ | NanoTFluna-L7 | 41 | |
| L6/ | NanoTFluna-L6 | 43 | |
| L5/ | NanoTFluna-L5 | 45 | |
| L4/ | NanoTFluna-L4 | 47 | |
| L3/ | NanoTFluna-L3 | 49 | |
| L2/ | NanoTFluna-L2 | 51 | |
| L1/ | NanoTFluna-L1 | 53 | Leftmost sensor |

## Quick Start

### Upload to a Specific Sensor

1. Navigate to the sensor's directory (e.g., `R1/`)
2. Open the `.ino` file in Arduino IDE
3. Connect the Arduino Nano 33 BLE
4. Select the correct board and port
5. Upload the firmware
6. Verify using Serial Monitor (115200 baud)

### Batch Upload Process

To upload to all sensors efficiently:

1. Start with R1, open `R1/nano_33_ble_sensor_R1.ino`
2. Upload to first board
3. Disconnect and label board as "R1"
4. Connect next board
5. Open `R2/nano_33_ble_sensor_R2.ino`
6. Upload to second board
7. Disconnect and label board as "R2"
8. Repeat for all 16 sensors

## Advantages of Pre-configured Sketches

- **No manual editing required**: Each sketch is already configured for its position
- **Reduced errors**: Eliminates risk of using wrong position in code
- **Faster deployment**: Can be uploaded directly without modifications
- **Easy verification**: Each directory's README shows expected output

## Alternative: Use Main Sketch

If you prefer to use the main sketch with manual configuration:
1. Use `../nano_33_ble_sensor.ino`
2. Edit `SENSOR_POSITION` define before each upload
3. See `../DEPLOYMENT.md` for detailed instructions

## Verification Checklist

For each uploaded sensor:
- [ ] Correct sketch opened (matches physical sensor position)
- [ ] Upload successful
- [ ] Serial Monitor shows correct position
- [ ] BLE name matches expected (use nRF Connect or similar)
- [ ] Board labeled with position
- [ ] TFLuna sensor connected
- [ ] Output wire connected to correct Giga pin

## Notes

- All sketches are identical except for the SENSOR_POSITION value
- Configuration settings (threshold, zero reference) can be changed via BLE after deployment
- Settings are stored in flash and persist across power cycles
- Each sketch includes full functionality: LIDAR reading, BLE, detection logic
