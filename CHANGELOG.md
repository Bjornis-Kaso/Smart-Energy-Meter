# Changelog

All notable changes to the Smart Energy Meter project are documented here — firmware, hardware, documentation, and repository.

---

## [Repository] - April 2025

### Added
- `README.md` — full project overview with hardware table, wiring, specs, schematics, and demo video links
- `CHANGELOG.md` — this file
- `.gitignore` — ignores Arduino build artifacts and OS files
- `docs/project_report.md` — full 13-section project report in English covering design, firmware, calibration, testing, analysis, and conclusions
- `calibration/calibration_sheet.md` — full calibration table with legend and instrument specifications
- `calibration/data.csv` — raw calibration measurement data (10 points, V/A/P)
- `media/schematics/` — 7 Proteus simulation screenshots across V1 and V2
- `media/hardware/` — 7 real hardware photos (Arduino, LCD, transformer, perfboard, live readings)
- `media/demo/` — Google Drive folder linked in README with 5 demo videos

---

## [V2] - March 2025 (elektronika_V2.ino) — Production

### Added
- I²C LCD support (`LiquidCrystal_I2C` library, address `0x27`)
- Power factor correction applied to power calculation (`× 0.9`)
- Voltage threshold filter: returns `0 V` if RMS < 180 V to suppress noise
- Voltage offset correction: `+1.4 V` applied to RMS result after calibration
- Current offset correction: `−0.3 A` subtracted from RMS current result
- Reduced `delay()` in main loop to `1 ms` for faster refresh

### Changed
- Voltage divider ratio updated from `80.0` → `72.5` (hardware recalibration)
- Current sensor changed to ACS712 30A module (sensitivity: `66 mV/A`)
- Current RMS: result clamped to `0` if negative after correction
- LCD uses `setCursor` without `lcd.clear()` each loop (eliminates flicker)
- Sample count increased: voltage `2000`, current `2500`

### Removed
- `delayMicroseconds` between ADC samples (replaced by tight loop at 1 ms total delay)
- Parallel LCD wiring (6 pins → 2 pins via I²C)

---

## [Calibration] - March 2025

### Completed
- Formal calibration using comparison method against reference instrument
- 10 measurement points across 224 V – 232 V range
- Maximum power error: **1.73%** (relative), **1.70%** (referenced)
- Maximum voltage error: **0.44%** (relative)
- Maximum current error: **1.7%** (relative)
- Accuracy class confirmed: **Class 1–2 per IEC 62053-21**
- Calibration offsets applied to V2 firmware: `+1.4 V` voltage, `−0.3 A` current

---

## [Simulimi] - Early 2025 (Simulimi_kodi.ino) — Simulation

### Notes
- Full circuit simulated in Proteus before V2 hardware construction
- Uses parallel LCD (`LiquidCrystal`, pins 2–7) for simulator compatibility
- Voltage divider ratio: `80.0` (simulation model)
- No power factor correction
- `delay(100)` in loop
- RMS current: no offset correction applied
- Bridge rectifier (BR1) added to voltage sensing path in later simulation revisions
- I²C LCD variant simulated separately (`I2C-20X4` component)
- Intended for circuit simulation only — not for hardware deployment

---

## [V1] - December 2024 (V1.ino) — Initial Prototype

### Notes
- First working prototype built and tested on perfboard
- Parallel LCD (`LiquidCrystal`, pins 2–7)
- Voltage divider ratio: `561.0` (earlier hardware version)
- Current sensor: ACS712 20A module, sensitivity `180 mV/A`, offset `500 mV`
- `delayMicroseconds(200)` between voltage samples
- `delayMicroseconds(100)` between current samples
- `delay(1000)` in main loop (1-second refresh)
- No power factor correction
- No calibration offsets
- Tested with hairdryer (2000W), soldering iron, and standby loads
