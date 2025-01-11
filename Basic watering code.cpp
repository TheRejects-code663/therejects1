// Define pins
const int soilMoisturePin = A0;  // Soil moisture sensor pin
const int pumpRelayPin = 7;      // Relay control pin

// Define thresholds
int moistureThreshold = 500;      // Threshold for soil moisture (adjustable)

// Debug mode (set to true for debugging, false to disable)
#define DEBUG true

// Timeout for pump operation (in milliseconds)
const unsigned long pumpTimeout = 60000; // 1 minute timeout for pump run
unsigned long previousMillis = 0;        // For tracking time
bool pumpActive = false;                 // To track if the pump is on

void setup() {
  // Initialize the relay pin
  pinMode(pumpRelayPin, OUTPUT);
  digitalWrite(pumpRelayPin, LOW);  // Ensure the pump is off initially

  // Start serial communication for debugging
  Serial.begin(9600);

  if (DEBUG) {
    Serial.println("Irrigation System Initialized");
  }
}

void loop() {
  // Read the soil moisture value with noise reduction by averaging
  int soilMoistureValue = 0;
  for (int i = 0; i < 10; i++) {
    soilMoistureValue += analogRead(soilMoisturePin);
  }
  soilMoistureValue /= 10;  // Averaging over 10 readings

  if (DEBUG) {
    Serial.print("Soil Moisture Value: ");
    Serial.println(soilMoistureValue);
  }

  // If soil moisture is below threshold and the pump is not already active
  if (soilMoistureValue < moistureThreshold && !pumpActive) {
    if (DEBUG)}
      Serial.println("Soil is dry. Turning on the pump..."); 
    digitalWrite (pumpRelayPin, HIGH);