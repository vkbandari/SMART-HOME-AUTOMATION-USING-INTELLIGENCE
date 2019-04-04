#include <ESP8266WiFi.h>
#include <PubSubClient.h>
 
//-------- Customise these values -----------
const char* ssid = "ECE1";
const char* password = "ece1@1234";

#include "DHT.h"
#define DHTPIN D1    // what pin we're connected to
#define DHTTYPE DHT11   // define type of sensor DHT 11
DHT dht (DHTPIN, DHTTYPE);
 String command;
#define ORG "6ea444"
#define DEVICE_TYPE "auto1"
#define DEVICE_ID "home"
#define TOKEN "y@w(Wm(8cKIt-n8?si"
//-------- Customise the above values --------,
 
char server[] = ORG ".messaging.internetofthings.ibmcloud.com";
char topic[] = "iot-2/evt/data/fmt/json";
char subtopic[] = "iot-2/cmd/home/fmt/String";
char authMethod[] = "use-token-auth";
char token[] = TOKEN;
char clientId[] = "d:" ORG ":" DEVICE_TYPE ":" DEVICE_ID;
 
WiFiClient wifiClient;
PubSubClient client(server, 1883,wifiClient);
int sensorPin=D4;
int sensorValue=0;
int led=D1;
int Status=D2;
int sensor=D3;
void setup() {
 Serial.begin(115200);
 Serial.println();
 pinMode(D1,OUTPUT);
  pinMode(D6,OUTPUT);
   pinMode(D7,OUTPUT);
    pinMode(D8,OUTPUT);
 pinMode(sensorPin,INPUT);
  pinMode(sensor,INPUT);
  pinMode(Status,OUTPUT);
 dht.begin();
 Serial.print("Connecting to ");
 Serial.print(ssid);
 WiFi.begin(ssid, password);
 while (WiFi.status() != WL_CONNECTED) {
 delay(500);
 Serial.print(".");
 } 
 Serial.println("");
 
 Serial.print("WiFi connected, IP address: ");
 Serial.println(WiFi.localIP());
}
 
void loop() {
 if (!client.loop()) {
    mqttConnect();
  }
  delay(1000);
sensorValue=digitalRead(sensorPin);
  Serial.println(sensorValue);
  if (sensorValue<1)
  {
      Serial.println("fire detected");
      Serial.println("Led on");
      digitalWrite(D8,LOW);
    // delay(1000);
  }
  else{
   Serial.println("fire  not detected");
   Serial.println("Led off");
  digitalWrite(D8,HIGH);
  //delay(1000);
  }
  int state=digitalRead(sensor);
  Serial.println(state);
    if(state==HIGH){
      digitalWrite(D7,LOW);
      Serial.println("Motion detected");
     // delay(1000);
    }
    else {
      digitalWrite(D7,HIGH);
      Serial.println("MOTION absent");
      //delay(1000);
    }
   // delay(1000);

 
  

  
float h = dht.readHumidity();
float t = dht.readTemperature();

if (isnan(h) || isnan(t))
{
Serial.println("Failed to read from DHT sensor!");
delay(1000);
return;
}
PublishData(t,h,sensor,state);
Serial.println("Temperature:");
Serial.println(t);
 if (t<25)
  {
    
    digitalWrite(D6,LOW);
  }
  else
  {
      digitalWrite(D6,HIGH);
  }
 
delay(1000);
}

void mqttConnect() {
  if (!client.connected()) {
    Serial.print("Reconnecting MQTT client to "); Serial.println(server);
    while (!client.connect(clientId, authMethod, token)) {
      Serial.print(".");
      delay(500);
    }
    initManagedDevice();
    Serial.println();
  }
}
void initManagedDevice() {
  if (client.subscribe(subtopic)) {
  }
}



void PublishData(float temperature, float humidity,int sesnor,int sate){
 if (!!!client.connected()) {
 Serial.print("Reconnecting client to ");
 Serial.println(server);
 while (!!!client.connect(clientId, authMethod, token)) {
 Serial.print(".");
 delay(500);
 }
 Serial.println();
 }
  String payload = "{\"d\":{\"Temperature\":";
  payload += temperature;
  payload+="," "\"Humidity\":";
  payload += humidity;
  payload+="," "\"Sensor\":";
  payload += sesnor;
  payload+="," "\"state\":";
  payload += sate;
  payload += "}}";
 Serial.print("Sending payload: ");
 Serial.println(payload);
  
 if (client.publish(topic, (char*) payload.c_str())) {
 Serial.println("Publish ok");
 } else {
 Serial.println("Publish failed");
 }
}
void callback(char* subtopic, byte* payload, unsigned int payloadLength) {
  Serial.print("callback invoked for topic: "); Serial.println(subtopic);

  for (int i = 0; i < payloadLength; i++) {
    //Serial.println((char)payload[i]);
    command += (char)payload[i];
  }
Serial.println(command);
if(command == "LIGHTON"){
  digitalWrite(D0,LOW);
  Serial.println("Light is Switched ON");
}
if(command == "LIGHTOFF"){
  digitalWrite(D0,HIGH);
  Serial.println("Light is Switched OFF");
}
command ="";
}
