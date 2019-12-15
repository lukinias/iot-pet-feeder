#include <ESP8266WiFi.h>        // Include the Wi-Fi library
#include <PubSubClient.h>       // MQTT pub sub client

const char* ssid     = "UTN-IOT";  // The SSID (name) of the Wi-Fi network you want to connect to
const char* password = "123456789";     // The password of the Wi-Fi network

const char* mqttServer = "postman.cloudmqtt.com";
const int mqttPort = 13850;
const char* mqttUser = "vibbfwta";
const char* mqttPassword = "oouQQ8IW43dO";

/* 
Pines
NodeMCU   Driver ULN200
  D5          IN1
  D6          IN2
  D7          IN3
  D8          IN4
*/

int retardo=5;          // Tiempo de retardo en milisegundos (Velocidad del Motor)
int numero_pasos;       // Valor en grados donde se encuentra el motor
String leeCadena;       // Almacena la cadena de datos recibida
int pin_1 = D5;
int pin_2 = D6;
int pin_3 = D7;
int pin_4 = D8;
int grados = 0;
int bomba = D1;
int sensor = D2;

WiFiClient espClient;
PubSubClient client(espClient);
  
void setup() {                
  Serial.begin(9600);     // inicializamos el puerto serie a 9600 baudios
  pinMode(pin_4, OUTPUT);    // Pin D8 conectar a IN4
  pinMode(pin_3, OUTPUT);    // Pin D7 conectar a IN3
  pinMode(pin_2, OUTPUT);    // Pin D6 conectar a IN2
  pinMode(pin_1, OUTPUT);    // Pin D5 conectar a IN1
  pinMode(bomba, OUTPUT);
  pinMode(sensor, INPUT);

  Serial.println('\n');
  WiFi.begin(ssid, password); // Connect to the network
  Serial.print("Connecting to "); Serial.print(ssid); Serial.println(" ...");

  int i = 0;
  while (WiFi.status() != WL_CONNECTED) { // Wait for the Wi-Fi to connect
    delay(1000);
    Serial.print(++i); Serial.print(' ');
  }

  Serial.println('\n');
  Serial.println("Connection established!");  
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP()); // Send the IP address of the ESP8266 to the computer

  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);

  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
 
    if (client.connect("ESP8266Client", mqttUser, mqttPassword )) {
 
      Serial.println("connected");  
 
    } else {
 
      Serial.print("failed with state ");
      Serial.println(client.state());
      delay(2000);
 
    }
  }
 
  client.publish("esp/test", "Hello from ESP8266");
  client.subscribe("/feeder/comer");
  client.subscribe("/feeder/beber");
  client.subscribe("/feeder/calibrar");
}

void loop() {

  client.loop();
  
  while (Serial.available()) {   // Leer el valor enviado por el Puerto serial
    delay(retardo);
    char c  = Serial.read();     // Lee los caracteres
    leeCadena += c;              // Convierte Caracteres a cadena de caracteres
  }  
  
  leeCadena.trim();

  /*
   * Si la mascota anda cerca, prendemos la fuente de agua por un rato
   */
   int sensorValue;
  sensorValue = digitalRead(sensor);
  if (sensorValue == LOW) {
    digitalWrite(bomba, HIGH);
    client.publish("/feeder/presencia", "Gato cerca! ");
    delay(5000);
    digitalWrite(bomba, LOW);
    client.publish("/feeder/presencia", "");
  }

  leeCadena = "";   // Inicializamos la cadena de caracteres recibidos 
  apagado();        // Apagado del Motor para que no se caliente
}

// Pasos
void paso() {
  digitalWrite(pin_4, HIGH); 
  digitalWrite(pin_3, HIGH);  
  digitalWrite(pin_2, LOW);  
  digitalWrite(pin_1, LOW);  
  delay(retardo); 
  digitalWrite(pin_4, LOW); 
  digitalWrite(pin_3, HIGH);  
  digitalWrite(pin_2, HIGH);  
  digitalWrite(pin_1, LOW);  
  delay(retardo); 
  digitalWrite(pin_4, LOW); 
  digitalWrite(pin_3, LOW);  
  digitalWrite(pin_2, HIGH);  
  digitalWrite(pin_1, HIGH);  
  delay(retardo); 
  digitalWrite(pin_4, HIGH); 
  digitalWrite(pin_3, LOW);  
  digitalWrite(pin_2, LOW);  
  digitalWrite(pin_1, HIGH);  
  delay(retardo); 
}

// Apagado del Motor
void apagado() {
  digitalWrite(11, LOW); 
  digitalWrite(10, LOW);  
  digitalWrite(9, LOW);  
  digitalWrite(8, LOW);  
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.println();
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);

  /*
   * Le damos de comer
   */
  if (String(topic) == "/feeder/comer") {
    grados += 60 * 1.4222222222;
    while (grados > numero_pasos){   // Giro en grados
      paso();
      numero_pasos = numero_pasos + 1;
    }
  }

  /*
   * Prendemos la fuente de agua por un rato
   */
  if (String(topic) == "/feeder/beber") {
    digitalWrite(bomba, HIGH);
    delay(3000);
    digitalWrite(bomba, LOW);
  }

  /*
   * Clibra la posicion del alimentador
   */
  if (String(topic) == "/feeder/calibrar") {
    grados += 10 * 1.4222222222;
    while (grados > numero_pasos){   // Giro en grados
      paso();
      numero_pasos = numero_pasos + 1;
    }
  }
 
  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
 
  Serial.println();
  Serial.println("-----------------------");

}
