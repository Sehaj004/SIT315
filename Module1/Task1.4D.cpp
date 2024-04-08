//Anshpreet Singh
//2210994762
const int tempSensorPin = A0;    // Analog pin for temperature sensor
const int pirSensorPin = 3;      // Digital pin for PIR sensor
const int ledPin = 7;            // Digital pin for LED
const int ledPin1 = 13;          // Digital pin for other LED
const int tiltSensorPin = 2;     // Digital pin for tilt sensor

// Flags for sensor detections
volatile boolean pirDetected = false; // Flag for PIR sensor detection
volatile boolean tiltDetected = false; // Flag for tilt sensor detection

void setup() {
  // Initialize serial communication
  Serial.begin(9600);

  // Configure sensor and LED pins
  pinMode(tempSensorPin, INPUT);   // Temperature sensor pin as input
  pinMode(ledPin, OUTPUT);          // LED pin as output
  pinMode(pirSensorPin, INPUT);     // PIR sensor pin as input
  pinMode(tiltSensorPin, INPUT);    // Tilt sensor pin as input

  attachInterrupt(digitalPinToInterrupt(pirSensorPin), pirInterrupt, CHANGE); // PIR sensor interrupt on pin change
  attachInterrupt(digitalPinToInterrupt(tiltSensorPin), tiltInterrupt, CHANGE); // Tilt sensor interrupt on pin change
}

// Interrupt for PIR sensor
void pirInterrupt() {
  digitalWrite(ledPin1, (digitalRead(pirSensorPin))); // Set other LED based on PIR sensor state
}

// Interrupt for tilt sensor
void tiltInterrupt() {
  digitalWrite(ledPin1, (digitalRead(tiltSensorPin))); // Set other LED based on tilt sensor state
}

// Function to check temperature and control main LED
void checkTemperature() {
  // Read temperature sensor value
  int tempValue = analogRead(tempSensorPin);
  // Convert sensor value to temperature in Celsius
  float voltage = (tempValue / 1024.0) * 5.0;
  float temperature = (voltage - 0.5) * 100;

  // Print temperature to serial monitor
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" °C");

  // Control main LED based on temperature and sensor detections
  if (temperature > 25.0 || pirDetected || tiltDetected) {
    digitalWrite(ledPin, HIGH); // Turn on main LED
    Serial.println("LED ON");
  } else {
    digitalWrite(ledPin, LOW); // Turn off main LED
    Serial.println("LED OFF");
  }

  // Reset sensor detection flags
  if (!pirDetected && !tiltDetected) {
    pirDetected = false;
  }
}

void loop() {
  // Continuously check temperature and sensor detections
  checkTemperature();
  // Delay for stability and to avoid flooding the serial monitor
  delay(1000);
}
