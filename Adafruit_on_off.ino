#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

#define Dv1           D0
#define Dv2           D1
#define Dv3           D2
#define Dv4           D3


#define WLAN_SSID       "**********"         
#define WLAN_PASS       "**********"       

/************************* Adafruit.io Setup *********************************/

#define AIO_SERVER      "io.adafruit.com" 
#define AIO_SERVERPORT  1883                   
#define AIO_USERNAME    "**********"           
#define AIO_KEY         "**********"  

//WIFI CLIENT
WiFiClient client;

Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

Adafruit_MQTT_Subscribe Device1 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/relay1"); 
Adafruit_MQTT_Subscribe Device2 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/relay2");
Adafruit_MQTT_Subscribe Device3 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/relay3");
Adafruit_MQTT_Subscribe Device4 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/relay4");


void MQTT_connect();

void setup() {
  Serial.begin(115200);

  pinMode(Dv1, OUTPUT);
  pinMode(Dv2, OUTPUT);
  pinMode(Dv3, OUTPUT);
  pinMode(Dv4, OUTPUT);

  digitalWrite(Dv1, LOW);
  digitalWrite(Dv2, LOW);
  digitalWrite(Dv3, LOW);
  digitalWrite(Dv4, LOW);

  
  // Connect to WiFi access point.
  Serial.println(); Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.println("WiFi connected");
  Serial.println("IP address: "); 
  Serial.println(WiFi.localIP());
 
  mqtt.subscribe(&Device1);
  mqtt.subscribe(&Device2);
  mqtt.subscribe(&Device3);
  mqtt.subscribe(&Device4);

}

void loop() {
 
  MQTT_connect();
  

  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(20000))) {
    if (subscription == &Device1) {
      Serial.print(F("Got: "));
      Serial.println((char *)Device1.lastread);
      int Device1_State = atoi((char *)Device1.lastread);
      digitalWrite(Dv1, Device1_State);
      
    }
    if (subscription == &Device2) {
      Serial.print(F("Got: "));
      Serial.println((char *)Device2.lastread);
      int Device2_State = atoi((char *)Device2.lastread);
      digitalWrite(Dv2, Device2_State);
    }
    if (subscription == &Device3) {
      Serial.print(F("Got: "));
      Serial.println((char *)Device3.lastread);
      int Device3_State = atoi((char *)Device3.lastread);
      digitalWrite(Dv3, Device3_State);
    }
    if (subscription == &Device4) {
      Serial.print(F("Got: "));
      Serial.println((char *)Device4.lastread);
      int Device4_State = atoi((char *)Device4.lastread);
      digitalWrite(Dv4, Device4_State);
    }
  }

  
}

void MQTT_connect() {
  int8_t ret;

  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  
  while ((ret = mqtt.connect()) != 0) {
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println("Retrying MQTT connection in 5 seconds...");
    mqtt.disconnect();
    delay(5000); 
    retries--;
    if (retries == 0) {
      while (1);
    }
  }
  Serial.println("MQTT Connected!");
  
}
