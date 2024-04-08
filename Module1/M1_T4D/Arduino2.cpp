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
