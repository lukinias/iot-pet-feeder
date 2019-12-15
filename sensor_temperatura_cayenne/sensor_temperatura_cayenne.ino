
#include <DHT.h>
#define DHTPIN 14
#define DHTTYPE DHT11   // DHT 11 
//#define DHTTYPE DHT22   // DHT 22 
DHT dht(DHTPIN, DHTTYPE, 11);

#define ledrojo 16 
float t, h;

//#define CAYENNE_DEBUG
#define CAYENNE_PRINT Serial
#include <CayenneMQTTESP8266.h>

// WiFi network info.
char ssid[] = "negro_el_33";
char wifiPassword[] = "taunus23";

// Cayenne authentication info. This should be obtained from the Cayenne Dashboard.
char username[] = "cb397b70-a4a2-11e9-be3b-372b0d2759ae";
char password[] = "49252f9b110f6297b7ce88ef42ba58450395d91f";
char clientID[] = "10cc3480-a655-11e9-94e9-493d67fd755e";

unsigned long lastMillis = 0;

void setup() {
  Serial.begin(9600);
  Serial.println();
  Serial.println("NodeMCU - Cayenne IoT");
  dht.begin();
  pinMode(ledrojo, OUTPUT);  
  h = dht.readHumidity();
  t = dht.readTemperature();
  Serial.print("Temperatura: ");
  Serial.print(t);
  Serial.print(" grados - Humedad: "); 
  Serial.println(h);
  Serial.println();
  Cayenne.begin(username, password, clientID, ssid, wifiPassword);
}

void loop() {
  Cayenne.loop();

  //Publicamos cada 10 segundos
  if (millis() - lastMillis > 10000) {
    h = dht.readHumidity();
    t = dht.readTemperature();
    Serial.println();
    Serial.print("Temperatura: ");
    Serial.print(t);
    Serial.print(" grados - Humedad: "); 
    Serial.println(h);
    if (isnan(h) || isnan(t)) {
      Serial.println("Fallo al leer sensor DHT!");
      return;
    }
    
    lastMillis = millis();

    Cayenne.celsiusWrite(1, t);
    Cayenne.virtualWrite(2, h);

    // Si la temeratura supera los 27 grados, enciendo el led rojo
    if (t > 18.00)  
    {
      digitalWrite(ledrojo, HIGH);   // Turn the LED on
      Serial.println("demasiada temperatura !");
    }
    else
    {
      digitalWrite(ledrojo, LOW);   // Turn the LED off
    }
  }
}  // end main loop

//Default function for processing actuator commands from the Cayenne Dashboard.
//You can also use functions for specific channels, e.g CAYENNE_IN(1) for channel 1 commands.
CAYENNE_IN_DEFAULT()
{
  CAYENNE_LOG("CAYENNE_IN_DEFAULT(%u) - %s, %s", request.channel, getValue.getId(), getValue.asString());
  //Process message here. If there is an error set an error message using getValue.setError(), e.g getValue.setError("Error message");
}

CAYENNE_IN(3)  // control del led rojo
{   
  int currentValue=getValue.asInt();
  if(currentValue==1)
  {
    digitalWrite(ledrojo,HIGH);
  }
  else
  {
    digitalWrite(ledrojo,LOW); 
  }
}

CAYENNE_OUT(3)
{
  bool state = digitalRead(ledrojo);
  if (state == HIGH) {
    Cayenne.virtualWrite(3, 1);
  }
  else
  {
    Cayenne.virtualWrite(3, 0);
  }
}
