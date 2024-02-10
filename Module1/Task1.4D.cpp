#include <PinChangeInterrupt.h>

const int tempSensorPin = A0;
const int pirSensorPin = 3;
const int lightSensorPin = A1; // Light sensor pin
const int ledPin = 7;
const int buttonPin = 2;
volatile boolean pirDetected = false;
volatile boolean buttonPressed = false;
volatile boolean lightDetected = false;

void setup() {
  Serial.begin(9600);
  pinMode(tempSensorPin, INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(pirSensorPin, INPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(lightSensorPin, INPUT); // Set light sensor pin as input
  
  attachPCINT(digitalPinToPCINT(pirSensorPin), pirInterrupt, CHANGE); // Attaching PCINT for PIR sensor
  attachPCINT(digitalPinToPCINT(buttonPin), buttonInterrupt, FALLING); // Attaching PCINT for button
  attachPCINT(digitalPinToPCINT(lightSensorPin), lightInterrupt, CHANGE); // Attaching PCINT for light sensor

  // Set up timer interrupt
  TCCR1A = 0; // Set Timer 1 Registers to normal mode (zero)
  TCCR1B = 0;
  TCNT1 = 0; // Reset Timer 1 Counter
  OCR1A = 15624; // Compare Match register (16MHz/1024 = 15624)
  TCCR1B |= (1 << WGM12); // Turn on CTC mode
  TCCR1B |= (1 << CS12) | (1 << CS10); // Set CS12 and CS10 bits for 1024 prescaler
  TIMSK1 |= (1 << OCIE1A); // Enable Timer Compare A Match interrupt
}

void loop() {
  if (pirDetected || buttonPressed || lightDetected) {
    checkSensors();
    pirDetected = false;
    buttonPressed = false;
    lightDetected = false;
  }
}

void pirInterrupt() {
  pirDetected = true;
}

void buttonInterrupt() {
  buttonPressed = true;
}

void lightInterrupt() {
  lightDetected = true;
}

void checkSensors() {
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

  if (pirDetected) {
    Serial.println("Motion Detected");
  }

  if (lightDetected) {
    Serial.println("Light Detected");
  }
}

ISR(TIMER1_COMPA_vect) { // Timer interrupt service routine
  static boolean ledState = LOW;
  ledState = !ledState;
  digitalWrite(ledPin, ledState);
}
