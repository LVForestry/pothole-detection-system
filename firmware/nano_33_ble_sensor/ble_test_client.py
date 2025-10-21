#!/usr/bin/env python3
"""
BLE Test Client for Arduino Nano 33 BLE Sensor Nodes
This script demonstrates how to connect to and interact with the sensor nodes via BLE.

Requirements:
    pip install bleak

Usage:
    python3 ble_test_client.py [SENSOR_NAME]
    
Example:
    python3 ble_test_client.py NanoTFluna-R1
"""

import asyncio
import sys
from bleak import BleakScanner, BleakClient

# BLE Service and Characteristic UUIDs
SERVICE_UUID = "12340000-1234-5678-9abc-def012345678"
DISTANCE_CHAR_UUID = "12340001-1234-5678-9abc-def012345678"
ZERO_REF_CHAR_UUID = "12340002-1234-5678-9abc-def012345678"
THRESHOLD_CHAR_UUID = "12340003-1234-5678-9abc-def012345678"
STATUS_CHAR_UUID = "12340004-1234-5678-9abc-def012345678"

async def scan_for_sensors():
    """Scan for all NanoTFluna sensors"""
    print("Scanning for NanoTFluna sensors...")
    devices = await BleakScanner.discover(timeout=5.0)
    
    sensors = [d for d in devices if d.name and d.name.startswith("NanoTFluna-")]
    
    if not sensors:
        print("No sensors found.")
        return None
    
    print(f"\nFound {len(sensors)} sensor(s):")
    for i, sensor in enumerate(sensors):
        print(f"  {i+1}. {sensor.name} ({sensor.address})")
    
    return sensors

async def connect_to_sensor(device_name=None):
    """Connect to a specific sensor or scan and select"""
    if device_name:
        print(f"Scanning for {device_name}...")
        devices = await BleakScanner.discover(timeout=5.0)
        device = next((d for d in devices if d.name == device_name), None)
        
        if not device:
            print(f"Error: Sensor '{device_name}' not found.")
            return None
    else:
        sensors = await scan_for_sensors()
        if not sensors:
            return None
        
        if len(sensors) == 1:
            device = sensors[0]
        else:
            choice = input("\nSelect sensor number (or 'q' to quit): ")
            if choice.lower() == 'q':
                return None
            try:
                idx = int(choice) - 1
                if 0 <= idx < len(sensors):
                    device = sensors[idx]
                else:
                    print("Invalid selection.")
                    return None
            except ValueError:
                print("Invalid input.")
                return None
    
    print(f"\nConnecting to {device.name}...")
    client = BleakClient(device.address)
    
    try:
        await client.connect()
        print(f"Connected to {device.name}")
        return client
    except Exception as e:
        print(f"Failed to connect: {e}")
        return None

async def read_distance(client):
    """Read current distance from sensor"""
    try:
        value = await client.read_gatt_char(DISTANCE_CHAR_UUID)
        distance = int.from_bytes(value, byteorder='little', signed=True)
        return distance
    except Exception as e:
        print(f"Error reading distance: {e}")
        return None

async def read_zero_reference(client):
    """Read zero reference value"""
    try:
        value = await client.read_gatt_char(ZERO_REF_CHAR_UUID)
        zero_ref = int.from_bytes(value, byteorder='little', signed=True)
        return zero_ref
    except Exception as e:
        print(f"Error reading zero reference: {e}")
        return None

async def read_threshold(client):
    """Read threshold value"""
    try:
        value = await client.read_gatt_char(THRESHOLD_CHAR_UUID)
        threshold = int.from_bytes(value, byteorder='little', signed=True)
        return threshold
    except Exception as e:
        print(f"Error reading threshold: {e}")
        return None

async def read_status(client):
    """Read status string"""
    try:
        value = await client.read_gatt_char(STATUS_CHAR_UUID)
        status = value.decode('utf-8').rstrip('\x00')
        return status
    except Exception as e:
        print(f"Error reading status: {e}")
        return None

