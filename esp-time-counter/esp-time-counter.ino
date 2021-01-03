#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <LittleFS.h>

const char* ssid = "TimeCounter";
const char* password = "";

const char* counterFileName = "/counter";

ESP8266WebServer webServer(80);

const long interval = 10000;
unsigned long previousMillis = 0;
unsigned long previousCount = 0;
unsigned long currentCount = 0;

void handleRoot() {
  // Return the number of seconds elapsed since the first power on
  digitalWrite(LED_BUILTIN, 0);
  Serial.println("HTTP request");
  webServer.send(200, "text/plain", String(currentCount));
  digitalWrite(LED_BUILTIN, 1);
}

void loadCounter() {
  File file = LittleFS.open(counterFileName, "r");
  if (!file)
    Serial.println("Failed to open counter file for reading");
  else {
    String str = file.readString();
    previousCount = strtoull(str.c_str(), nullptr, 0);
    Serial.print("Loaded counter value: ");
    Serial.println(previousCount);
    file.close();
  }
}

void saveCounter() {
  File file = LittleFS.open(counterFileName, "w");
  if (!file)
    Serial.println("Failed to open counter file for writing");
  else {
    file.println(currentCount);
    file.close();
  }
}

void setup() {
  // Setup LED
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, 1);

  // Setup serial
  Serial.begin(74880);
  Serial.println();

  // Initialize LittleFS
  if (!LittleFS.begin())
    Serial.println("Failed to initialize LittleFS");

  // Format LittleFS
  //LittleFS.format();

  // Delete counter file
  //LittleFS.remove(counterFileName);
  
  // Read counter value from the file
  loadCounter();
  
  // Setup WiFi AP
  WiFi.softAP(ssid, password);
  IPAddress apIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");  
  Serial.println(apIP);

  // Setup web server
  webServer.on("/", handleRoot);
  webServer.begin();   
}

void loop() {
  webServer.handleClient();
  
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {    
    digitalWrite(LED_BUILTIN, 0);

    // Calculate total seconds count
    currentCount =  previousCount + currentMillis / 1000;    
    Serial.println(currentCount);

    // Write current value to the SPI flash
    saveCounter();
    
    delay(10);
    previousMillis = currentMillis;
    digitalWrite(LED_BUILTIN, 1);   
  }
}
