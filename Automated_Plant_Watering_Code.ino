// Define the pins for the components
const int moisturePin = 0;  // Soil moisture sensor input
const int relayPin = 6;      // Relay control pin

// Set the threshold moisture level (adjust based on sensor calibration)
int moistureThreshold = 400;  // Moisture level threshold (lower values mean dryer soil)

// Variable to store moisture sensor readings
int moistureValue = 0;

void setup() {
  // Start the serial monitor
  Serial.begin(9600);
  
  // Set the relay pin as output
  pinMode(relayPin, OUTPUT);
  
  // Initially turn off the relay (pump off)
  digitalWrite(relayPin, LOW);
}

void loop() {
  // Read the moisture level from the soil sensor
  moistureValue = analogRead(moisturePin);
  
  // Print the moisture value to the serial monitor for debugging
  Serial.print("Moisture Value: ");
  Serial.println(moistureValue);
  
  // Check if the moisture value is below the threshold (dry soil)
  if (moistureValue < moistureThreshold) {
    // If soil is dry, turn on the water pump
    digitalWrite(relayPin, HIGH);
    Serial.println("Soil is dry. Watering...");
  } else {
    // If soil is wet, turn off the water pump
    digitalWrite(relayPin, LOW);
    Serial.println("Soil is wet. No need to water.");
  }
  
  // Wait for a short period before checking again
  delay(5000);  // Delay in milliseconds (5 seconds)
}
