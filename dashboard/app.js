/**
 * Pothole Detection System - Dashboard Application
 * Connects to Arduino Giga R1 WiFi AP and monitors sensor states via BLE
 */

// Configuration
const CONFIG = {
    // Sensor names and their corresponding Giga R1 pins
    sensors: [
        { name: 'R1', pin: 23, bleDevice: 'NanoTFluna-R1' },
        { name: 'R2', pin: 25, bleDevice: 'NanoTFluna-R2' },
        { name: 'R3', pin: 27, bleDevice: 'NanoTFluna-R3' },
        { name: 'R4', pin: 29, bleDevice: 'NanoTFluna-R4' },
        { name: 'R5', pin: 31, bleDevice: 'NanoTFluna-R5' },
        { name: 'R6', pin: 33, bleDevice: 'NanoTFluna-R6' },
        { name: 'R7', pin: 35, bleDevice: 'NanoTFluna-R7' },
        { name: 'R8', pin: 37, bleDevice: 'NanoTFluna-R8' },
        { name: 'L8', pin: 39, bleDevice: 'NanoTFluna-L8' },
        { name: 'L7', pin: 41, bleDevice: 'NanoTFluna-L7' },
        { name: 'L6', pin: 43, bleDevice: 'NanoTFluna-L6' },
        { name: 'L5', pin: 45, bleDevice: 'NanoTFluna-L5' },
        { name: 'L4', pin: 47, bleDevice: 'NanoTFluna-L4' },
        { name: 'L3', pin: 49, bleDevice: 'NanoTFluna-L3' },
        { name: 'L2', pin: 51, bleDevice: 'NanoTFluna-L2' },
        { name: 'L1', pin: 53, bleDevice: 'NanoTFluna-L1' }
    ],
    // Polling interval for sensor states (ms)
    pollingInterval: 1000,
    // API endpoints (served by Giga R1)
    apiBaseUrl: '/api',
    // BLE service UUIDs (these should match the Arduino implementation)
    bleServiceUUID: '19B10000-E8F2-537E-4F6C-D104768A1214',
    bleCharacteristics: {
        distance: '19B10001-E8F2-537E-4F6C-D104768A1214',
        zero: '19B10002-E8F2-537E-4F6C-D104768A1214',
        threshold: '19B10003-E8F2-537E-4F6C-D104768A1214'
    }
};

// Application State
const state = {
    wifiConnected: false,
    bleAvailable: false,
    sensors: {},
    events: [],
    activeBLEDevice: null,
    pollingTimer: null
};

// Initialize state for all sensors
CONFIG.sensors.forEach(sensor => {
    state.sensors[sensor.name] = {
        status: 'unknown', // 'low', 'high', 'unknown'
        distance: null,
        zero: null,
        threshold: 5.0,
        lastUpdate: null
    };
});

// DOM Elements
const elements = {
    wifiStatus: document.getElementById('wifi-status'),
    wifiState: document.getElementById('wifi-state'),
    bleStatus: document.getElementById('ble-status'),
    bleState: document.getElementById('ble-state'),
    connectWifi: document.getElementById('connect-wifi'),
    checkBle: document.getElementById('check-ble'),
    connectionInfo: document.getElementById('connection-info'),
    sensorDetails: document.getElementById('sensor-details'),
    detailSensorName: document.getElementById('detail-sensor-name'),
    detailDistance: document.getElementById('detail-distance'),
    detailZero: document.getElementById('detail-zero'),
    detailThreshold: document.getElementById('detail-threshold'),
    detailStatus: document.getElementById('detail-status'),
    closeDetails: document.getElementById('close-details'),
    eventsLog: document.getElementById('events-log'),
    clearEvents: document.getElementById('clear-events'),
    configModal: document.getElementById('config-modal'),
    configSensorName: document.getElementById('config-sensor-name'),
    configZero: document.getElementById('config-zero'),
    configThreshold: document.getElementById('config-threshold'),
    configCurrentReading: document.getElementById('config-current-reading'),
    setZero: document.getElementById('set-zero'),
    saveConfig: document.getElementById('save-config'),
    closeConfig: document.getElementById('close-config'),
    cancelConfig: document.getElementById('cancel-config')
};

// Initialize the application
function init() {
    console.log('Initializing Pothole Detection Dashboard...');
    
    // Check BLE availability
    checkBLESupport();
    
    // Set up event listeners
    setupEventListeners();
    
    // Try to auto-connect if already on Giga's WiFi network
    checkWiFiConnection();
    
    // Update UI
    updateUI();
}

