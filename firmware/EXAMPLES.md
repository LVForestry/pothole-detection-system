# Usage Examples

This document provides practical examples for using the Pothole Detection System.

## Table of Contents

1. [Web Dashboard Usage](#web-dashboard-usage)
2. [API Usage with curl](#api-usage-with-curl)
3. [API Usage with Python](#api-usage-with-python)
4. [API Usage with JavaScript](#api-usage-with-javascript)
5. [Arduino Serial Monitor](#arduino-serial-monitor)
6. [Integration Examples](#integration-examples)

## Web Dashboard Usage

### Accessing the Dashboard

1. Connect to WiFi:
   - Network: `Pothole-Detection-AP`
   - Password: (none - open network)

2. Open browser and navigate to:
   ```
   http://192.168.4.1/
   ```

### Monitoring Sensors

The dashboard automatically refreshes every 5 seconds. Each sensor card shows:
- **Name**: R1-R8 (right), L8-L1 (left)
- **State**: GREEN (normal) or RED (pothole detected)
- **Pin Number**: Giga R1 input pin
- **BLE Name**: Sensor node device name

### Polling a Sensor

1. Click **Poll Sensor** on any sensor card
2. Wait 1-8 seconds for BLE connection
3. Alert shows:
   - Sensor name
   - Current distance reading (cm)
   - Connection status

Example result:
```
Sensor R1
Distance: 125.50 cm
Status: connected
```

### Configuring a Sensor

1. Click **Configure** on any sensor card
2. Enter **zero reference** value (e.g., 150.0 cm)
3. Enter **threshold** value (e.g., 5.0 cm)
4. Click OK
5. Wait for confirmation

Example:
- Zero: `150.0` (normal road surface distance)
- Threshold: `5.0` (detect drops of 5cm or more)

## API Usage with curl

### Get All Sensor States

```bash
curl http://192.168.4.1/api/sensors
```

Response:
```json
{
  "sensors": [
    {
      "index": 0,
      "name": "R1",
      "pin": 23,
      "state": false,
      "ble_name": "NanoTFluna-R1"
    },
    {
      "index": 1,
      "name": "R2",
      "pin": 25,
      "state": true,
      "ble_name": "NanoTFluna-R2"
    }
    // ... more sensors
  ]
}
```

### Poll a Specific Sensor

```bash
# Poll sensor R1 (index 0)
curl http://192.168.4.1/api/poll/0
```

Response:
```json
{
  "success": true,
  "name": "R1",
  "ble_device": "NanoTFluna-R1",
  "distance": 125.50,
  "status": "connected",
  "pin_state": "LOW"
}
```

### Configure a Sensor

```bash
# Configure sensor R1 (index 0)
curl -X POST http://192.168.4.1/api/configure/0 \
  -H "Content-Type: application/json" \
  -d '{"zero": 150.0, "threshold": 5.0}'
```

Response:
```json
{
  "success": true,
  "message": "Configuration updated successfully",
  "sensor": "R1",
  "zero": 150.00,
  "threshold": 5.00
}
```

## API Usage with Python

### Prerequisites

```bash
pip install requests
```

### Get All Sensor States

```python
import requests

def get_sensor_states():
    """Get the state of all sensors."""
    response = requests.get('http://192.168.4.1/api/sensors')
    return response.json()

# Usage
sensors = get_sensor_states()
for sensor in sensors['sensors']:
    state = "ALERT" if sensor['state'] else "Normal"
    print(f"{sensor['name']}: {state} (Pin {sensor['pin']})")
```

Output:
```
R1: Normal (Pin 23)
R2: ALERT (Pin 25)
R3: Normal (Pin 27)
...
```

### Poll a Sensor

```python
import requests

def poll_sensor(index):
    """Poll a specific sensor for current distance."""
    response = requests.get(f'http://192.168.4.1/api/poll/{index}')
    return response.json()

# Usage
result = poll_sensor(0)  # Poll sensor R1
if result['success']:
    print(f"Sensor {result['name']}")
    print(f"Distance: {result['distance']} cm")
    print(f"Pin State: {result['pin_state']}")
else:
    print("Failed to poll sensor")
```

Output:
```
Sensor R1
Distance: 125.5 cm
Pin State: LOW
```

### Configure a Sensor

```python
import requests

def configure_sensor(index, zero_ref, threshold):
    """Configure a sensor's zero reference and threshold."""
    url = f'http://192.168.4.1/api/configure/{index}'
    data = {
        'zero': zero_ref,
        'threshold': threshold
    }
    response = requests.post(url, json=data)
    return response.json()

# Usage
result = configure_sensor(0, 150.0, 5.0)
if result['success']:
    print(f"Configuration successful for {result['sensor']}")
    print(f"Zero: {result['zero']} cm")
    print(f"Threshold: {result['threshold']} cm")
else:
    print(f"Configuration failed: {result['message']}")
```

Output:
```
Configuration successful for R1
Zero: 150.0 cm
Threshold: 5.0 cm
```

### Monitor System Continuously

```python
import requests
import time

def monitor_system(interval=5):
    """Continuously monitor the system."""
    print("Monitoring system... (Press Ctrl+C to stop)")
    try:
        while True:
            sensors = requests.get('http://192.168.4.1/api/sensors').json()
            
            # Check for alerts
            alerts = [s for s in sensors['sensors'] if s['state']]
            
            if alerts:
                print(f"\n[{time.strftime('%H:%M:%S')}] ALERTS:")
                for sensor in alerts:
                    print(f"  - {sensor['name']} (Pin {sensor['pin']})")
            else:
                print(f"[{time.strftime('%H:%M:%S')}] All sensors normal")
            
            time.sleep(interval)
    except KeyboardInterrupt:
        print("\nMonitoring stopped")

# Usage
monitor_system(interval=5)  # Check every 5 seconds
```

Output:
```
Monitoring system... (Press Ctrl+C to stop)
[14:23:10] All sensors normal
[14:23:15] All sensors normal
[14:23:20] ALERTS:
  - R2 (Pin 25)
  - L3 (Pin 49)
[14:23:25] ALERTS:
  - R2 (Pin 25)
[14:23:30] All sensors normal
```

## API Usage with JavaScript

### Get All Sensor States

```javascript
async function getSensorStates() {
  const response = await fetch('http://192.168.4.1/api/sensors');
  const data = await response.json();
  return data;
}

// Usage
getSensorStates().then(data => {
  data.sensors.forEach(sensor => {
    const state = sensor.state ? 'ALERT' : 'Normal';
    console.log(`${sensor.name}: ${state} (Pin ${sensor.pin})`);
  });
});
```

### Poll a Sensor

```javascript
async function pollSensor(index) {
  const response = await fetch(`http://192.168.4.1/api/poll/${index}`);
  const data = await response.json();
  return data;
}

// Usage
pollSensor(0).then(result => {
  if (result.success) {
    console.log(`Sensor ${result.name}`);
    console.log(`Distance: ${result.distance} cm`);
    console.log(`Pin State: ${result.pin_state}`);
  } else {
    console.log('Failed to poll sensor');
  }
});
```

### Configure a Sensor

```javascript
async function configureSensor(index, zero, threshold) {
  const response = await fetch(`http://192.168.4.1/api/configure/${index}`, {
    method: 'POST',
    headers: {
      'Content-Type': 'application/json'
    },
    body: JSON.stringify({ zero, threshold })
  });
  const data = await response.json();
  return data;
}

// Usage
configureSensor(0, 150.0, 5.0).then(result => {
  if (result.success) {
    console.log(`Configuration successful for ${result.sensor}`);
    console.log(`Zero: ${result.zero} cm`);
    console.log(`Threshold: ${result.threshold} cm`);
  } else {
    console.log(`Configuration failed: ${result.message}`);
  }
});
```

### Real-time Monitoring Dashboard

```html
<!DOCTYPE html>
<html>
<head>
  <title>Custom Pothole Monitor</title>
  <script>
    async function updateStatus() {
      try {
        const response = await fetch('http://192.168.4.1/api/sensors');
        const data = await response.json();
        
        const statusDiv = document.getElementById('status');
        statusDiv.innerHTML = '';
        
        data.sensors.forEach(sensor => {
          const div = document.createElement('div');
          div.style.color = sensor.state ? 'red' : 'green';
          div.textContent = `${sensor.name}: ${sensor.state ? 'ALERT' : 'Normal'}`;
          statusDiv.appendChild(div);
        });
      } catch (error) {
        console.error('Error fetching data:', error);
      }
    }
    
    // Update every 2 seconds
    setInterval(updateStatus, 2000);
    
    // Initial update
    window.onload = updateStatus;
  </script>
</head>
<body>
  <h1>Pothole Detection Monitor</h1>
  <div id="status"></div>
</body>
</html>
```

## Arduino Serial Monitor

### Reading Events

1. Open Serial Monitor (115200 baud)
2. Events are logged automatically

Example output:
```
Arduino Giga R1 Central Controller
Pothole Detection System
================================
Initializing sensor input pins...
Pin 23 (R1) configured as INPUT
Pin 25 (R2) configured as INPUT
...
BLE initialized successfully
WiFi Access Point created successfully
SSID: Pothole-Detection-AP
IP Address: 192.168.4.1
System initialized successfully
Ready to monitor sensors

================================
POTHOLE DETECTED!
Sensor: R2 (Pin 25)
Timestamp: 45230 ms
================================

New web client connected
Request: GET / HTTP/1.1
Client disconnected

================================
POTHOLE DETECTED!
Sensor: L3 (Pin 49)
Timestamp: 67450 ms
================================
```

## Integration Examples

### Example 1: Data Logger

```python
import requests
import csv
from datetime import datetime

def log_sensor_data(filename='sensor_log.csv'):
    """Log sensor states to CSV file."""
    sensors = requests.get('http://192.168.4.1/api/sensors').json()
    
    with open(filename, 'a', newline='') as f:
        writer = csv.writer(f)
        
        # Write header if file is empty
        if f.tell() == 0:
            writer.writerow(['Timestamp', 'Sensor', 'State'])
        
        # Write sensor states
        timestamp = datetime.now().isoformat()
        for sensor in sensors['sensors']:
            state = 'HIGH' if sensor['state'] else 'LOW'
            writer.writerow([timestamp, sensor['name'], state])
    
    print(f"Logged data for {len(sensors['sensors'])} sensors")

# Run every minute
import time
while True:
    log_sensor_data()
    time.sleep(60)
```

### Example 2: Alert System

```python
import requests
import time
from datetime import datetime

def send_email_alert(sensor_name):
    """Send email alert (placeholder - implement with your email service)."""
    print(f"ALERT: Pothole detected on sensor {sensor_name}")
    # Implement actual email sending here

def monitor_with_alerts():
    """Monitor system and send alerts."""
    last_states = {}
    
    while True:
        sensors = requests.get('http://192.168.4.1/api/sensors').json()
        
        for sensor in sensors['sensors']:
            name = sensor['name']
            state = sensor['state']
            
            # Check for new alert (LOW to HIGH transition)
            if state and not last_states.get(name, False):
                timestamp = datetime.now().strftime('%Y-%m-%d %H:%M:%S')
                print(f"[{timestamp}] Alert: {name}")
                send_email_alert(name)
            
            last_states[name] = state
        
        time.sleep(2)

monitor_with_alerts()
```

### Example 3: Calibration Tool

```python
import requests
import time

def calibrate_all_sensors(zero_ref=150.0, threshold=5.0):
    """Calibrate all sensors with the same values."""
    print("Starting calibration...")
    
    for i in range(16):
        print(f"Configuring sensor {i}...", end=' ')
        result = requests.post(
            f'http://192.168.4.1/api/configure/{i}',
            json={'zero': zero_ref, 'threshold': threshold}
        ).json()
        
        if result['success']:
            print("✓")
        else:
            print(f"✗ ({result['message']})")
        
        time.sleep(1)  # Delay between sensors
    
    print("Calibration complete!")

# Usage
calibrate_all_sensors(zero_ref=150.0, threshold=5.0)
```

### Example 4: Real-time Distance Monitor

```python
import requests
import time

def monitor_distances(sensor_indices):
    """Monitor distances for specific sensors."""
    print("Monitoring distances... (Press Ctrl+C to stop)")
    
    try:
        while True:
            print("\n" + "="*50)
            print(f"Time: {time.strftime('%H:%M:%S')}")
            print("="*50)
            
            for index in sensor_indices:
                result = requests.get(f'http://192.168.4.1/api/poll/{index}').json()
                
                if result['success']:
                    print(f"{result['name']}: {result['distance']:.2f} cm")
                else:
                    print(f"Sensor {index}: Failed to read")
            
            time.sleep(10)  # Wait 10 seconds between readings
    except KeyboardInterrupt:
        print("\nMonitoring stopped")

# Monitor sensors R1, R2, and L1
monitor_distances([0, 1, 15])
```

Output:
```
Monitoring distances... (Press Ctrl+C to stop)

==================================================
Time: 14:30:15
==================================================
R1: 148.50 cm
R2: 152.30 cm
L1: 149.80 cm

==================================================
Time: 14:30:25
==================================================
R1: 148.45 cm
R2: 145.10 cm  # Distance decreased!
L1: 149.85 cm
```

## Tips and Best Practices

### API Usage

1. **Rate Limiting**: Don't poll too frequently
   - Recommended: 1-5 second intervals for monitoring
   - BLE operations: Wait at least 10 seconds between polls

2. **Error Handling**: Always check for success
   ```python
   result = poll_sensor(0)
   if result['success']:
       # Process data
   else:
       # Handle error
   ```

3. **Timeout**: Set appropriate timeouts
   ```python
   requests.get(url, timeout=10)
   ```

### Dashboard Usage

1. **Refresh Rate**: Dashboard auto-refreshes every 5 seconds
2. **BLE Operations**: Be patient - can take 1-8 seconds
3. **Multiple Users**: Only one client can access at a time
4. **Browser Cache**: Clear if seeing stale data

### Serial Monitor

1. **Baud Rate**: Always set to 115200
2. **Logging**: Keep monitor open to catch all events
3. **Timestamps**: Note system timestamps for correlation

## Troubleshooting Examples

### Check if System is Responding

```bash
# Ping the system
ping 192.168.4.1

# Test HTTP connectivity
curl -I http://192.168.4.1/
```

### Test Each Sensor Individually

```python
import requests

for i in range(16):
    try:
        response = requests.get(f'http://192.168.4.1/api/poll/{i}', timeout=15)
        result = response.json()
        status = "✓" if result['success'] else "✗"
        print(f"Sensor {i} ({result['name']}): {status}")
    except Exception as e:
        print(f"Sensor {i}: Error - {e}")
```

### Verify Configuration

```python
import requests

# Poll sensor to verify its current behavior
result = requests.get('http://192.168.4.1/api/poll/0').json()
print(f"Current distance: {result['distance']} cm")

# Reconfigure if needed
config_result = requests.post(
    'http://192.168.4.1/api/configure/0',
    json={'zero': 150.0, 'threshold': 5.0}
).json()

print(f"Configuration: {config_result['message']}")
```

## Additional Resources

- [README.md](giga_r1_central_controller/README.md) - Full feature documentation
- [ARCHITECTURE.md](ARCHITECTURE.md) - System design details
- [INSTALLATION.md](INSTALLATION.md) - Installation guide
- [QUICKSTART.md](QUICKSTART.md) - Quick start guide

---

For more examples or questions, refer to the project documentation or raise an issue in the repository.