async def write_zero_reference(client, value):
    """Write new zero reference value"""
    try:
        data = value.to_bytes(2, byteorder='little', signed=True)
        await client.write_gatt_char(ZERO_REF_CHAR_UUID, data)
        print(f"Zero reference set to {value} cm")
        return True
    except Exception as e:
        print(f"Error writing zero reference: {e}")
        return False

async def write_threshold(client, value):
    """Write new threshold value"""
    try:
        data = value.to_bytes(2, byteorder='little', signed=True)
        await client.write_gatt_char(THRESHOLD_CHAR_UUID, data)
        print(f"Threshold set to {value} cm")
        return True
    except Exception as e:
        print(f"Error writing threshold: {e}")
        return False

def distance_notification_handler(sender, data):
    """Handler for distance notifications"""
    distance = int.from_bytes(data, byteorder='little', signed=True)
    print(f"Distance update: {distance} cm")

async def monitor_distance(client, duration=10):
    """Monitor distance for specified duration"""
    print(f"\nMonitoring distance for {duration} seconds...")
    await client.start_notify(DISTANCE_CHAR_UUID, distance_notification_handler)
    await asyncio.sleep(duration)
    await client.stop_notify(DISTANCE_CHAR_UUID)

async def interactive_session(client):
    """Interactive session with the sensor"""
    while True:
        print("\n=== Sensor Control Menu ===")
        print("1. Read current distance")
        print("2. Read configuration (zero ref & threshold)")
        print("3. Read status")
        print("4. Set zero reference")
        print("5. Set threshold")
        print("6. Monitor distance (10 seconds)")
        print("7. Calibrate (set current distance as zero)")
        print("q. Quit")
        
        choice = input("\nSelect option: ").strip().lower()
        
        if choice == 'q':
            break
        elif choice == '1':
            distance = await read_distance(client)
            if distance is not None:
                print(f"Current distance: {distance} cm")
        elif choice == '2':
            zero_ref = await read_zero_reference(client)
            threshold = await read_threshold(client)
            if zero_ref is not None and threshold is not None:
                print(f"Zero reference: {zero_ref} cm")
                print(f"Threshold: {threshold} cm")
        elif choice == '3':
            status = await read_status(client)
            if status is not None:
                print(f"Status: {status}")
        elif choice == '4':
            try:
                value = int(input("Enter new zero reference (cm): "))
                await write_zero_reference(client, value)
            except ValueError:
                print("Invalid input. Please enter an integer.")
        elif choice == '5':
            try:
                value = int(input("Enter new threshold (cm): "))
                await write_threshold(client, value)
            except ValueError:
                print("Invalid input. Please enter an integer.")
        elif choice == '6':
            await monitor_distance(client)
        elif choice == '7':
            distance = await read_distance(client)
            if distance is not None:
                print(f"Current distance: {distance} cm")
                confirm = input(f"Set zero reference to {distance} cm? (y/n): ")
                if confirm.lower() == 'y':
                    await write_zero_reference(client, distance)
        else:
            print("Invalid option. Please try again.")

async def main():
    """Main function"""
    device_name = None
    if len(sys.argv) > 1:
        device_name = sys.argv[1]
    
    client = await connect_to_sensor(device_name)
    
    if client:
        try:
            # Read initial values
            print("\n=== Initial Sensor State ===")
            distance = await read_distance(client)
            zero_ref = await read_zero_reference(client)
            threshold = await read_threshold(client)
            status = await read_status(client)
            
            if distance is not None:
                print(f"Distance: {distance} cm")
            if zero_ref is not None:
                print(f"Zero Reference: {zero_ref} cm")
            if threshold is not None:
                print(f"Threshold: {threshold} cm")
            if status is not None:
                print(f"Status: {status}")
            
            # Interactive session
            await interactive_session(client)
            
        finally:
            await client.disconnect()
            print("\nDisconnected.")
    else:
        print("Failed to connect to sensor.")

if __name__ == "__main__":
    try:
        asyncio.run(main())
    except KeyboardInterrupt:
        print("\n\nInterrupted by user.")
        sys.exit(0)
