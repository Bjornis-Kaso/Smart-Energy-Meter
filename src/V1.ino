#include <LiquidCrystal.h> // Library for LCD

// LCD Pins
LiquidCrystal lcd(2, 3, 4, 5, 6, 7);

// Constants for Current Sensor (ACS712)
const int currentSensorPin = A0;  // Analog pin connected to ACS712
const float sensitivity = 180.0;  // mV/A (100 for 20A module, 66 for 30A module)
const float offsetVoltage = 500.0; // Offset voltage in mV

// Constants for Voltage Measurement
const int voltageSensorPin = A3;  // Analog pin for voltage measurement
const float voltageDividerRatio = 561.0; // Voltage divider ratio (updated based on your circuit)

// Time and Energy Tracking
unsigned long lastTime = 0; // Last recorded time
float energyConsumed = 0;   // Energy in kWh

void setup() {
    // Initialize serial communication
    Serial.begin(9600);

    // Initialize LCD
    lcd.begin(20, 4);
    lcd.setCursor(0, 0);
    lcd.print("SMART ENERGY METER");
    delay(2000); // Display welcome message for 2 seconds
    lcd.clear();
}

void loop() {
    // Get RMS values for voltage and current
    float Vrms = measureVoltageRMS();
    float Irms = measureCurrentRMS();

    // Calculate power and energy
    float power = Vrms * Irms; // Power in Watts
    updateEnergy(power);

    // Display readings
    displayReadings(Vrms, Irms, power, energyConsumed);

    // Short delay for stability
    delay(1000);
}

// Function to measure RMS voltage
float measureVoltageRMS() {
    float voltageSum = 0;
    const int sampleCount = 2500; // Number of samples for RMS calculation

    for (int i = 0; i < sampleCount; i++) {
        int adcValue = analogRead(voltageSensorPin); // Read ADC value
        float voltage = (adcValue / 1023.0) * 5000; // Convert to mV
        voltageSum += voltage * voltage + 2;           // Sum of squares
        delayMicroseconds(200); // Small delay for sampling
    }

    float voltageRMS = sqrt(voltageSum / sampleCount); // RMS calculation
    return voltageRMS * voltageDividerRatio / 1000.0 *2.23;  // Convert to Volts
}

// Function to measure RMS current
float measureCurrentRMS() {
    float currentSum = 0;
    const int sampleCount = 1000; // Number of samples for RMS calculation

    for (int i = 0; i < sampleCount; i++) {
        int adcValue = analogRead(currentSensorPin); // Read ADC value
        float voltage = (adcValue / 1023.0) * 5000; // Convert to mV
        float current = (voltage - offsetVoltage) / sensitivity; // Convert to Amps
        currentSum += current * current;           // Sum of squares
        delayMicroseconds(100); // Small delay for sampling
    }

    return sqrt(currentSum / sampleCount); // RMS calculation
}

// Function to update energy consumption
void updateEnergy(float power) {
    unsigned long currentTime = millis();
    float elapsedTime = (currentTime - lastTime) / 1000.0; // Time in seconds
    energyConsumed += (power * elapsedTime) / 3600.0 / 1000.0; // Energy in kWh
    lastTime = currentTime; // Update last time
}

// Function to display readings on LCD
void displayReadings(float Vrms, float Irms, float power, float energy) {
    lcd.clear();

    // Display Voltage
    lcd.setCursor(0, 0);
    lcd.print("V=");
    lcd.print(Vrms, 2);
    lcd.print(" V");

    // Display Current
    lcd.setCursor(0, 1);
    lcd.print("I=");
    lcd.print(Irms, 3);
    lcd.print(" A");

    // Display Power
    lcd.setCursor(0, 2);
    lcd.print("P=");
    lcd.print(power, 2);
    lcd.print(" W");

    // Display Energy
    lcd.setCursor(0, 3);
    lcd.print("E=");
    lcd.print(energy, 3);
    lcd.print(" kWh");
}
