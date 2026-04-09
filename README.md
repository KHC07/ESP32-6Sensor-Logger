# ESP32 6-Sensor Light Measurement System

## Overview

This project uses an ESP32 to measure and analyze light using:

* 3 × AS7341 spectral sensors (via TCA9548A I2C multiplexer)
* 3 × KY-018 photoresistors (analog light sensors)

The system calculates:

* Average light level (lux approximation)
* Average blue light fraction
* Average spectral reading
* Raw blue light values from each spectral sensor

Data is output in a **space-separated format** for real-time plotting and analysis.

---

## Features

* Multi-sensor data acquisition
* I2C multiplexing using TCA9548A
* Bluetooth control (start/stop measurements)
* Real-time serial output for plotting
* Portable (battery-powered compatible)

---

## Bill of Materials (BOM)

| Quantity | Component                    |
| -------- | ---------------------------- |
| 1        | ESP32 Development Board      |
| 3        | AS7341 Spectral Sensors      |
| 3        | KY-018 Photoresistor Modules |
| 1        | TCA9548A I2C Multiplexer     |
| 1        | Battery Pack                 |
| ~20      | Male-to-Female Jumper Wires  |

---

## Wiring

### ESP32 ↔ TCA9548A

| TCA9548A Pin | ESP32 Pin |
| ------------ | --------- |
| SDA          | GPIO 21   |
| SCL          | GPIO 22   |
| VCC          | 3.3V      |
| GND          | GND       |

---

### AS7341 Sensors (via TCA9548A)

| Sensor   | Channel   |
| -------- | --------- |
| Sensor 1 | Channel 0 |
| Sensor 2 | Channel 1 |
| Sensor 3 | Channel 2 |

---

### KY-018 Sensors

| Sensor | ESP32 Pin |
| ------ | --------- |
| LDR 1  | GPIO 34   |
| LDR 2  | GPIO 35   |
| LDR 3  | GPIO 32   |

---

## Software Requirements

* Arduino IDE
* ESP32 Board Package

Libraries:

* Adafruit AS7341 (v1.1.3)
* Adafruit BusIO (v1.14.8)
* BluetoothSerial (built-in)

---

## Installation

1. Open Arduino IDE
2. Install required libraries
3. Open `Lab24_Lights.ino`
4. Select ESP32 board
5. Upload code to ESP32

---

## Usage

### 1. Power the System

* Connect via USB or battery

### 2. Connect via Bluetooth

* Use **Serial Bluetooth Terminal (Kai Morich)**
* Connect to: `ESP32_Sensor`

### 3. Control Logging

Send:

```
start
```

→ Begin measurements

Send:

```
stop
```

→ Stop measurements

---

## Output Format

Data is printed in a **single line**:

```
AvgLux AvgBlueFraction AvgSpectral RawBlue1 RawBlue2 RawBlue3
```

### Example:

```
1.24 0.35 52.1 120 130 125
```

| Value           | Description                             |
| --------------- | --------------------------------------- |
| AvgLux          | Average light level from KY-018 sensors |
| AvgBlueFraction | Blue light proportion across sensors    |
| AvgSpectral     | Average spectral intensity              |
| RawBlue1–3      | Blue channel readings from each AS7341  |

---

## Testing

### Test 1: Power Test

* ESP32 powers on without errors

### Test 2: Sensor Response

* Light changes affect KY-018 readings

### Test 3: Spectral Sensors

* AS7341 values change with light conditions

---

## System Architecture

* ESP32 reads:

  * Analog signals (KY-018)
  * I2C spectral data (AS7341 via TCA9548A)
* Processes averages
* Outputs formatted data via:

  * Serial Monitor
  * Bluetooth

---

## Future Improvements

* Add SD card data logging
* Improve lux calibration
* Add enclosure for outdoor deployment
* Reduce power consumption for overnight use

---

## Author

Your Name
Course / Lab: Lab 24 – Light Measurement System
Date: 2026
