
const int pump = 5;

void setup() {
  pinMode(pump, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  Serial.println("water pump on");
  digitalWrite(pump, HIGH);
  delay(2000);

  Serial.println("water pump off");
  digitalWrite(pump, LOW);
  delay(2000);
}
