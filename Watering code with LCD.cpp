#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Define pins
const int soilMoisturePin = A0; // Soil moisture sensor pin
const int pumpRelayPin = 7; // Relay control pin

// Define thresholds
int moistureThreshold = 100; // Threshold for soil moisture (adjustable)

// Debug mode (set to true for debugging, false to disable)
#define DEBUG true

// Timeout for pump operation (in milliseconds)
const unsigned long pumpTimeout = 60000; // 1 minute timeout for pump run
unsigned long previousMillis = 0; // For tracking time
bool pumpActive = false; // To track if the pump is on

LiquidCrystal_I2C lcd(0x27, 16, 2); // Replace 0x27 with your I2C address if needed

void setup() {
  // Initialize the relay pin
  pinMode(pumpRelayPin, OUTPUT);
  digitalWrite(pumpRelayPin, LOW); // Ensure the pump is off initially

  // Start serial communication for debugging
  Serial.begin(9600);

  // Initialize the LCD
  lcd.begin(16, 2); 
  lcd.backlight(); // Turn on the backlight
  lcd.print("System loading...");
  delay(1000); // Display loading message for a second
  lcd.clear(); // Clear the display
}

void loop() {
  // Read soil moisture value
  int soilMoistureValue = 0;

  // Read the soil moisture value with noise reduction by averaging
  for (int i = 0; i < 25; i++) {
    soilMoistureValue += analogRead(soilMoisturePin);
  }
  soilMoistureValue /= 25; // Averaging over 25 readings

  // Print soil moisture value to Serial Monitor (for debugging)
  if (DEBUG) {
    Serial.print("Soil Moisture: ");
    Serial.println(soilMoistureValue);
  }

  // Update the LCD display only if the state has changed
  static int lastSoilMoistureValue = -1;
  if (soilMoistureValue != lastSoilMoistureValue) {
    lcd.clear();
    if (soilMoistureValue < moistureThreshold) {
      lcd.print("Soil Dry!!");
    } else {
      lcd.print("Soil OK");
    }
    lastSoilMoistureValue = soilMoistureValue;
  }

  // Control the pump
  if (soilMoistureValue < moistureThreshold && !pumpActive) {
    if (DEBUG) {
      Serial.println("Soil is dry. Turning on the pump...");
    }
    digitalWrite(pumpRelayPin, HIGH); // Turn on the pump
    pumpActive = true; // Mark pump as active
    previousMillis = millis(); // Record the time pump was turned on
  }

  if (pumpActive) {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= pumpTimeout) {
      if (DEBUG) {
        Serial.println("Pump timeout reached. Turning off the pump...");
      }
      digitalWrite(pumpRelayPin, LOW); // Turn off the pump
      pumpActive = false; // Mark pump as inactive
      lcd.clear();
      lcd.print("Pump Off");
      delay(1000); // Display "Pump Off" for 1 second
    }
  }

  delay(1000); // Small delay to stabilize the loop
}