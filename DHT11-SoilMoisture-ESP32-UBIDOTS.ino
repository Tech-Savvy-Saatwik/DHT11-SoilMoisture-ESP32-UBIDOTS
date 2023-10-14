#include "UbidotsEsp32Mqtt.h"
#include "DHT.h"
#define DHTPIN 18
#define Spin 21
#define DHTTYPE DHT11   
DHT dht(DHTPIN, DHTTYPE);
const char *UBIDOTS_TOKEN = "BBFF-UBLFGoqfx7pt3DLttkyKP5o72feIrM";         //Paste the ID credentials from the website.
const char *WIFI_SSID = "CodingScientist";              //Enter your WiFi connection name.
const char *WIFI_PASS = "BumBiteTrisha";              //Enter the password of your WiFi.
const char *DEVICE_LABEL = "DHT11-SoilMoisture-ESP32-C";
const char *VARIABLE_LABEL_1 = "Temperature";
const char *VARIABLE_LABEL_2 = "Humidity";
const char *VARIABLE_LABEL_3 = "Soil_moisture"; 
const int PUBLISH_FREQUENCY = 5000; 
unsigned long timer; 
Ubidots ubidots(UBIDOTS_TOKEN);

void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void setup()
{
  Serial.begin(9600);     
  delay(1000);
  ubidots.connectToWifi(WIFI_SSID, WIFI_PASS);
  ubidots.setCallback(callback);
  ubidots.setup();
  ubidots.reconnect();
  timer = millis();
}

void loop()
{
  if (!ubidots.connected())
  {
    ubidots.reconnect();
  }
  if (millis() - timer) 
  {
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    int s = digitalRead(Spin);
    s = map(s,550,10,0,100);
    ubidots.add(VARIABLE_LABEL_1, t);
    ubidots.add(VARIABLE_LABEL_2, h);
    ubidots.add(VARIABLE_LABEL_3, s);
    ubidots.publish(DEVICE_LABEL);
    Serial.println("Temperature: " + String(t));
    Serial.println("Humidity: " + String(h));
    Serial.println("Soil_moisture: " + String(s));
    Serial.println("-----------------------------------------");
    timer = millis();
  }
  delay(10000); 
  ubidots.loop();
}