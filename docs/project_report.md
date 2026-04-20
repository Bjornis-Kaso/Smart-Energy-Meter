# Project Report — Smart Energy Meter (Matës Smart i Energjisë)

**Project Title:** Smart Energy Meter  
**Academic Year:** 2024–2025  
**School:** Instituti Harry Fultz, Tiranë, Albania  
**Department:** Elektronikë  
**Class:** 4-7  
**Student:** Bjornis Kaso  
**Supervisor:** MSc. Jurgen Metalla  
**Subject 1:** Elektronikë  
**Subject 2:** Matje Elektronike  

---

## Table of Contents

1. [Keywords](#1-keywords)
2. [References](#2-references)
3. [Abstract](#3-abstract)
4. [Introduction](#4-introduction)
5. [Components and Materials](#5-components-and-materials)
6. [Circuit Design](#6-circuit-design)
7. [Firmware Development](#7-firmware-development)
8. [Calibration](#8-calibration)
9. [Testing and Results](#9-testing-and-results)
10. [Analysis](#10-analysis)
11. [Future Improvements](#11-future-improvements)
12. [Conclusions](#12-conclusions)
13. [Self-Assessment](#13-self-assessment)

---

## 1. Keywords

`Arduino Uno` · `Smart Energy Meter` · `ACS712` · `RMS Measurement` · `Voltage Divider` · `Active Power` · `Energy Consumption` · `I²C LCD` · `Proteus Simulation` · `IEC 62053-21` · `Calibration` · `Hall-effect sensor`

---

## 2. References

1. **Arduino Documentation** — *Arduino Uno Reference*, arduino.cc/en/reference
2. **Allegro MicroSystems** — *ACS712 Datasheet: Fully Integrated, Hall Effect-Based Linear Current Sensor*, allegromicro.com
3. **International Electrotechnical Commission** — *IEC 62053-21: Electricity metering equipment – Particular requirements – Part 21: Static meters for AC active energy*, iec.ch
4. **International Electrotechnical Commission** — *IEC 61010-1: Safety requirements for electrical equipment for measurement, control and laboratory use*, iec.ch
5. **International Electrotechnical Commission** — *IEC 60529: Degrees of protection provided by enclosures (IP Code)*, iec.ch
6. **Albanian Government** — *Law No. 124/2015 on Energy Efficiency*, gov.al
7. **Frank de Brabander** — *LiquidCrystal_I2C Library*, GitHub, github.com/johnrickman/LiquidCrystal_I2C

---

## 3. Abstract

This project presents the design, simulation, construction, and calibration of a low-cost Arduino-based smart energy meter capable of measuring AC voltage, current, active power, and cumulative energy consumption in real time. The device uses an ACS712 Hall-effect current sensor and a resistive voltage divider to interface safely with mains-level AC signals, processing them through an Arduino Uno microcontroller using RMS oversampling algorithms. Readings are displayed on a 20×4 I²C LCD. The system was simulated in Proteus before physical construction, and calibrated using the comparison method against a reference instrument across 10 measurement points in the 224–232 V range. Calibration results showed a maximum power error of 1.73%, placing the device in accuracy Class 1–2 per IEC 62053-21. The project demonstrates that accurate, functional energy metering hardware can be built at low cost using widely available components, making it suitable for educational, residential monitoring, and prototyping applications.

---

## 4. Introduction

### 4.1 Background and Motivation

Energy consumption monitoring has become increasingly important in modern life. With rising electricity costs and growing awareness of energy efficiency, individuals and businesses alike benefit from understanding exactly how much power their devices consume. Commercial energy meters are accurate but expensive and offer little flexibility for customization or learning.

This project was motivated by the desire to build a functional smart energy meter from scratch — one that could measure real AC mains parameters, display them in real time, and be understood at every level of its design. The project combines knowledge from electronics, measurement, and embedded programming into a single working instrument.

### 4.2 Project Goals

The main goals of this project were:

- To design and simulate a complete energy metering circuit in Proteus
- To construct a working physical prototype using an Arduino Uno
- To implement RMS voltage and current measurement algorithms in firmware
- To calculate active power with power factor correction
- To track cumulative energy consumption in kWh
- To calibrate the device against a reference instrument and document the results
- To verify compliance with relevant IEC standards

### 4.3 Scope

The device is designed for single-phase AC mains (230 V, 50 Hz) with a current range of 0–30 A, covering typical residential and light commercial loads. The project covers hardware design, firmware development across multiple versions, simulation, physical construction, and formal calibration.

---

## 5. Components and Materials

### 5.1 Main Components

| Component | Model / Specification | Role |
|-----------|----------------------|------|
| Microcontroller | Arduino Uno (ATmega328P) | Main processing unit |
| Current sensor | ACS712ELCTR-30A-T | AC current measurement |
| Display | 20×4 I²C LCD (address 0x27) | Real-time display |
| Transformer | Step-down, 230V → low voltage | Voltage sensing reference |
| Rectifier diode | 1N4007 | Half-wave rectification |
| Zener diode | 1N4733A (5.1V) | Voltage clamping/protection |
| Resistors | 100kΩ, 220kΩ (×2), 1kΩ, 5kΩ, 10kΩ | Voltage divider network |
| Capacitor | 10nF | Noise filtering |
| Potentiometer | 1kΩ, 10kΩ | LCD contrast and calibration |
| Perfboard | Copper clad | Circuit assembly |

### 5.2 The ACS712 Current Sensor

The ACS712 is a Hall-effect based linear current sensor from Allegro MicroSystems. The 30A variant (used in this project) has a sensitivity of 66 mV/A with an offset voltage of 2500 mV at zero current. It provides galvanic isolation between the mains current path and the Arduino's analog input, making it safe and suitable for this application.

### 5.3 Voltage Measurement

Mains voltage (230 V AC) cannot be connected directly to the Arduino's ADC, which operates at 0–5 V. A resistive voltage divider network steps the voltage down to a safe level. In the final V2 design, a voltage divider ratio of 72.5× was used after calibration adjustment, with a 1N4007 diode for rectification and a 1N4733A Zener for protection against overvoltage spikes.

---

## 6. Circuit Design

### 6.1 Design Evolution

The circuit went through several design iterations:

**V1 (Initial Prototype)**
The first version used a simple resistive voltage divider with a ratio of 561× and a parallel-interface LCD (pins 2–7). The ACS712 sensitivity was set to 180 mV/A (20A module). While functional, this version had significant measurement errors due to uncalibrated constants and a slow 1-second display refresh.

**Simulation Version**
Before advancing to V2 hardware, the circuit was redesigned and simulated in Proteus. The simulation used a VSINE source at 220–230 V, a transformer model (TRAN-2P2S), the ACS712 model, and a 20×4 LCD. This confirmed the signal conditioning approach and allowed firmware testing without risk to hardware.

**V2 (Production Version)**
The final version switched to an I²C LCD (reducing wiring from 6 pins to 2), updated the voltage divider ratio to 72.5× based on calibration, changed the ACS712 to the 30A module (66 mV/A sensitivity), added a power factor correction of 0.9, and applied measured offset corrections to both voltage (+1.4 V) and current (−0.3 A) readings.

### 6.2 Signal Conditioning

The voltage measurement path is: mains AC → transformer → voltage divider (R1/R2/R3/R4/R5) → 1N4007 rectifier → 1N4733A Zener clamp → Arduino A3.

The current measurement path is: mains AC through ACS712 → VIOUT pin → Arduino A0.

### 6.3 Proteus Simulation

The full circuit was simulated in Proteus before construction. The simulation verified:
- Correct LCD output (V, I, P, E displayed on all 4 rows)
- Voltage readings tracking the VSINE source correctly
- Current readings responding to load changes
- Power calculation updating in real time

Simulation screenshots are available in `media/schematics/`.

---

## 7. Firmware Development

### 7.1 Overview

The firmware is written in C++ for the Arduino platform. It performs three main tasks in a continuous loop: measure RMS voltage, measure RMS current, and update the display and energy accumulator.

### 7.2 RMS Measurement Algorithm

True RMS measurement requires sampling the AC waveform many times per cycle and computing the root mean square. For a 50 Hz signal, the Arduino samples at a much higher rate using tight ADC loops.

**Voltage RMS (V2 implementation):**
```cpp
float measureVoltageRMS() {
    float voltageSum = 0;
    const int sampleCount = 2000;

    for (int i = 0; i < sampleCount; i++) {
        int adcValue = analogRead(voltageSensorPin);
        float voltage = (adcValue / 1023.0) * 5.0;
        voltageSum += voltage * voltage;
    }

    float voltageRMS = sqrt(voltageSum / sampleCount);
    voltageRMS *= voltageDividerRatio;  // 72.5

    if (voltageRMS < 180) return 0.0;   // noise threshold
    return voltageRMS + 1.4;            // calibration offset
}
```

**Current RMS (V2 implementation):**
```cpp
float measureCurrentRMS() {
    float currentSum = 0;
    const int sampleCount = 2500;

    for (int i = 0; i < sampleCount; i++) {
        int adcValue = analogRead(currentSensorPin);
        float voltage = (adcValue / 1023.0) * 5000;
        float current = (voltage - offsetVoltage) / sensitivity; // 2500mV offset, 66mV/A
        currentSum += current * current;
    }

    float rmsCurrent = sqrt(currentSum / sampleCount) - 0.3; // calibration offset
    return (rmsCurrent < 0) ? 0 : rmsCurrent;
}
```

### 7.3 Power and Energy Calculation

Active power is calculated as:

```
P = Vrms × Irms × PF
```

where PF = 0.9 (assumed power factor for typical resistive/mixed loads).

Energy is accumulated over time:

```cpp
void updateEnergy(float power) {
    unsigned long currentTime = millis();
    float elapsedTime = (currentTime - lastTime) / 1000.0; // seconds
    energyConsumed += (power * elapsedTime) / 3600.0 / 1000.0; // kWh
    lastTime = currentTime;
}
```

### 7.4 Display

The 20×4 I²C LCD shows all four measurements simultaneously:

```
V=228.13 V
I=0.510 A
P=104.57 W
E=0.001 kWh
```

The display is updated every loop iteration (approximately every 1 ms) without calling `lcd.clear()`, which eliminates flicker.

### 7.5 Firmware Versions

| Version | File | Key Changes |
|---------|------|-------------|
| V1 | `src/V1.ino` | First prototype, parallel LCD, 1s refresh |
| Simulation | `simulation/Simulimi_kodi.ino` | Parallel LCD, for Proteus only |
| V2 | `src/elektronika_V2.ino` | I²C LCD, calibration offsets, power factor |

---

## 8. Calibration

### 8.1 Method

Calibration was performed using the **comparison method** (Metoda e kalibrimit me krahasim), in which the device under test (Mx) is measured simultaneously with a calibrated reference instrument (Mo) and the errors are computed.

### 8.2 Procedure

Ten measurements were taken across the operational voltage range (224 V – 232 V) with a constant resistive load drawing approximately 0.51–0.53 A. For each measurement point, voltage, current, and power were recorded from both instruments.

### 8.3 Error Calculations

For each measurement point, the following were calculated:

- **Absolute error:** Ga = Mx − Mo
- **Correction:** θ = −Ga
- **Relative error:** Grl (%) = (Ga / Mo) × 100
- **Referenced error:** Grf (%) = (Ga / Full-scale) × 100

### 8.4 Results Summary

| Quantity | Max Relative Error | Max Referenced Error |
|----------|-------------------|---------------------|
| Voltage | 0.44% | 0.29% |
| Current | 1.7% | 1.2% |
| Power | 1.73% | 1.70% |

Full calibration data is in `calibration/calibration_sheet.md` and `calibration/data.csv`.

### 8.5 Accuracy Class

Based on the calibration results, the device falls within **Class 1–2** per **IEC 62053-21**, which defines accuracy requirements for static active energy meters. The maximum power error of 1.73% is within the Class 2 limit of ±2%.

---

## 9. Testing and Results

### 9.1 Test Loads

The device was tested with several real loads:

| Load | Rated Power | Measured V | Measured I | Measured P |
|------|------------|------------|------------|------------|
| Hairdryer (low mode) | ~1000 W | ~228 V | ~4.4 A | ~1007 W |
| Hairdryer (high mode) | 2000 W | ~228 V | ~8.8 A | ~1803 W |
| Soldering iron | ~48 W | ~217 V | ~0.22 A | ~43 W |
| Standby (no load) | 0 W | ~217 V | 0.035 A | ~7.6 W |

### 9.2 Observations

- The device responds correctly to load changes, updating readings in under 100 ms
- At low loads (< 0.1 A), the current reading shows slight noise (~0.03 A residual)
- The voltage reading is stable and consistent across the mains range (217–232 V)
- Energy accumulation is continuous and correct over time
- The I²C LCD shows no flicker and is clearly readable

### 9.3 Comparison with Reference

During formal calibration, the device showed consistent positive power bias (readings slightly higher than reference), which is likely due to the fixed power factor of 0.9 not perfectly matching the actual load power factor. This is addressed in future improvements.

---

## 10. Analysis

### 10.1 Measurement Uncertainty Sources

The main sources of error in this device are:

1. **ADC resolution** — The Arduino's 10-bit ADC (1024 steps over 5V) introduces a quantization error of approximately 4.9 mV per step, which propagates through the voltage divider calculation
2. **ACS712 offset drift** — The zero-current offset voltage (2500 mV) can drift with temperature, causing a small current reading even at zero load
3. **Fixed power factor** — Using PF = 0.9 for all loads introduces systematic error for purely resistive loads (PF = 1.0) or highly inductive loads
4. **Sampling window** — The RMS calculation uses 2000–2500 samples per measurement. At 50 Hz, this covers approximately 2–3 full AC cycles, which is sufficient but not ideal for highly distorted waveforms

### 10.2 Strengths

- Low cost (total component cost under €15)
- Safe isolation between mains and microcontroller via ACS712 and transformer
- Accurate enough for residential monitoring (< 2% error)
- Compact and readable display
- Serial output for further data logging

### 10.3 Limitations

- Single phase only
- No wireless connectivity or data logging
- Fixed power factor assumption
- No protection against sustained overvoltage on voltage divider

---

## 11. Future Improvements

1. **Automatic power factor measurement** — Instead of assuming PF = 0.9, measure the phase angle between voltage and current waveforms to compute true power factor dynamically

2. **Wi-Fi connectivity** — Add an ESP8266 or ESP32 module to transmit readings to a smartphone app or web dashboard for remote monitoring

3. **Data logging** — Add an SD card module to record timestamped energy data for historical analysis

4. **Improved current sensing** — Replace the ACS712 with a higher-precision current transformer (CT sensor) for lower noise at small currents

5. **PCB design** — Design a proper PCB to replace the perfboard construction, reducing noise and improving reliability

6. **Multi-phase support** — Extend the design to 3-phase measurements for industrial applications

7. **Enclosure** — Design and 3D print a proper enclosure with clear safety labeling and DIN rail mounting

8. **Harmonic analysis** — Use FFT on the sampled waveform to detect and display total harmonic distortion (THD)

---

## 12. Conclusions

This project successfully demonstrated the design, simulation, construction, and calibration of a functional smart energy meter based on the Arduino Uno platform. Starting from a basic prototype (V1), the design evolved through Proteus simulation to a calibrated production version (V2) capable of measuring AC voltage, current, active power, and cumulative energy with errors below 1.73%.

The calibration process confirmed that the device meets **IEC 62053-21 Class 2** accuracy requirements, making it suitable for residential energy monitoring applications. Testing with real loads including a hairdryer and soldering iron confirmed correct and responsive behavior across a wide power range.

The project provided hands-on experience with analog signal conditioning for mains-level AC, RMS digital signal processing, I²C communication, formal instrument calibration, and technical documentation — all of which are directly applicable to professional electronics and measurement engineering work.

---

## 13. Self-Assessment

This project was challenging in several ways. Interfacing safely with 230 V mains required careful attention to isolation and protection — mistakes here could damage equipment or cause injury. Getting accurate RMS readings from the Arduino's ADC required understanding of sampling theory and careful tuning of the divider ratio and firmware offsets.

The calibration process was particularly valuable. Comparing the device against a reference instrument and formally computing errors made the accuracy limitations concrete and measurable, rather than just theoretical.

If I were to do this project again, I would start with PCB design from the beginning rather than perfboard, and I would add wireless connectivity earlier in the development process. I would also implement dynamic power factor measurement rather than using a fixed value.

Overall I am satisfied with the result — a working, calibrated energy meter built from scratch that meets international accuracy standards.

---

*Report prepared by Bjornis Kaso — Instituti Harry Fultz, Klasa 4-7, 2024–2025*  
*Supervisor: MSc. Jurgen Metalla*
