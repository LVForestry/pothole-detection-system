#!/usr/bin/env python3
"""
Generate individual sensor configuration sketches for all 16 sensor positions.
This creates separate directories for each sensor with its own .ino file.

Usage:
    python3 generate_configs.py
    
This will create:
    examples/R1/nano_33_ble_sensor_R1.ino
    examples/R2/nano_33_ble_sensor_R2.ino
    ...
    examples/L1/nano_33_ble_sensor_L1.ino
"""

import os
import shutil

# Define sensor positions
RIGHT_SENSORS = ["R1", "R2", "R3", "R4", "R5", "R6", "R7", "R8"]
LEFT_SENSORS = ["L8", "L7", "L6", "L5", "L4", "L3", "L2", "L1"]
ALL_SENSORS = RIGHT_SENSORS + LEFT_SENSORS

# Pin mappings for reference
PIN_MAPPING = {
    "R1": 23, "R2": 25, "R3": 27, "R4": 29,
    "R5": 31, "R6": 33, "R7": 35, "R8": 37,
    "L8": 39, "L7": 41, "L6": 43, "L5": 45,
    "L4": 47, "L3": 49, "L2": 51, "L1": 53
}

def read_template():
    """Read the base template file"""
    template_path = "nano_33_ble_sensor.ino"
    if not os.path.exists(template_path):
        print(f"Error: Template file '{template_path}' not found.")
        print("Please run this script from the nano_33_ble_sensor directory.")
        return None
    
    with open(template_path, 'r') as f:
        return f.read()

def update_sensor_position(content, position):
    """Update the SENSOR_POSITION define in the content"""
    lines = content.split('\n')
    for i, line in enumerate(lines):
        if line.strip().startswith('#define SENSOR_POSITION'):
            lines[i] = f'#define SENSOR_POSITION "{position}"'
            break
    return '\n'.join(lines)

def create_sensor_sketch(position, template_content):
    """Create a sensor sketch for the given position"""
    # Create directory
    dir_path = f"examples/{position}"
    os.makedirs(dir_path, exist_ok=True)
    
    # Update content with sensor position
    content = update_sensor_position(template_content, position)
    
    # Write sketch file
    sketch_path = os.path.join(dir_path, f"nano_33_ble_sensor_{position}.ino")
    with open(sketch_path, 'w') as f:
        f.write(content)
    
    # Create README for this sensor
    readme_content = f"""# Sensor Node {position}

## Configuration
- **Position**: {position}
- **BLE Name**: NanoTFluna-{position}
- **Giga R1 Input Pin**: {PIN_MAPPING[position]}

## Upload Instructions
1. Open `nano_33_ble_sensor_{position}.ino` in Arduino IDE
2. Connect Arduino Nano 33 BLE via USB
3. Select Board: Tools → Board → Arduino Mbed OS Nano Boards → Arduino Nano 33 BLE
4. Select Port: Tools → Port → (select appropriate port)
5. Click Upload

## Verification
1. Open Serial Monitor (115200 baud)
2. Look for:
   ```
   Sensor Position: {position}
   BLE initialized. Device name: NanoTFluna-{position}
   ```
3. Use BLE scanner to verify device appears as `NanoTFluna-{position}`

## Notes
This is a pre-configured version of the firmware for sensor {position}.
The SENSOR_POSITION is already set to "{position}" in the code.
"""
    
    readme_path = os.path.join(dir_path, "README.md")
    with open(readme_path, 'w') as f:
        f.write(readme_content)
    
    print(f"Created: {sketch_path}")

def create_examples_readme():
    """Create main README for examples directory"""
    content = """# Pre-configured Sensor Examples

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
"""
    
    readme_path = "examples/README.md"
    with open(readme_path, 'w') as f:
        f.write(content)
    
    print(f"Created: {readme_path}")

def main():
    """Main function"""
    print("=== Sensor Configuration Generator ===\n")
    
    # Read template
    print("Reading template file...")
    template_content = read_template()
    if template_content is None:
        return
    
    print(f"Template loaded ({len(template_content)} bytes)\n")
    
    # Create examples directory if it doesn't exist
    os.makedirs("examples", exist_ok=True)
    
    # Generate sketches for all sensors
    print("Generating sensor-specific sketches...")
    for position in ALL_SENSORS:
        create_sensor_sketch(position, template_content)
    
    # Create main README for examples
    print("\nCreating examples README...")
    create_examples_readme()
    
    print(f"\n=== Complete ===")
    print(f"Generated {len(ALL_SENSORS)} sensor configurations in 'examples/' directory")
    print("\nNext steps:")
    print("1. Navigate to examples/<POSITION>/ directory")
    print("2. Open nano_33_ble_sensor_<POSITION>.ino in Arduino IDE")
    print("3. Upload to corresponding Arduino Nano 33 BLE board")
    print("\nSee examples/README.md for detailed instructions.")

if __name__ == "__main__":
    main()
