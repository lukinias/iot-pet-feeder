#define CAYENNE_PRINT Serial
#include <CayenneMQTTESP8266.h>

// WiFi network info.
char* ssid = "negro_el_33";
char* wifiPassword = "taunus23";

// Cayenne authentication info. This should be obtained from the Cayenne Dashboard.
char* username = "cb397b70-a4a2-11e9-be3b-372b0d2759ae";
char* password = "49252f9b110f6297b7ce88ef42ba58450395d91f";
char* clientID = "10cc3480-a655-11e9-94e9-493d67fd755e";

unsigned long lastMillis = 0;
int pinLed = 16;

void setup()
{
  Serial.begin(9600);
  pinMode(pinLed, OUTPUT);
  Cayenne.begin(username, password, clientID, ssid, wifiPassword);
}

void loop()
{
  Cayenne.loop();
  if (millis() - lastMillis > 10000)
  {
    lastMillis = millis();
    Cayenne.virtualWrite(0, lastMillis);
  }
}

CAYENNE_IN(1) // Led command
{
  int currentValue = getValue.asInt();
  if (currentValue == 1)
  {
    digitalWrite(pinLed, HIGH);
  }
  else
  {
    digitalWrite(pinLed, LOW);
  }
}

/*CAYENNE_IN_DEFAULT()
{
  CAYENNE_LOG(“CAYENNE_IN_DEFAULT(%u) - %s, %s”, request.channel, getValue.getId(), getValue.asString());
}*/
