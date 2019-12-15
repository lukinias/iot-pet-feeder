
int ledPin = 6;
int analogPin = 0;
int val = 0;

void setup() {
  pinMode(ledPin, OUTPUT);
}

void loop() {
  val = analogRead(analogPin);
  analogWrite(ledPin, val / 4);
}