// Check if Web Bluetooth API is available
function checkBLESupport() {
    if (navigator.bluetooth) {
        state.bleAvailable = true;
        elements.bleState.textContent = 'Available';
        elements.bleStatus.classList.add('connected');
        elements.checkBle.disabled = false;
        console.log('Web Bluetooth API is available');
    } else {
        state.bleAvailable = false;
        elements.bleState.textContent = 'Not Supported';
        elements.bleStatus.classList.add('disconnected');
        showMessage('Web Bluetooth is not supported in this browser. Please use Chrome, Edge, or Opera.', 'error');
        console.warn('Web Bluetooth API is not available');
    }
}

// Check WiFi connection to Giga R1
async function checkWiFiConnection() {
    try {
        // Try to fetch sensor status from the Giga R1
        const response = await fetch(`${CONFIG.apiBaseUrl}/status`, {
            method: 'GET',
            headers: { 'Content-Type': 'application/json' }
        });
        
        if (response.ok) {
            state.wifiConnected = true;
            elements.wifiState.textContent = 'Connected';
            elements.wifiStatus.classList.remove('disconnected');
            elements.wifiStatus.classList.add('connected');
            elements.connectWifi.textContent = 'Refresh Connection';
            showMessage('Connected to Giga R1 WiFi AP', 'success');
            
            // Start polling for sensor states
            startPolling();
            
            return true;
        }
    } catch (error) {
        console.log('Not connected to Giga R1:', error.message);
    }
    
    state.wifiConnected = false;
    elements.wifiState.textContent = 'Disconnected';
    elements.wifiStatus.classList.remove('connected');
    elements.wifiStatus.classList.add('disconnected');
    return false;
}

// Connect to Giga R1 WiFi
async function connectToWiFi() {
    showMessage('Please connect to the Giga R1 WiFi Access Point using your device settings.', 'info');
    
    // Wait a moment, then check connection
    setTimeout(async () => {
        const connected = await checkWiFiConnection();
        if (!connected) {
            showMessage('Could not detect connection to Giga R1. Please ensure you are connected to the correct WiFi network.', 'error');
        }
    }, 2000);
}

// Start polling sensor states
function startPolling() {
    if (state.pollingTimer) {
        clearInterval(state.pollingTimer);
    }
    
    // Initial poll
    pollSensorStates();
    
    // Set up regular polling
    state.pollingTimer = setInterval(pollSensorStates, CONFIG.pollingInterval);
    console.log('Started polling sensor states');
}

// Stop polling sensor states
function stopPolling() {
    if (state.pollingTimer) {
        clearInterval(state.pollingTimer);
        state.pollingTimer = null;
        console.log('Stopped polling sensor states');
    }
}

// Poll all sensor states from Giga R1
async function pollSensorStates() {
    if (!state.wifiConnected) return;
    
    try {
        const response = await fetch(`${CONFIG.apiBaseUrl}/sensors`, {
            method: 'GET',
            headers: { 'Content-Type': 'application/json' }
        });
        
        if (response.ok) {
            const data = await response.json();
            
            // Update sensor states
            CONFIG.sensors.forEach(sensor => {
                const sensorData = data[sensor.name];
                if (sensorData) {
                    const previousStatus = state.sensors[sensor.name].status;
                    state.sensors[sensor.name].status = sensorData.status;
                    state.sensors[sensor.name].lastUpdate = new Date();
                    
                    // Log status change events
                    if (previousStatus !== 'unknown' && previousStatus !== sensorData.status) {
                        if (sensorData.status === 'high') {
                            addEvent(`Pothole detected by sensor ${sensor.name}!`, 'pothole');
                        } else {
                            addEvent(`Sensor ${sensor.name} returned to normal`, 'normal');
                        }
                    }
                }
            });
            
            updateUI();
        }
    } catch (error) {
        console.error('Error polling sensor states:', error);
        // Don't show error message for every failed poll
    }
}

