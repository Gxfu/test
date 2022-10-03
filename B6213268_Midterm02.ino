#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
WiFiClient client;
PubSubClient mqtt(client);
DynamicJsonDocument docs(1024); // doc for serialize
DynamicJsonDocument docd(1024); // doc for de-serialize
char output[128];
#define WIFI_STA_NAME "@SUT-IoT"
#define WIFI_STA_PASS ""
#define MQTT_SERVER   "electsut.trueddns.com"
#define MQTT_PORT     27861
#define MQTT_USERNAME "giftgift1" //Token
#define MQTT_PASSWORD ""
#define MQTT_NAME     "Giffu"
#define LED_PIN 23
#define sw 5

char out1[256];
char out3[256];
int Num = 0;
void callback(char* topic, byte* payload, unsigned int length) {
std::string TP=topic;
deserializeJson(docd, payload, length);
serializeJsonPretty(docd,out3);
const char* swid = docd["method"];
int swst = docd["params"];
if(String(swid)=="SW1"){
  if(swst == 1){
      digitalWrite(18,HIGH);
    }
  if(swst == 0){
      digitalWrite(18,LOW);
    }
}
}
void setup() {
Serial.begin(115200);
pinMode(LED_BUILTIN, OUTPUT);
pinMode(18, OUTPUT);
pinMode(19, OUTPUT);
Serial.println();
Serial.println();
Serial.print("Connecting to ");
Serial.println(WIFI_STA_NAME);
WiFi.mode(WIFI_STA);
WiFi.begin(WIFI_STA_NAME, WIFI_STA_PASS);
while (WiFi.status() != WL_CONNECTED) {
  delay(500);
  Serial.print(".");
  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
}
digitalWrite(LED_BUILTIN, HIGH);
Serial.println("");
Serial.println("WiFi connected");
Serial.println("IP address: ");
Serial.println(WiFi.localIP());
mqtt.setServer(MQTT_SERVER, MQTT_PORT);
mqtt.setCallback(callback);
}
void loop() {
if (mqtt.connected() == false) {
  Serial.print("MQTT connection... ");
    if (mqtt.connect(MQTT_NAME, MQTT_USERNAME, MQTT_PASSWORD)) {
      Serial.println("connected");
      mqtt.subscribe("v1/devices/me/rpc/request/+"); //เป็นการSupข้อมูลที่มาจากthingsboard
//      mqtt.subscribe("B6213268/test/num1");
  } else {
      Serial.println("failed");
      delay(1000);
  }
} else {
    mqtt.loop();
    int val = analogRead(33);
//    Serial.println(val);
    int Temp = random(20,30);
    int Humid = random(60,70);
    int wind = random(1500,1600);
    docs["Energy1"]=val;
    docs["Temp"]=Temp; 
    docs["Humid"]=Humid; 
    docs["Wind1rpm"]=wind;  
    serializeJson(docs,out1);
    mqtt.publish("v1/devices/me/telemetry", out1);
    delay(2000);
}
}
