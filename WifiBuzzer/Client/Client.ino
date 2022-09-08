/*
  Thomas Transeth
  Based on code by Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp32-client-server-wi-fi/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*/

#include <WiFi.h>
#include <HTTPClient.h>

#define BUTTON_PIN 23
#define LED_PIN 22
#define BEEP_PIN 16

const char* server = "http://192.168.4.1";
const char* ssid = "ESP32-Access-Point";
const char* password = "12345678910";


//Your IP address or domain name with URL path
const char* serverNameBuzz = server + "/buzz";
const char* serverNameResponse = server + "/response";


String buzz;
bool doBuzz = false;
bool isResponded = false;

unsigned long previousMillis = 0;
const long interval = 1000; 

void setup() {
  Serial.begin(115200);

  pinMode(LED_PIN, OUTPUT);
  pinMode(BEEP_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) { 
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  unsigned long currentMillis = millis();
  
  if(currentMillis - previousMillis >= interval) {
     // Check WiFi connection status
    if(WiFi.status()== WL_CONNECTED ){ 
      
      buzz = httpGETRequest(serverNameBuzz);
      if (buzz == "0") {
        doBuzz = true;
      }

      if(digitalRead(BUTTON_PIN) == LOW) {
        doBuzz = false;
        isResponded = true;
      }


      if(doBuzz) {
        digitalWrite(LED_PIN, HIGH); // Set GPIO22 active high
        digitalWrite(BEEP_PIN, HIGH);
        delay(1000);  // delay of one second
        digitalWrite(LED_PIN, LOW); // Set GPIO22 active low
        digitalWrite(BEEP_PIN, LOW);
        //delay(1000); // delay of one second
      }
      // save the last HTTP GET Request
      previousMillis = currentMillis;

      if(isResponded) {
        //String post = httpPOSTRequest(serverNameResponse);
        isResponded = false;
      }
      
    }
    else {
      Serial.println("WiFi Disconnected");
    }
  }
}

String httpGETRequest(const char* serverName) {
  WiFiClient client;
  HTTPClient http;
    
  // Your Domain name with URL path or IP address with path
  http.begin(client, serverName);
  
  // Send HTTP POST request
  int httpResponseCode = http.GET();
  
  String payload = "--"; 
  
  if (httpResponseCode>0) {
    //Serial.print("HTTP Response code: ");
    //Serial.println(httpResponseCode);
    
    payload = http.getString();
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();

  return payload;
}

String httpPOSTRequest(const char* serverName) {
  
  WiFiClient client;
  HTTPClient http;
    
  http.begin(client, serverName);
  
  http.addHeader("Content-Type", "text/plain");
  int httpResponseCode = http.POST("1");

  String payload = http.getString();
  http.end();
  
  return payload;
}