// Poll individual sensor via BLE
async function pollSensorBLE(sensorName) {
    if (!state.bleAvailable) {
        showMessage('BLE is not available in this browser', 'error');
        return;
    }
    
    const sensorConfig = CONFIG.sensors.find(s => s.name === sensorName);
    if (!sensorConfig) {
        showMessage(`Sensor ${sensorName} not found`, 'error');
        return;
    }
    
    try {
        showMessage(`Connecting to ${sensorConfig.bleDevice}...`, 'info');
        
        // Request BLE device
        const device = await navigator.bluetooth.requestDevice({
            filters: [{ name: sensorConfig.bleDevice }],
            optionalServices: [CONFIG.bleServiceUUID]
        });
        
        console.log(`Connected to ${device.name}`);
        
        // Connect to GATT server
        const server = await device.gatt.connect();
        const service = await server.getPrimaryService(CONFIG.bleServiceUUID);
        
        // Read distance characteristic
        const distanceChar = await service.getCharacteristic(CONFIG.bleCharacteristics.distance);
        const distanceValue = await distanceChar.readValue();
        const distance = distanceValue.getFloat32(0, true);
        
        // Read zero and threshold if available
        let zero = null, threshold = null;
        try {
            const zeroChar = await service.getCharacteristic(CONFIG.bleCharacteristics.zero);
            const zeroValue = await zeroChar.readValue();
            zero = zeroValue.getFloat32(0, true);
        } catch (e) {
            console.log('Zero characteristic not available');
        }
        
        try {
            const thresholdChar = await service.getCharacteristic(CONFIG.bleCharacteristics.threshold);
            const thresholdValue = await thresholdChar.readValue();
            threshold = thresholdValue.getFloat32(0, true);
        } catch (e) {
            console.log('Threshold characteristic not available');
        }
        
        // Update state
        state.sensors[sensorName].distance = distance;
        if (zero !== null) state.sensors[sensorName].zero = zero;
        if (threshold !== null) state.sensors[sensorName].threshold = threshold;
        
        // Show details
        showSensorDetails(sensorName);
        
        // Disconnect
        device.gatt.disconnect();
        
        showMessage(`Successfully read data from ${sensorName}`, 'success');
        addEvent(`Polled sensor ${sensorName} via BLE: ${distance.toFixed(2)} cm`);
        
    } catch (error) {
        console.error('BLE Error:', error);
        showMessage(`Failed to connect to ${sensorName}: ${error.message}`, 'error');
    }
}

// Configure sensor via BLE
async function configureSensorBLE(sensorName, zero, threshold) {
    if (!state.bleAvailable) {
        showMessage('BLE is not available in this browser', 'error');
        return false;
    }
    
    const sensorConfig = CONFIG.sensors.find(s => s.name === sensorName);
    if (!sensorConfig) {
        showMessage(`Sensor ${sensorName} not found`, 'error');
        return false;
    }
    
    try {
        showMessage(`Connecting to ${sensorConfig.bleDevice}...`, 'info');
        
        // Request BLE device
        const device = await navigator.bluetooth.requestDevice({
            filters: [{ name: sensorConfig.bleDevice }],
            optionalServices: [CONFIG.bleServiceUUID]
        });
        
        // Connect to GATT server
        const server = await device.gatt.connect();
        const service = await server.getPrimaryService(CONFIG.bleServiceUUID);
        
        // Write zero value if provided
        if (zero !== null && zero !== undefined) {
            const zeroChar = await service.getCharacteristic(CONFIG.bleCharacteristics.zero);
            const buffer = new ArrayBuffer(4);
            const view = new DataView(buffer);
            view.setFloat32(0, zero, true);
            await zeroChar.writeValue(buffer);
            console.log(`Set zero to ${zero}`);
        }
        
        // Write threshold value if provided
        if (threshold !== null && threshold !== undefined) {
            const thresholdChar = await service.getCharacteristic(CONFIG.bleCharacteristics.threshold);
            const buffer = new ArrayBuffer(4);
            const view = new DataView(buffer);
            view.setFloat32(0, threshold, true);
            await thresholdChar.writeValue(buffer);
            console.log(`Set threshold to ${threshold}`);
        }
        
        // Update local state
        if (zero !== null) state.sensors[sensorName].zero = zero;
        if (threshold !== null) state.sensors[sensorName].threshold = threshold;
        
        // Disconnect
        device.gatt.disconnect();
        
        showMessage(`Successfully configured ${sensorName}`, 'success');
        addEvent(`Configured sensor ${sensorName}: zero=${zero}, threshold=${threshold}`);
        
        return true;
    } catch (error) {
        console.error('BLE Configuration Error:', error);
        showMessage(`Failed to configure ${sensorName}: ${error.message}`, 'error');
        return false;
    }
}

// Show sensor details panel
function showSensorDetails(sensorName) {
    const sensor = state.sensors[sensorName];
    
    elements.detailSensorName.textContent = sensorName;
    elements.detailDistance.textContent = sensor.distance !== null 
        ? `${sensor.distance.toFixed(2)} cm` 
        : '--';
    elements.detailZero.textContent = sensor.zero !== null 
        ? `${sensor.zero.toFixed(2)} cm` 
        : '--';
    elements.detailThreshold.textContent = sensor.threshold !== null 
        ? `${sensor.threshold.toFixed(2)} cm` 
        : '--';
    elements.detailStatus.textContent = sensor.status.toUpperCase();
    
    elements.sensorDetails.style.display = 'block';
}

