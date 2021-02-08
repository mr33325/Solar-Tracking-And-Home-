/***************************************************
  Adafruit MQTT Library ESP8266 Example

  Must use ESP8266 Arduino from:
   https://github.com/esp8266/Arduino

  Works great with Adafruit's Huzzah ESP board & Feather
  ----> https://www.adafruit.com/product/2471
  ----> https://www.adafruit.com/products/2821

  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Tony DiCola for Adafruit Industries.
  MIT license, all text above must be included in any redistribution

  Code is edited by Sachin Soni for it's project called
  Ultimate Home Automation

  For Project video, visit his YouTube channel named "CreativeDesk"




****************************************************/
//for esp32 use <wifi.h> ,for esp8266 use <esp8266wifi.h>
// this code is for esp32 for using esp8266 you just need to change <wifi.h> into <esp8266wifi.h> and also change the relay pin
#include <WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>


/************************* Pin Definition *********************************/

//Relays for switching appliances
#define Relay1            12 //light
#define Relay2            13
#define Relay3            14 //fan
#define Relay4            27
#define buzzer            26
#define RXD2              16
#define TXD2              17
#define lightfan          4

//buzzer to know the status of MQTT connections and can be used for any other purpose according to your project need.




/************************* WiFi Access Point *********************************/

#define WLAN_SSID       "Redmi1"
#define WLAN_PASS       "puspa_hai_tu"

/************************* Adafruit.io Setup *********************************/

#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                   // use 8883 for SSL
#define AIO_USERNAME    "mr33325"
#define AIO_KEY         "5aeeaab390a04f2eaa9ea907852b78a8"

/************ Global State (you don't need to change this!) ******************/

// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient client;
// or... use WiFiFlientSecure for SSL
//WiFiClientSecure client;

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

/****************************** Feeds ***************************************/

// Notice MQTT paths for AIO follow the form: <username>/feeds/<feedname>

Adafruit_MQTT_Publish Light = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/relay1");
Adafruit_MQTT_Publish Fan = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/relay2");
Adafruit_MQTT_Publish Humidity = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/Humidity");
Adafruit_MQTT_Publish Temperature = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/Temperature");

// Setup a feed called 'onoff' for subscribing to changes.
Adafruit_MQTT_Subscribe Light1 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/relay1");
Adafruit_MQTT_Subscribe Fan1 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/relay2");
Adafruit_MQTT_Subscribe Light2 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/relay3");
Adafruit_MQTT_Subscribe Fan2 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/relay4");

/************************* define variable **********************************/

bool x = true;
bool light_fan_off = false;

#define DHTPIN 2     // Digital pin connected to the DHT sensor 
// Uncomment the type of sensor in use:
#define DHTTYPE    DHT11     // DHT 11
//#define DHTTYPE    DHT22     // DHT 22 (AM2302)
//#define DHTTYPE    DHT21     // DHT 21 (AM2301)

// See guide for details on sensor wiring and usage:
//   https://learn.adafruit.com/dht/overview

DHT_Unified dht(DHTPIN, DHTTYPE);

uint32_t delayMS;

/*************************** Sketch Code ************************************/

// Bug workaround for Arduino 1.6.6, it seems to need a function declaration
// for some reason (only affects ESP8266, likely an arduino-builder bug).
void MQTT_connect();

void setup() {
  Serial.begin(115200);

  delay(10);

  pinMode(Relay1, OUTPUT);
  pinMode(Relay2, OUTPUT);
  pinMode(Relay3, OUTPUT);
  pinMode(Relay4, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(lightfan, INPUT);

  Serial.println(F("Adafruit MQTT demo"));

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

  // Setup MQTT subscription for onoff feed.
  mqtt.subscribe(&Light1);
  mqtt.subscribe(&Fan1);
  mqtt.subscribe(&Light2);
  mqtt.subscribe(&Fan2);

  Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2);
  Serial.println("Serial Txd is on pin: " + String(TX));
  Serial.println("Serial Rxd is on pin: " + String(RX));

  dht.begin();
  Serial.println(F("DHTxx Unified Sensor Example"));
  // Print temperature sensor details.
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  Serial.println(F("------------------------------------"));
  Serial.println(F("Temperature Sensor"));
  Serial.print  (F("Sensor Type: ")); Serial.println(sensor.name);
  Serial.print  (F("Driver Ver:  ")); Serial.println(sensor.version);
  Serial.print  (F("Unique ID:   ")); Serial.println(sensor.sensor_id);
  Serial.print  (F("Max Value:   ")); Serial.print(sensor.max_value); Serial.println(F("°C"));
  Serial.print  (F("Min Value:   ")); Serial.print(sensor.min_value); Serial.println(F("°C"));
  Serial.print  (F("Resolution:  ")); Serial.print(sensor.resolution); Serial.println(F("°C"));
  Serial.println(F("------------------------------------"));
  // Print humidity sensor details.
  dht.humidity().getSensor(&sensor);
  Serial.println(F("Humidity Sensor"));
  Serial.print  (F("Sensor Type: ")); Serial.println(sensor.name);
  Serial.print  (F("Driver Ver:  ")); Serial.println(sensor.version);
  Serial.print  (F("Unique ID:   ")); Serial.println(sensor.sensor_id);
  Serial.print  (F("Max Value:   ")); Serial.print(sensor.max_value); Serial.println(F("%"));
  Serial.print  (F("Min Value:   ")); Serial.print(sensor.min_value); Serial.println(F("%"));
  Serial.print  (F("Resolution:  ")); Serial.print(sensor.resolution); Serial.println(F("%"));
  Serial.println(F("------------------------------------"));
  // Set delay between sensor readings based on sensor details.
  delayMS = sensor.min_delay / 1000;
}


