# Changelog

All notable changes to the Smart Energy Meter firmware are documented here.

---

## [V2] - 2025 (elektronika_V2.ino) — Production

### Added
- I²C LCD support (`LiquidCrystal_I2C` library, address `0x27`)
- Power factor correction applied to power calculation (`× 0.9`)
- Voltage threshold filter: returns `0 V` if RMS < 180 V to suppress noise
- Voltage offset correction: `+1.4 V` applied to RMS result after calibration
- Current offset correction: `−0.3 A` subtracted from RMS current result
- Reduced `delay()` in main loop to `1 ms` for faster refresh

### Changed
- Voltage divider ratio updated from `80.0` → `72.5` (hardware recalibration)
- Current RMS: result clamped to 0 if negative after correction
- LCD uses `setCursor` without `lcd.clear()` each loop (reduces flicker)

### Removed
- `delayMicroseconds` between ADC samples (replaced by tight loop at 1 ms total delay)

---

## [Simulimi] - 2025 (Simulimi_kodi.ino) — Simulation

### Notes
- Uses parallel LCD (`LiquidCrystal`, pins 2–7) for Tinkercad/simulator compatibility
- Voltage divider ratio: `80.0` (simulation model)
- No power factor correction
- `delay(100)` in loop
- RMS current: no offset correction applied
- Intended for circuit simulation only — not for hardware deployment

---

## [V1] - 2024 (V1.ino) — Initial Prototype

### Notes
- First working prototype
- Parallel LCD (`LiquidCrystal`, pins 2–7)
- Voltage divider ratio: `561.0` (earlier hardware version)
- Current sensor sensitivity: `180 mV/A` (earlier ACS712 module)
- `delayMicroseconds(200)` between voltage samples, `delayMicroseconds(100)` between current samples
- `delay(1000)` in main loop (1-second refresh)
- No power factor correction
- No calibration offsets
