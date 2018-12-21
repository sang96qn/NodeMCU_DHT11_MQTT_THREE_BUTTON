/*
  Tran Sang DHBK 0964 500 940
*/
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#define ssid "viettel1234"
#define password "1223334444"
#define server "192.168.1.9" // cloud mqtt
#define topic_pub_motor "Motor"
#define topic_sub_motor "Motor"
#define topic_sub_copony "Copony"
#define topic_pub_copony "Copony"
#define topic_sub_light "Light"
#define topic_pub_light "Light"
#define topic_pub_temperature "Temperature"
#define topic_pub_humidity "Humidity"
#include "DHT.h"

#define DHTPIN 13 // what digital pin we're connected to NodeMCU (D6)

// Uncomment whatever type you're using!
#define DHTTYPE DHT11 // DHT 11

//#define DHTTYPE DHT22 // DHT 22 (AM2302), AM2321
//#define DHTTYPE DHT21 // DHT 21 (AM2301)
// Connect pin 1 (on the left) of the sensor to +5V
// NOTE: If using a board with 3.3V logic like an Arduino Due connect pin 1
// to 3.3V instead of 5V!
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor
// Initialize DHT sensor.
// Note that older versions of this library took an optional third parameter to
// tweak the timings for faster processors. This parameter is no longer needed
// as the current DHT reading algorithm adjusts itself to work on faster procs.

DHT dht(DHTPIN, DHTTYPE);
#include <SimpleTimer.h>
// the timer object
SimpleTimer timer;

const uint16_t port = 1883;// default

WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  payload[length] = '\0';
   char p = char(payload[0]);
   Serial.println(p);
    if(p=='1')
    {
       digitalWrite(2, HIGH);
      Serial.println(" Turn On Motor! " );
    }
    // if MQTT comes a 1, turn on LED on pin D2
    else if(p=='2')
    {
       digitalWrite(2, LOW);
      Serial.println(" Turn Off Motor! " );
    }

    if(p=='3')
    {
       digitalWrite(5, HIGH);
      Serial.println(" Turn On Copony! " );
    }
    // if MQTT comes a 1, turn on LED on pin D2
    else if(p=='4')
    {
       digitalWrite(5, LOW);
      Serial.println(" Turn Off Capony! " );
    }

    if(p=='5')
    {
       digitalWrite(4, HIGH);
      Serial.println(" Turn On Light! " );
    }
    // if MQTT comes a 1, turn on LED on pin D2
    else if(p=='6')
    {
       digitalWrite(4, LOW);
      Serial.println(" Turn Off Light! " );
    }
//  else Serial.println(" Khong hop le");
    Serial.println();
  }
  
// a function to be executed periodically
void repeatMe() {
       // Reading temperature or humidity takes about 250 milliseconds!
      // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
      float h = dht.readHumidity();
      // Read temperature as Celsius (the default)
      float t = dht.readTemperature();

      // Check if any reads failed and exit early (to try again).
      if (isnan(h) || isnan(t)) {
      Serial.println("Failed to read from DHT sensor!");
      return;
      }
      
      Serial.print("Temperature in Celsius:");
      Serial.println(String(t).c_str());
      client.publish(topic_pub_temperature, String(t).c_str(), true);

      Serial.print("Humidity:");
      Serial.println(String(h).c_str());
      client.publish(topic_pub_humidity, String(h).c_str(), true);
}
void setup() {
  Serial.begin(115200);
  pinMode(0,INPUT_PULLUP);
  pinMode(14,INPUT_PULLUP);
  pinMode(12,INPUT_PULLUP);
  pinMode(2,OUTPUT);
  pinMode(5,OUTPUT);
  pinMode(4,OUTPUT);
  digitalWrite(2,LOW);// tat led
  digitalWrite(5,LOW);// tat led
  digitalWrite(4,LOW);// tat led
  setup_wifi();
  dht.begin();
  timer.setInterval(2000, repeatMe);
  client.setServer(server, port);
  client.setCallback(callback);
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
      // Once connected, publish an announcement...

      client.publish(topic_pub_motor,"ESP Connected");
    //  client.publish(topic_pub_motor,"",{retain: 1}); //gửi đè giá trị trống
      // ... and resubscribe
      client.subscribe(topic_sub_motor);
      client.subscribe(topic_sub_copony);
      client.subscribe(topic_sub_light);

    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  timer.run();
  if(digitalRead(0) == LOW)
  {
    while(digitalRead(0) == LOW);
    int statusLed = digitalRead(2);
    if(statusLed == HIGH)// dang sang
    {
      client.publish(topic_pub_motor,"2",{retain: 2});//de tat
      }
      else {
         client.publish(topic_pub_motor,"1",{retain: 2});// de bat
        }
    }
    if(digitalRead(14) == LOW)
    {
      while(digitalRead(14) == LOW);
      int statusLed = digitalRead(5);
      if(statusLed == HIGH)// dang sang
      {
        client.publish(topic_pub_copony,"4",{retain: 2});//de tat
        }
        else {
           client.publish(topic_pub_copony,"3",{retain: 2});// de bat
          }
      }
      if(digitalRead(12) == LOW)
      {
        while(digitalRead(12) == LOW);
        int statusLed = digitalRead(4);
        if(statusLed == HIGH)// dang sang
        {
          client.publish(topic_pub_light,"6",{retain: 2});//de tat
          }
          else {
             client.publish(topic_pub_light,"5",{retain: 2});// de bat
            }
        }
        
  client.loop();
}