void loop() {

  // Ensure the connection to the MQTT server is alive (this will make the first
  // connection and automatically reconnect when disconnected).  See the MQTT_connect
  // function definition further below.
  MQTT_connect();
  // this is our 'wait for incoming subscription packets' busy subloop
  // try to spend your time here

  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(10000))) {
    if (subscription == &Light1) {
      Serial.print(F("Got: "));
      Serial.println((char *)Light1.lastread);
      int Light1_State = atoi((char *)Light1.lastread);
      digitalWrite(Relay1, Light1_State);

    }
    if (subscription == &Light2) {
      Serial.print(F("Got: "));
      Serial.println((char *)Light2.lastread);
      int Light2_State = atoi((char *)Light2.lastread);
      digitalWrite(Relay2, Light2_State);
    }
    if (subscription == &Fan1) {
      Serial.print(F("Got: "));
      Serial.println((char *)Fan1.lastread);
      int Fan1_State = atoi((char *)Fan1.lastread);
      digitalWrite(Relay3, Fan1_State);
    }
    if (subscription == &Fan2) {
      Serial.print(F("Got: "));
      Serial.println((char *)Fan2.lastread);
      int Fan2_State = atoi((char *)Fan2.lastread);
      digitalWrite(Relay4, Fan2_State);

    }
  }

  //*******************publish  AUTO light off*****************
  if (digitalRead(lightfan) == HIGH) light_fan_off = true;
  else light_fan_off = false;
  if (light_fan_off) {
    Serial.print(F("\nSending light status "));
    Serial.print(0);
    Serial.print("...");
    if (! Light.publish(0)) {
      Serial.println(F("Failed"));
    } else {
      Serial.println(F("OK!"));
    }

    Serial.print(F("\nSending Fan status "));
    Serial.print(0);
    Serial.print("...");
    if (! Fan.publish(0)) {
      Serial.println(F("Failed"));
    } else {
      Serial.println(F("OK!"));
    }
    light_fan_off = false;
  }

  //*******************publish  humidity and temperature*****************
  delay(delayMS);
  // Get temperature event and print its value.
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature)) {
    Serial.println(F("Error reading temperature!"));
  }
  else {
    Serial.print(F("\nSending Temperature status: "));
    Serial.print(event.temperature);
    Serial.println(F("°C"));
    Serial.print("...");
    if (! Temperature.publish(event.temperature)) {
      Serial.println(F("Failed"));
    } else {
      Serial.println(F("OK!"));
    }
  }

  // Get humidity event and print its value.
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    Serial.println(F("Error reading humidity!"));
  }
  else {
    Serial.print(F("\nSending Humidity status "));
    Serial.print(event.relative_humidity);
    Serial.println(F("%"));
    Serial.print("...");
    if (! Humidity.publish(event.relative_humidity)) {
      Serial.println(F("Failed"));
    } else {
      Serial.println(F("OK!"));
    }
  }
  //************************** serial read and write and publoisg to server************************************


  //Serial.print(F("\nSending light status "));
  //  Serial.print(x);
  //  Serial.print("...");
  //  if (! Light.publish(x)) {
  //    Serial.println(F("Failed"));
  //  } else {
  //    Serial.println(F("OK!"));
  //  }
  //x=!x;
  //if (Serial2.available()) {
  //    char c=Serial2.read();
  //    if(c=='T'){
  //      int i=Serial2.read();
  //    Serial.println("Temperature:"+String(i));
  //    Serial.print(F("\nSending Temperature status "));
  //  Serial.print(i);
  //  Serial.print("...");
  //  if (! Temperature.publish(i)) {
  //    Serial.println(F("Failed"));
  //  } else {
  //    Serial.println(F("OK!"));
  //  }
  //    digitalWrite(2,HIGH);
  //    delay(50);
  //    digitalWrite(2,LOW);
  //    }
  //    else if(c=='H'){
  //      int j=Serial2.read();
  //    Serial.println("Humidity:"+String(j));
  //    Serial.print(F("\nSending Humidity status "));
  //  Serial.print(j);
  //  Serial.print("...");
  //  if (! Humidity.publish(j)) {
  //    Serial.println(F("Failed"));
  //  } else {
  //    Serial.println(F("OK!"));
  //  }
  //    digitalWrite(2,HIGH);
  //    delay(50);
  //    digitalWrite(2,LOW);
  //    }
  //  }
  //  if (Serial.available()) {
  //    Serial2.write(Serial.read());
  //  }
  //  Serial.println(F("done publishing"));
  //*************************************************************
  delay(2000);


  // ping the server to keep the mqtt connection alive
  // NOT required if you are publishing once every KEEPALIVE seconds
  /*
    if(! mqtt.ping()) {
    mqtt.disconnect();
    }
  */
}

// Function to connect and reconnect as necessary to the MQTT server.
// Should be called in the loop function and it will take care if connecting.
void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  digitalWrite(buzzer, HIGH);
  delay(200);
  digitalWrite(buzzer, LOW);
  delay(200);
  digitalWrite(buzzer, HIGH);
  delay(200);
  digitalWrite(buzzer, LOW);
  delay(200);
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println("Retrying MQTT connection in 5 seconds...");
    mqtt.disconnect();
    delay(5000);  // wait 5 seconds
    retries--;
    if (retries == 0) {
      // basically die and wait for WDT to reset me
      while (1);
    }
  }
  Serial.println("MQTT Connected!");
  digitalWrite(buzzer, HIGH);
  delay(2000);
  digitalWrite(buzzer, LOW);
}
