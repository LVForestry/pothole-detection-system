/**
 * Mock Server for Dashboard Testing
 * 
 * This is a simple Node.js server that simulates the Arduino Giga R1 API
 * for testing the dashboard without physical hardware.
 * 
 * Usage:
 *   node mock-server.js
 * 
 * Then open http://localhost:8080 in your browser
 */

const http = require('http');
const fs = require('fs');
const path = require('path');

const PORT = 8080;

// Mock sensor states
const sensorStates = {
    R1: { status: 'low' },
    R2: { status: 'low' },
    R3: { status: 'low' },
    R4: { status: 'low' },
    R5: { status: 'low' },
    R6: { status: 'low' },
    R7: { status: 'low' },
    R8: { status: 'low' },
    L8: { status: 'low' },
    L7: { status: 'low' },
    L6: { status: 'low' },
    L5: { status: 'low' },
    L4: { status: 'low' },
    L3: { status: 'low' },
    L2: { status: 'low' },
    L1: { status: 'low' }
};

// Randomly change some sensor states every few seconds
setInterval(() => {
    const sensors = Object.keys(sensorStates);
    const randomSensor = sensors[Math.floor(Math.random() * sensors.length)];
    const randomStatus = Math.random() > 0.7 ? 'high' : 'low';
    sensorStates[randomSensor].status = randomStatus;
    console.log(`[${new Date().toISOString()}] ${randomSensor} -> ${randomStatus}`);
}, 3000);

// MIME types for static files
const mimeTypes = {
    '.html': 'text/html',
    '.js': 'text/javascript',
    '.css': 'text/css',
    '.json': 'application/json',
    '.png': 'image/png',
    '.jpg': 'image/jpg',
    '.gif': 'image/gif',
    '.svg': 'image/svg+xml',
    '.ico': 'image/x-icon'
};

const server = http.createServer((req, res) => {
    console.log(`${req.method} ${req.url}`);
    
    // Enable CORS
    res.setHeader('Access-Control-Allow-Origin', '*');
    res.setHeader('Access-Control-Allow-Methods', 'GET, POST, OPTIONS');
    res.setHeader('Access-Control-Allow-Headers', 'Content-Type');
    
    if (req.method === 'OPTIONS') {
        res.writeHead(200);
        res.end();
        return;
    }
    
    // API endpoints
    if (req.url === '/api/status') {
        res.writeHead(200, { 'Content-Type': 'application/json' });
        res.end(JSON.stringify({
            status: 'ok',
            timestamp: new Date().toISOString()
        }));
        return;
    }
    
    if (req.url === '/api/sensors') {
        res.writeHead(200, { 'Content-Type': 'application/json' });
        res.end(JSON.stringify(sensorStates));
        return;
    }
    
    // Serve static files
    let filePath = '.' + req.url;
    if (filePath === './') {
        filePath = './index.html';
    }
    
    const extname = String(path.extname(filePath)).toLowerCase();
    const contentType = mimeTypes[extname] || 'application/octet-stream';
    
    fs.readFile(filePath, (error, content) => {
        if (error) {
            if (error.code === 'ENOENT') {
                res.writeHead(404, { 'Content-Type': 'text/html' });
                res.end('<h1>404 Not Found</h1>', 'utf-8');
            } else {
                res.writeHead(500);
                res.end(`Server Error: ${error.code}`, 'utf-8');
            }
        } else {
            res.writeHead(200, { 'Content-Type': contentType });
            res.end(content, 'utf-8');
        }
    });
});

server.listen(PORT, () => {
    console.log('╔═══════════════════════════════════════════════════════════════╗');
    console.log('║  Pothole Detection System - Mock Server                      ║');
    console.log('╚═══════════════════════════════════════════════════════════════╝');
    console.log('');
    console.log(`  Server running at http://localhost:${PORT}/`);
    console.log('');
    console.log('  API Endpoints:');
    console.log(`    GET  http://localhost:${PORT}/api/status`);
    console.log(`    GET  http://localhost:${PORT}/api/sensors`);
    console.log('');
    console.log('  Note: BLE functionality requires physical hardware');
    console.log('  Sensor states will randomly change for testing purposes');
    console.log('');
    console.log('  Press Ctrl+C to stop the server');
    console.log('');
});
