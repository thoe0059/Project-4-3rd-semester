//Import libraries
#include <ESP8266WiFi.h>
#include <PubSubClient.h> 
#include <DHT.h>

DHT dht(14, DHT22); // Initialize DHT sensor DHT22
const char* ssid = "ITEK 3rd";  //Wifi name
const char* password = "roedgroedmedfloede";  //Wi-Fi Password
const char* mqtt_server = "broker.mqtt-dashboard.com";
String mqttTopicTemp, mqttTopicHum;
char mqttTopicTempChar[80];
char mqttTopicHumChar[80];
char msgIN; // For converting byte to char.

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
char msg[50];
char point1Msg[50];
int value = 0;
float templow;
float temphigh;
float humlow;
float humhigh;

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

payload[length] = '\0'; //Convert payload byte to string.
  String s = String((char*)payload);

 //Compare value of string to preconfigured animal species.
  if (s == "ballpython") {
  Serial.println("Applying settings for ballpython, tlow 28°C, thi 30°C, hLow 50, hHi 75");
  templow = 28;
  temphigh = 30; 
  humlow = 50;
  humhigh = 75;
  }
  else if (s == "dragon") {
  Serial.println("Applying settings for dragon, tlow 24°C, thi 29°C, hLow 50, hHi 75");
  templow = 24;
  temphigh = 29; 
  humlow = 25;
  humhigh = 40;
}
  else if (s == "boa") {
  Serial.println("Applying settings for boa, tlow 27°C, thi 29°C, hLow 50, hHi 75");
  templow = 27;
  temphigh = 29;
  humlow = 55;
  humhigh = 75; 
}
else if (s == "chameleon") {
  Serial.println("Applying settings for chameleon, tlow 25°C, thi 27°C, hLow 50, hHi 75");
  templow = 25;
  temphigh = 27; 
  humlow = 65;
  humhigh = 80;
}
else if (s == "cornsnake") {
  Serial.println("Applying settings for cornsnake, tlow 25°C, thi 27°C, hLow 50, hHi 75");
  templow = 25;
  temphigh = 27; 
  humlow = 40;
  humhigh = 50;
}
else if (s == "gtp") {
  Serial.println("Applying settings for gtp, tlow 26°C, thi 29°C, hLow 50, hHi 75");
  templow = 26;
  temphigh = 29; 
  humlow = 50;
  humhigh = 80;
}
else if (s == "hognose") {
  Serial.println("Applying settings for hognose, tlow 25°C, thi 27°C, hLow 50, hHi 75");
  templow = 25;
  temphigh = 27; 
  humlow = 30;
  humhigh = 65;
}
else if (s == "kingsnake") {
  Serial.println("Applying settings for kingsnake, tlow 26°C, thi 29°C, hLow 50, hHi 75");
  templow = 26;
  temphigh = 29; 
  humlow = 35;
  humhigh = 60;
}
else if (s == "gecko") {
  Serial.println("Applying settings for gecko, tLow 26°C, tHi 28°C, hLow 50, hHi 75");
  templow = 26;
  temphigh = 28; 
  humlow = 30;
  humhigh = 40;
}
else if (s == "rtortoise") {
  Serial.println("Applying settings for russian tortoise, tLow 22°C, tHi 30°C, hLow 50, hHi 75");
  templow = 22;
  temphigh = 30; 
  humlow = 40;
  humhigh = 60;
}
else if (s == "custom") {

Serial.println("Applying settings for custom animal");
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
      client.subscribe("ssshome/animal/");
//      client.subscribe("ssshome/custom/HL");
//      client.subscribe("ssshome/custom/HH");
//      client.subscribe("ssshome/custom/TL");
//      client.subscribe("ssshome/custom/TH");
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
  
 //-----Control for temperature ranges-----

    if (dht.readTemperature() < templow){ 
      digitalWrite(LED_BUILTIN, LOW);  // Turn the LED off by making the voltage HIGH
      digitalWrite(12, HIGH);
      Serial.println("Temperature is too low - adjusting..");
      client.publish("ssshome/warning/", "Temperature too low - adjusting.."); 
      delay(1000);
}
    else if (dht.readTemperature() > temphigh){
      digitalWrite(12, LOW);
      digitalWrite(LED_BUILTIN, HIGH);
      Serial.println("Temperature is too high - adjusting..");
      client.publish("ssshome/warning/", "Temperature too high - adjusting..");
      delay(1000);
  }
  
  //-----Control for humidity ranges------
  
  if (dht.readHumidity() < humlow){ 
      digitalWrite(13, LOW);
      Serial.println("Humidity is too low - make it rain..");
      client.publish("ssshome/warning/", "Humidity too low - please raise it");
      delay(1000);
}
    else if (dht.readHumidity() > humhigh){
      digitalWrite(13, HIGH);
      Serial.println("Humidity is too high - adjusting..");
      client.publish("ssshome/warning/", "Humidity too high - adjusting..");
      delay(1000);
  }

  

}
