//Anshpreet Singh
//2210994762
/* 
  Calculations (for 500ms):
  System clock 16 Mhz and Prescalar 256;
  Timer1 speed = 16Mhz/256 = 62500Hz
  Count for 500ms = 500ms * 62500Hz = 31250
*/
const int PIR_PIN = 2;         // Pin connected to the PIR sensor
const int PIR_LED_PIN = 8;     // Pin connected to the PIR LED

const int TILT_PIN = 3;        // Pin connected to the TILT sensor
const int TILT_LED_PIN = 10;   // Pin connected to the TILT LED

const int SWITCH_PIN = 4;      // Pin connected to the switch
const int SWITCH_LED_PIN = 12; // Pin connected to the switch LED

const int TIMER_LED_PIN = 13;  // Pin connected to the timer LED
bool LED_STATE = true;         // Initial state for LED

volatile byte motionState, tiltState, switchState;

void setup()
{
  pinMode(TILT_PIN, INPUT_PULLUP);    // Set TILT pin as an input with pull-up resistor
  pinMode(PIR_PIN, INPUT);             // Set PIR pin as an input
  pinMode(SWITCH_PIN, INPUT);          // Set switch pin as an input

  pinMode(PIR_LED_PIN, OUTPUT);       // Set PIR LED pin as an output
  pinMode(TILT_LED_PIN, OUTPUT);      // Set TILT LED pin as an output
  pinMode(SWITCH_LED_PIN, OUTPUT);    // Set switch LED pin as an output

  PCICR |= B00000100;  // Enable Pin Change Interrupt for Port D
  PCMSK2 |= B00011100; // Enable Pin Change Interrupt for Pins D2, D3, and D4

  // Read initial sensor states
  motionState = digitalRead(PIR_PIN);
  tiltState = !digitalRead(TILT_PIN);
  switchState = digitalRead(SWITCH_PIN);

  cli(); // Disable interrupts for configuration
  // Configure Timer 1 for interrupt generation//triggering value
  TCCR1A = 0;         // Reset TCCR1A //control register //clear previous configurations
  TCCR1B = 0;         // Reset TCCR1B //mask register
  TCCR1B |= B00000100; // Set CS12 to 1 for prescaler of 256 //increment everyr 256
  TIMSK1 |= B00000010; // Enable compare match mode for register A
  OCR1A = 31250;       // Set value of register A to 31250
  sei();              // Enable interrupts

  Serial.begin(9600); // Initialize serial communication
}

void loop()
{
  printSensorStates(); // Continuously print sensor states
}

void printSensorStates()
{
  // Print sensor states
  Serial.print("PIR State: ");
  Serial.print(motionState);
  Serial.print("\t");
  Serial.print("TILT State: ");
  Serial.print(tiltState);
  Serial.print("\t");
  Serial.print("Switch State: ");
  Serial.println(switchState);
}

ISR(TIMER1_COMPA_vect)
{
  TCNT1 = 0;                          // Reset Timer to 0 for next interrupt
  LED_STATE = !LED_STATE;             // Toggle LED state
  digitalWrite(TIMER_LED_PIN, LED_STATE); // Update timer LED state
}

ISR(PCINT2_vect)
{
  // Read current states of PIR, TILT, and switch pins
  motionState = (PIND & B00000100) ? 1 : 0;
  tiltState = (PIND & B00001000) ? 0 : 1; // Invert tilt state
  switchState = (PIND & B00010000) ? 1 : 0;

  // Update LED states based on sensor states
  digitalWrite(PIR_LED_PIN, motionState);
  digitalWrite(TILT_LED_PIN, tiltState);
  digitalWrite(SWITCH_LED_PIN, switchState);
}

//Anshpreet Singh
//2210994762
int state = 0;

void setup() {
  pinMode(4, OUTPUT);
  pinMode(2, INPUT);
  attachInterrupt(digitalPinToInterrupt(2), switch1, CHANGE);
  Serial.begin(9600);
}

void loop() {
  state = digitalRead(2);
  digitalWrite(4, state);
  Serial.println(state);
}

void switch1() {
}
