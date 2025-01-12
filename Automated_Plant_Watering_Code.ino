#include <Wire.h>
#include <LiquidCrystal_I2C.h>


const int soilMoisturePin = A0;    
const int pumpRelayPin = 5;        
const int moistureThreshold = 20; 
#define DEBUG true
const unsigned long pumpTimeout = 10; 
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
    controlPump(soilMoistureValue);           

   
    unsigned long currentMillis = millis();
    if (pumpActive && (currentMillis - previousMillis >= pumpTimeout)) {
        digitalWrite(pumpRelayPin, LOW); 
        pumpActive = false;             
        lcd.setCursor(0, 1);
        lcd.print("Pump OFF - Timeout");
        Serial.println("Pump OFF due to timeout.");
    }
}

int readSoilMoisture() {
    int soilMoistureValue = 0;

    for (int i = 0; i < 10; i++) {
        soilMoistureValue += analogRead(soilMoisturePin);
        delay(10); 
    }
    soilMoistureValue /= 10; 

    if (DEBUG) {
        Serial.print("Soil Moisture Value: ");
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

    if (DEBUG) {
        Serial.print("LCD Update - Soil Moisture: ");
        Serial.println(soilMoistureValue);
    }
}

void displayMessage(const char* message) {
    lcd.setCursor(0, 0); 
    lcd.print(message);  
}

void controlPump(int soilMoistureValue) {
    if (soilMoistureValue >= moistureThreshold && !pumpActive) {
        digitalWrite(pumpRelayPin, HIGH); 
        pumpActive = true;              
        previousMillis = millis();      
        lcd.setCursor(0, 1);
        lcd.print("Pump ON - Dry soil");
        Serial.println("Pump ON - Soil is dry.");
    } else if (soilMoistureValue < moistureThreshold && pumpActive) {
        digitalWrite(pumpRelayPin, LOW); 
        pumpActive = false;              
        lcd.setCursor(0, 1);
        lcd.print("Pump OFF - Wet soil");
        Serial.println("Pump OFF - Soil is wet.");
    }
}
