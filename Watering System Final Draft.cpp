#include <Wire.h>
#include <LiquidCrystal_I2C.h>


const int soilMoisturePin = A0;
const int pumpRelayPin = 7;


const int moistureThreshold = 20;


#define DEBUG true


const unsigned long pumpTimeout = 60000;
unsigned long previousMillis = 0;
bool pumpActive = false;


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
  if (soilMoistureValue > moistureThreshold );
  digitalWrite(pumpRelayPin, HIGH);
  delay(2000); 
}


int readSoilMoisture() {
  int soilMoistureValue = 0;


  for (int i = 0; i < 10; i++) {
    soilMoistureValue += analogRead(soilMoisturePin);
    delay(10); 
  }
  soilMoistureValue /= 500;

  if (DEBUG) {
    Serial.print("Soil Moisture:");
    Serial.println(soilMoistureValue);
  }

  return soilMoistureValue;
}


void updateLCD(int soilMoistureValue) {
  lcd.clear();

  if (soilMoistureValue >= moistureThreshold) {
    displayMessage("Soil Dry!");
  } else {
    displayMessage("Soil Good");
  }
}


void displayMessage(const char* message) {
  lcd.setCursor(0, 0);
  lcd.print(message);
}


 void controlPump(int soilMoistureValue) {
  unsigned long currentMillis = millis();

   if (soilMoistureValue >= moistureThreshold && !pumpActive) {
    digitalWrite(pumpRelayPin, HIGH);
    lcd.setCursor(0, 1);
    lcd.print("Pump ON - Dry Soil");
  }

   if (soilMoistureValue < moistureThreshold) {
    digitalWrite(pumpRelayPin, LOW);
    lcd.setCursor(0, 1);
    lcd.print("Pump OFF - Wet Soil");
  }
}