// Show configuration modal
function showConfigModal(sensorName) {
    const sensor = state.sensors[sensorName];
    
    elements.configSensorName.textContent = sensorName;
    elements.configZero.value = sensor.zero !== null ? sensor.zero : '';
    elements.configThreshold.value = sensor.threshold !== null ? sensor.threshold : 5.0;
    elements.configCurrentReading.textContent = sensor.distance !== null 
        ? `${sensor.distance.toFixed(2)} cm` 
        : '--';
    
    elements.configModal.classList.add('active');
    
    // Store current sensor name for later use
    elements.configModal.dataset.sensor = sensorName;
}

// Hide configuration modal
function hideConfigModal() {
    elements.configModal.classList.remove('active');
    delete elements.configModal.dataset.sensor;
}

// Add event to log
function addEvent(message, type = 'info') {
    const event = {
        message,
        type,
        timestamp: new Date()
    };
    
    state.events.unshift(event);
    
    // Keep only last 50 events
    if (state.events.length > 50) {
        state.events = state.events.slice(0, 50);
    }
    
    updateEventsLog();
}

// Update events log display
function updateEventsLog() {
    if (state.events.length === 0) {
        elements.eventsLog.innerHTML = '<p class="no-events">No events recorded yet.</p>';
        return;
    }
    
    elements.eventsLog.innerHTML = state.events
        .map(event => `
            <div class="event-item ${event.type}">
                <div>${event.message}</div>
                <div class="event-time">${event.timestamp.toLocaleTimeString()}</div>
            </div>
        `)
        .join('');
}

// Clear events log
function clearEventsLog() {
    state.events = [];
    updateEventsLog();
}

// Show message to user
function showMessage(message, type = 'info') {
    elements.connectionInfo.textContent = message;
    elements.connectionInfo.className = `info-message ${type}`;
    elements.connectionInfo.style.display = 'block';
    
    // Auto-hide after 5 seconds
    setTimeout(() => {
        elements.connectionInfo.style.display = 'none';
    }, 5000);
}

// Update UI with current state
function updateUI() {
    // Update all sensor indicators
    CONFIG.sensors.forEach(sensor => {
        const indicator = document.getElementById(`sensor-${sensor.name}`);
        if (indicator) {
            const status = state.sensors[sensor.name].status;
            indicator.className = 'sensor-indicator';
            indicator.classList.add(status);
        }
    });
}

// Set up all event listeners
function setupEventListeners() {
    // Connection controls
    elements.connectWifi.addEventListener('click', connectToWiFi);
    elements.checkBle.addEventListener('click', checkBLESupport);
    
    // Sensor poll buttons
    document.querySelectorAll('.poll-btn').forEach(btn => {
        btn.addEventListener('click', (e) => {
            const sensorName = e.target.dataset.sensor;
            pollSensorBLE(sensorName);
        });
    });
    
    // Sensor config buttons
    document.querySelectorAll('.config-btn').forEach(btn => {
        btn.addEventListener('click', (e) => {
            const sensorName = e.target.dataset.sensor;
            showConfigModal(sensorName);
        });
    });
    
    // Details panel
    elements.closeDetails.addEventListener('click', () => {
        elements.sensorDetails.style.display = 'none';
    });
    
    // Configuration modal
    elements.closeConfig.addEventListener('click', hideConfigModal);
    elements.cancelConfig.addEventListener('click', hideConfigModal);
    
    elements.setZero.addEventListener('click', async () => {
        const sensorName = elements.configModal.dataset.sensor;
        const sensor = state.sensors[sensorName];
        
        if (sensor.distance !== null) {
            elements.configZero.value = sensor.distance.toFixed(2);
        } else {
            // Poll sensor first
            await pollSensorBLE(sensorName);
            const updatedSensor = state.sensors[sensorName];
            if (updatedSensor.distance !== null) {
                elements.configZero.value = updatedSensor.distance.toFixed(2);
                elements.configCurrentReading.textContent = `${updatedSensor.distance.toFixed(2)} cm`;
            }
        }
    });
    
    elements.saveConfig.addEventListener('click', async () => {
        const sensorName = elements.configModal.dataset.sensor;
        const zero = parseFloat(elements.configZero.value);
        const threshold = parseFloat(elements.configThreshold.value);
        
        if (isNaN(zero) || isNaN(threshold)) {
            showMessage('Please enter valid numbers for zero and threshold', 'error');
            return;
        }
        
        const success = await configureSensorBLE(sensorName, zero, threshold);
        if (success) {
            hideConfigModal();
        }
    });
    
    // Events log
    elements.clearEvents.addEventListener('click', clearEventsLog);
    
    // Click outside modal to close
    elements.configModal.addEventListener('click', (e) => {
        if (e.target === elements.configModal) {
            hideConfigModal();
        }
    });
}

// Clean up on page unload
window.addEventListener('beforeunload', () => {
    stopPolling();
});

// Initialize when DOM is ready
if (document.readyState === 'loading') {
    document.addEventListener('DOMContentLoaded', init);
} else {
    init();
}
