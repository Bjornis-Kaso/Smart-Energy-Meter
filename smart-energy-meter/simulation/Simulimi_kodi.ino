#include <LiquidCrystal.h> // Library for LCD

// LCD Pins
LiquidCrystal lcd(2, 3, 4, 5, 6, 7);

const int currentSensorPin = A0;
const int voltageSensorPin = A3;

const float sensitivity = 66.0;
const float offsetVoltage = 2500.0;
const float voltageDividerRatio = 80.0; 

unsigned long lastTime = millis();
float energyConsumed = 0.0;

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
    delay(100);
}

// Function to measure RMS voltage
float measureVoltageRMS() {
    float voltageSum = 0;
    const int sampleCount = 2000;

    for (int i = 0; i < sampleCount; i++) {
        int adcValue = analogRead(voltageSensorPin);
        float voltage = (adcValue / 1023.0) * 5.0;
        voltageSum += voltage * voltage;
    }

    float voltageRMS = sqrt(voltageSum / sampleCount);
    voltageRMS *= voltageDividerRatio;

    if (voltageRMS < 180) {
        return 0.0;  // Return 0V if below threshold
    }
    return voltageRMS;
}

float measureCurrentRMS() {
    float currentSum = 0;
    const int sampleCount = 2500;

    for (int i = 0; i < sampleCount; i++) {
        int adcValue = analogRead(currentSensorPin);
        float voltage = (adcValue / 1023.0) * 5000;
        float current = (voltage - offsetVoltage) / sensitivity;
        
        currentSum += current * current;
    }

    return sqrt(currentSum/sampleCount);
}


void updateEnergy(float power) {
    unsigned long currentTime = millis();
    float elapsedTime = (currentTime - lastTime) / 1000.0;
    if (elapsedTime > 0) {
        energyConsumed += (power * elapsedTime) / 3600.0 / 1000.0;
    }
    lastTime = currentTime;
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
