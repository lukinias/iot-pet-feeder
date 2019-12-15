const int EchoPin = 11;
const int TriggerPin = 12;

float distancia;
long tiempo;
 
void setup() {
  Serial.begin(9600);
  pinMode(TriggerPin, OUTPUT);
  pinMode(EchoPin, INPUT);
}
 
void loop() {

  digitalWrite(TriggerPin, HIGH);  //se envía un pulso para activar el sensor
  delayMicroseconds(10);
  digitalWrite(TriggerPin, LOW);

  // medimos el pulso de respuesta
  tiempo = (pulseIn(EchoPin, HIGH)/2); 
  
  // dividido por 2 por que es el 
  // tiempo que el sonido tarda
  // en ir y en volver
  // ahora calcularemos la distancia en cm
  // sabiendo que el espacio es igual a la velocidad por el tiempo
  // y que la velocidad del sonido es de 343m/s y que el tiempo lo 
  // tenemos en millonesimas de segundo
  
  distancia = float(tiempo * 0.0343);

  Serial.print("Distancia: "); // imprime la distancia en el Monitor Serie
  Serial.print(distancia);
  Serial.println(" cm"); 
    
  delay(1000);
}
