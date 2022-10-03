#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
WiFiClient client;
PubSubClient mqtt(client);
DynamicJsonDocument docs(1024); // doc for serialize
DynamicJsonDocument docs2(1024);
DynamicJsonDocument docs3(1024);
DynamicJsonDocument docs4(1024);
DynamicJsonDocument docs5(1024);
DynamicJsonDocument docd(1024); // doc for de-serialize
char output[256];
char output2[256];
char output3[256];
char output4[256];
char output5[256];
#define WIFI_STA_NAME "@SUT-IoT"
#define WIFI_STA_PASS ""
//#define WIFI_STA_NAME "Gift"
//#define WIFI_STA_PASS "0644364428"
#define MQTT_SERVER   "electsut.trueddns.com"
#define MQTT_PORT     27861
#define MQTT_USERNAME "giftgift1" //Token
#define MQTT_PASSWORD ""
#define MQTT_NAME     "Giffu"
#define LED_PIN 23
#define sw 5

#define button1 5
#define button2 18
#define button3 19
#define buttonRe 21
#define Led1 22
#define Led2 23
#define Led11 35
#define Led12 34

int Num =0;
int Tank01 = 3000; 
int Tank02 = 3000; 
int Tank03 = 3000; 
String sta_Sola = "OFF";
String sta_A = "Normal";
String sta_B = "Normal";
String sta_C = "Normal";
int Solar = 0;
float BT_sl = 0;
void callback(char* topic, byte* payload, unsigned int length) {
deserializeJson(docd, payload, length);
serializeJsonPretty(docd,Serial);
const char* swid = docd["method"];
int swst = docd["params"];
Serial.print(swid);
Serial.println(swst);
if(String(swid)=="SolarSW"){
  Solar = swst;
  if ( swst == 1){
    digitalWrite(Led1, HIGH);
    }
  if ( swst == 0){
    digitalWrite(Led1, LOW);
    }
}
const char* fan = docd["method"];
int swst1 = docd["params"];
if(String(fan)=="SW2"){
    ledcWrite(1 , swst1);
    ledcWrite(2 , swst1);
}
if(String(swid)=="SW1"){
  Serial.println(swid);
}
if(String(swid)=="num"){
  Num = swst*100;
  Serial.println("Num :: "+Num);
}
}
void setup() {
Serial.begin(115200);
ledcSetup(Led11, 5000, 10);
ledcAttachPin(Led11, 1);
ledcSetup(Led12, 5000, 10);
ledcAttachPin(Led12, 2);
pinMode(button1, OUTPUT);
pinMode(button2, OUTPUT);
pinMode(button3, OUTPUT);
pinMode(buttonRe, OUTPUT);
pinMode(Led1, OUTPUT);
pinMode(Led2, OUTPUT);
pinMode(Led12, OUTPUT);
pinMode(LED_BUILTIN, OUTPUT);
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
  } else {
      Serial.println("failed");
      delay(1000);
  }
} else {
    mqtt.loop();
    ledcWrite(Led11, 200);
    if (digitalRead(button1) == HIGH){
      Tank01 = Tank01 - 500;
    }
    if (digitalRead(button2) == HIGH){
      Tank02 = Tank02 - 500;
    }
    if (digitalRead(button3) == HIGH){
      Tank03 = Tank03 - 500;
    }
    if (digitalRead(buttonRe) == HIGH){
      Tank03 = 3000;
      Tank02 = 3000;
      Tank01 = 3000;
    }
    if (Tank01 <= 0 ){
      Tank01 = 0;
      }
    if (Tank02 <= 0 ){
      Tank02 = 0;
      }
    if (Tank03 <= 0 ){
      Tank03 = 0;
      }
    if (Solar == 1 ){
      BT_sl = BT_sl + 0.5 ;
      sta_Sola = "ON";
      }
    if (Solar == 0){
      BT_sl = BT_sl - 0.5;
      sta_Sola = "OFF";
      }
    if (BT_sl < 100){
      digitalWrite(Led2, LOW);
    }
    if (BT_sl >= 100){
      BT_sl = 100;
      digitalWrite(Led2, HIGH);
      }
    if (BT_sl <= 0){
      BT_sl = 0;
      }
    if (Num > 0){
      digitalWrite(Led11, Num);
      Serial.println(Num);
      } 
    int x01 = random(16,28);
    int x02 = random(15,27);
    int x03 = random(16,28);
    int x04 = random(15,27);
    int y01 = random(65,71);
    int y02 = random(64,70);
    int y03 = random(65,71);
    int y04 = random(64,70);
    int TempGH01 = x01;
    int TempGH02 = x02;
    int TempGH03 = x03;
    int TempGH04 = x04;
    int TempGH05 = x01 - 1;
    int TempGH06 = x02 - 1;
    int TempGH07 = x03 - 1;
    int TempGH08 = x04 - 1;
    int HumidGH01 = y01;
    int HumidGH02 = y02;
    int HumidGH03 = y03;
    int HumidGH04 = y04;
    int HumidGH05 = y01 -1;
    int HumidGH06 = y02 -1;
    int HumidGH07 = y03 -1;
    int HumidGH08 = y04 -1;
    int HumidA = random(85,91); //85-90%
    int HumidB = random(83,90); //85-90%
    int HumidC = random(84,92); //85-90%
    if (HumidA < 85){
      sta_A = "Low Humidity!!!";
      }
    if (HumidB < 85){
      sta_B = "Low Humidity!!!";
      }
    if (HumidC < 85){
      sta_C = "Low Humidity!!!";
      }
    if (HumidA > 89){
      sta_A = "Hight Humidity!!!";
      }
    if (HumidB > 89){
      sta_B = "Hight Humidity!!!";
      }
    if (HumidC > 89){
      sta_C = "Hight Humidity!!!";
      } 
    if (HumidA >= 85 and HumidA <=89){
      sta_A = "Normal";
      } 
    if (HumidB >= 85 and HumidB <=89){
      sta_B = "Normal";
      }
    if (HumidC >= 85 and HumidC <=89){
      sta_C = "Normal";
      } 
      
    docs["TempGH01"]=TempGH01;
    docs["TempGH02"]=TempGH02;
    docs["TempGH03"]=TempGH03;
    docs["TempGH04"]=TempGH04;
    docs["TempGH05"]=TempGH05;
    docs["TempGH06"]=TempGH06;
    docs["TempGH07"]=TempGH07;
    docs["TempGH08"]=TempGH08;
    docs2["HumidGH01"]=HumidGH01;
    docs2["HumidGH02"]=HumidGH02;
    docs2["HumidGH03"]=HumidGH03;
    docs2["HumidGH04"]=HumidGH04;
    docs2["HumidGH05"]=HumidGH05;
    docs2["HumidGH06"]=HumidGH06;
    docs2["HumidGH07"]=HumidGH07;
    docs2["HumidGH08"]=HumidGH08;
    docs3["HumidA"]=HumidA;
    docs3["HumidB"]=HumidB;
    docs3["HumidC"]=HumidC;
    docs3["Tank01"]=Tank01;
    docs3["Tank02"]=Tank02;
    docs3["Tank03"]=Tank03;
    docs4["SSolar"]=sta_Sola;
    docs4["SWSolar"]=Solar;
    docs4["Battery"]=BT_sl;
    docs5["stA"]=sta_A;
    docs5["stB"]=sta_B;  
    docs5["stC"]=sta_C;    
    
    serializeJson(docs,output);
    serializeJson(docs2,output2);
    serializeJson(docs3,output3);
    serializeJson(docs4,output4);
    serializeJson(docs5,output5);
    mqtt.publish("v1/devices/me/telemetry", output);
    mqtt.publish("v1/devices/me/telemetry", output2);
    mqtt.publish("v1/devices/me/telemetry", output3);
    mqtt.publish("v1/devices/me/telemetry", output4);
    mqtt.publish("v1/devices/me/telemetry", output5);
    delay(1000);
}
}
//hello

