//Import libraries
#include <ESP8266WiFi.h>
#include <PubSubClient.h> 
#include <DHT.h>

DHT dht(14, DHT22);              // Initialize DHT sensor DHT22
const char* ssid = "ITEK 3rd";         // Update the value suitable for your network
const char* password = "roedgroedmedfloede";             // Update the value suitable for your network
const char* mqtt_server = "broker.mqtt-dashboard.com";
String mqttTopicTemp, mqttTopicHum;
char mqttTopicTempChar[80];
char mqttTopicHumChar[80];

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
char msg[50];
char point1Msg[50];
int value = 0;

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1') {
    digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is active low on the ESP-01)
  } else {
    digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
  }

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("ssshome/esp", "I'm Connected");
      // ... and resubscribe
      client.subscribe("ssshome/esp");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  // put your setup code here, to run once:
   pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
   pinMode(12, OUTPUT);
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  dht.begin(); // Begin reading sensor data from DHT.


mqttTopicTemp = "ssshome/livedata/temp";
mqttTopicHum = "ssshome/livedata/hum";

mqttTopicTemp.toCharArray(mqttTopicTempChar, 80);
mqttTopicHum.toCharArray(mqttTopicHumChar, 80);

}
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  long now = millis();
  if (now - lastMsg > 2000) {
    lastMsg = now;
    ++value;
    Serial.print("Publishing live data: ");
    
    delay(1000);
    
    snprintf (point1Msg, 50, "%.1f", dht.readTemperature());
    Serial.print("Temperature °C: ");
    Serial.println(point1Msg);
    client.publish(mqttTopicTempChar, point1Msg); // EL/BAAA/School/2/A108/temp
    
    delay(50);
    
    snprintf (point1Msg, 50, "%.1f", dht.readHumidity());
    Serial.print("Humidity %: ");
    Serial.println(point1Msg);
    client.publish(mqttTopicHumChar, point1Msg);
  }
    if (dht.readTemperature() < 25){ 
      digitalWrite(LED_BUILTIN, LOW);  // Turn the LED off by making the voltage HIGH
      digitalWrite(12, HIGH);
}
    else if (dht.readTemperature() > 25){
      digitalWrite(12, LOW);
      digitalWrite(LED_BUILTIN, HIGH);
      Serial.println("Temperature is too high - adjusting it now");
      delay(1000);
  }

}
