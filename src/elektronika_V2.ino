#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);

const int currentSensorPin = A0;
const int voltageSensorPin = A3;

const float sensitivity = 66.0;
const float offsetVoltage = 2500.0;
const float voltageDividerRatio = 72.5; 

unsigned long lastTime = millis();
float energyConsumed = 0.0;

void setup() {
    Serial.begin(9600);
    lcd.init();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print(" SMART ENERGY METER ");
    delay(2000);
    lcd.clear();
}

void loop() {
    float Vrms = measureVoltageRMS();
    float Irms = measureCurrentRMS();

    Serial.print("Vrms: "); Serial.println(Vrms);
    Serial.print("Irms: "); Serial.println(Irms);

    float power = Vrms * Irms * 0.9;
    updateEnergy(power);
    
    displayReadings(Vrms, Irms, power, energyConsumed);

    delay(1);
}

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
        return 0.0;
    }
    return voltageRMS+1.4;
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
    float rmsCurrent = sqrt(currentSum / sampleCount)-0.3;

    return (rmsCurrent < 0) ? 0 : rmsCurrent;
}

void updateEnergy(float power) {
    unsigned long currentTime = millis();
    float elapsedTime = (currentTime - lastTime) / 1000.0;
    if (elapsedTime > 0) {
        energyConsumed += (power * elapsedTime) / 3600.0 / 1000.0;
    }
    lastTime = currentTime;
}

void displayReadings(float Vrms, float Irms, float power, float energy) {
    lcd.setCursor(0, 0);
    lcd.print("V="); lcd.print(Vrms, 2); lcd.print(" V   ");  

    lcd.setCursor(0, 1);
    lcd.print("I="); lcd.print(Irms, 3); lcd.print(" A   ");

    lcd.setCursor(0, 2);
    lcd.print("P="); lcd.print(power, 2); lcd.print(" W   ");

    lcd.setCursor(0, 3);
    lcd.print("E="); lcd.print(energy, 3); lcd.print(" kWh  ");
}