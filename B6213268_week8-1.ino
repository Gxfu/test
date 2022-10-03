#if defined(ESP32)
  #include <WiFiMulti.h>
  WiFiMulti wifiMulti;
  #define DEVICE "ESP32"
  #elif defined(ESP8266)
  #include <ESP8266WiFiMulti.h>
  ESP8266WiFiMulti wifiMulti;
  #define DEVICE "ESP8266"
  #endif
  
  #include <InfluxDbClient.h>
  #include <InfluxDbCloud.h>
  
  // WiFi AP SSID
  #define WIFI_SSID "Gift"
  // WiFi password
  #define WIFI_PASSWORD "0644364428"
  
  #define INFLUXDB_URL "https://europe-west1-1.gcp.cloud2.influxdata.com"
  #define INFLUXDB_TOKEN "ez13wyDc57QY6rGKXCDqCVf6ozzwKKhyuELKUFqML4WfdQw1YXGOb-pMsUts4bGvPna8fkDGb9hoPZKItPb7Mg=="
  #define INFLUXDB_ORG "6960219b8bd3229b"
  #define INFLUXDB_BUCKET "wannida"
  
  // Time zone info
  #define TZ_INFO "UTC7"
  
  // Declare InfluxDB client instance with preconfigured InfluxCloud certificate
  InfluxDBClient client(INFLUXDB_URL, INFLUXDB_ORG, INFLUXDB_BUCKET, INFLUXDB_TOKEN, InfluxDbCloud2CACert);
  
  // Declare Data point
  Point sensor("wifi_status");
  void setup() {
    Serial.begin(115200);
  
    // Setup wifi
    WiFi.mode(WIFI_STA);
    wifiMulti.addAP(WIFI_SSID, WIFI_PASSWORD);
  
    Serial.print("Connecting to wifi");
    while (wifiMulti.run() != WL_CONNECTED) {
      Serial.print(".");
      delay(100);
    sensor.addTag("device", DEVICE);
    sensor.addTag("SSID", WiFi.SSID());
    }
    Serial.println();
  
    // Accurate time is necessary for certificate validation and writing in batches
    // We use the NTP servers in your area as provided by: https://www.pool.ntp.org/zone/
    // Syncing progress and the time will be printed to Serial.
    timeSync(TZ_INFO, "pool.ntp.org", "time.nis.gov");
  
  
    // Check server connection
    if (client.validateConnection()) {
      Serial.print("Connected to InfluxDB: ");
      Serial.println(client.getServerUrl());
    } else {
      Serial.print("InfluxDB connection failed: ");
      Serial.println(client.getLastErrorMessage());
    }
  }
  void loop() {
    // Clear fields for reusing the point. Tags will remain the same as set above.
    sensor.clearFields();
    float num = random(1.0,20.0);
    float num2 = random(30.0,50.0);
    float num3 = random(1.0,20.0);
    float num4 = random(30.0,50.0);
  
    // Store measured value into point
    // Report RSSI of currently connected network
    sensor.addField("rssi", WiFi.RSSI());
    sensor.addField("Name", "Wannida Juhngee");
    sensor.addField("Humid", num2);
    sensor.addField("Temp", num);
    sensor.addField("Humid2", num4);
    sensor.addField("Temp2", num3);
  
    // Print what are we exactly writing
    Serial.print("Writing: ");
    Serial.println(sensor.toLineProtocol());
  
    // Check WiFi connection and reconnect if needed
    if (wifiMulti.run() != WL_CONNECTED) {
      Serial.println("Wifi connection lost");
    }
  
    // Write point
    if (!client.writePoint(sensor)) {
      Serial.print("InfluxDB write failed: ");
      Serial.println(client.getLastErrorMessage());
    }
  
    Serial.println("Waiting 1 second");
    delay(5000);
    }
