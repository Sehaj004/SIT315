const int tempSensorPin = A0;
const int ledPin = 7;

void setup() {
  Serial.begin(9600);
  pinMode(tempSensorPin, INPUT);
  pinMode(ledPin, OUTPUT);
}

void loop() {
  int tempValue = analogRead(tempSensorPin);
  float voltage = (tempValue / 1024.0) * 5.0;
  float temperature = (voltage - 0.5) * 100;

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" °C");

  if (temperature > 20.0) { // Adjust the threshold as needed
    digitalWrite(ledPin, HIGH);
    Serial.println("High Temperature - LED ON");
  } else {
    digitalWrite(ledPin, LOW);
    Serial.println("Normal Temperature - LED OFF");
  }

  delay(1000); // Adjust the delay according to preference
}
