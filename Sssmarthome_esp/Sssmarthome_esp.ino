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
char msgIN;

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
char msg[50];
char point1Msg[50];
int value = 0;
int templow;
int temphigh;

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

payload[length] = '\0';
  String s = String((char*)payload);
  if (s == "ballpython") {
  Serial.println("Applying settings for ballpython, templow 28°C, temphigh 30°C");
  templow = 28;
  temphigh = 30; 
  }
  else if (s == "dragon") {
  Serial.println("Applying settings for dragon, templow 24°C, temphigh 29°C");
  templow = 24;
  temphigh = 29; 
}
  else if (s == "boa") {
  Serial.println("Applying settings for boa, templow 27°C, temphigh 29°C");
  templow = 27;
  temphigh = 29; 
}
else if (s == "chameleon") {
  Serial.println("Applying settings for chameleon, templow 25°C, temphigh 27°C");
  templow = 25;
  temphigh = 27; 
}
else if (s == "cornsnake") {
  Serial.println("Applying settings for cornsnake, templow 25°C, temphigh 27°C");
  templow = 25;
  temphigh = 27; 
}
else if (s == "gtp") {
  Serial.println("Applying settings for gtp, templow 26°C, temphigh 29°C");
  templow = 26;
  temphigh = 29; 
}
else if (s == "hognose") {
  Serial.println("Applying settings for hognose, templow 25°C, temphigh 27°C");
  templow = 25;
  temphigh = 27; 
}
else if (s == "kingsnake") {
  Serial.println("Applying settings for kingsnake, templow 26°C, temphigh 29°C");
  templow = 26;
  temphigh = 29; 
}
else if (s == "gecko") {
  Serial.println("Applying settings for gecko, templow 26°C, temphigh 28°C");
  templow = 26;
  temphigh = 28; 
}
else if (s == "rtortoise") {
  Serial.println("Applying settings for russian tortoise, templow 22°C, temphigh 30°C");
  templow = 22;
  temphigh = 30; 
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
      client.publish("ssshome/animal/", "I'm Connected");
      // ... and resubscribe
      client.subscribe("ssshome/animal/#");
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
   pinMode(12, OUTPUT); //Heating lamp
   pinMode(13, OUTPUT); //Fan
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


    if (dht.readTemperature() < templow){ 
      digitalWrite(LED_BUILTIN, LOW);  // Turn the LED off by making the voltage HIGH
      digitalWrite(12, HIGH);
      Serial.println("Temperature is too low - adjusting..");
      delay(1000);
}
    else if (dht.readTemperature() > temphigh){
      digitalWrite(12, LOW);
      digitalWrite(LED_BUILTIN, HIGH);
      Serial.println("Temperature is too high - adjusting..");
      delay(1000);
  }

  

}
