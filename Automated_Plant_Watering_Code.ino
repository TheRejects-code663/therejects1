#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#define DEBUG true

const int soilMoisturePin = A0;   
const int pumpRelayPin = 5;        
const int moistureThreshold = 20;  
const unsigned long pumpTimeout = 1000;                            

LiquidCrystal_I2C lcd(0x27, 16, 2);      

void setup() {
  pinMode(pumpRelayPin, OUTPUT);
  digitalWrite(pumpRelayPin, LOW); 
  Serial.begin(9600);

  lcd.init();
  lcd.backlight();
  displayMessage("System loading...");

  delay(3000);
  lcd.clear();
}

void loop() {
  int soilMoistureValue = readSoilMoisture();
  updateLCD(soilMoistureValue);
  controlPump(soilMoistureValue); 
  delay(2000); 
}

int readSoilMoisture() {
  int soilMoistureValue = 0;

  for (int i = 0; i < 10; i++) {
    soilMoistureValue += analogRead(soilMoisturePin);
    delay(10); 
  }
  soilMoistureValue /= 10; 
  if (DEBUG) {
    Serial.print("Soil Moisture: ");
    Serial.println(soilMoistureValue);
  }
  
  return soilMoistureValue;
}

void updateLCD(int soilMoistureValue) {
  lcd.clear();
  lcd.setCursor(0, 0);

  if (soilMoistureValue >= moistureThreshold) {
    lcd.print("Soil Dry!");
  } else {
    lcd.print("Soil Good");
  }
}

void displayMessage(const char* message) {
  lcd.setCursor(0, 0);
  lcd.print(message);
}

void controlPump(int soilMoistureValue) {
  if (soilMoistureValue >= moistureThreshold) {
    digitalWrite(pumpRelayPin, HIGH); 
    lcd.setCursor(0, 1);
    lcd.print("Pump ON - Dry Soil");
  } else {
    digitalWrite(pumpRelayPin, LOW); 
    lcd.setCursor(0, 1);
    lcd.print("Pump OFF - Wet Soil");
  }
}
