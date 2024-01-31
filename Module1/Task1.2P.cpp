const int tempSensorPin = A0;
const int ledPin = 7;
const int buttonPin = 2; 
volatile boolean buttonPressed = false;

void setup() {
  Serial.begin(9600);
  pinMode(tempSensorPin, INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(buttonPin), buttonInterrupt, FALLING);
}

void loop() {
  if (buttonPressed) {
    checkTemperature();
    buttonPressed = false;
  }
}

void buttonInterrupt() {
  buttonPressed = true;
}

void checkTemperature() {
  int tempValue = analogRead(tempSensorPin);
  float voltage = (tempValue / 1024.0) * 5.0;
  float temperature = (voltage - 0.5) * 100;

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" °C");

  if (temperature > 20.0) { 
    digitalWrite(ledPin, HIGH);
    Serial.println("High Temperature - LED ON");
  } else {
    digitalWrite(ledPin, LOW);
    Serial.println("Normal Temperature - LED OFF");
  }

  delay(1000); 
}