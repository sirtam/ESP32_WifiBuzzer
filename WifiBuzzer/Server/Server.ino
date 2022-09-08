/*
  Thomas Transeth
  Based on code by Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp32-client-server-wi-fi/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*/


// Import required libraries
#include "WiFi.h"
#include "ESPAsyncWebServer.h"

#define BUTTON_PIN 21
#define REDLIGHT_PIN 18
#define GREENLIGHT_PIN 19

// Set your access point network credentials
const char* ssid = "ESP32-Access-Point";
const char* password = "12345678910";


bool buzzPressed = false;
unsigned long startTime;
bool waitForResponse;
String httpRequest;

AsyncWebServer server(80);

String readBuzz() {
  if (buzzPressed) {
      buzzPressed = false;
      return String(LOW);
  }
  else {
    return String(HIGH);
  }
}

String isResponse(AsyncWebServerRequest *request) {
    int paramsNr = request->params(); // number of params (e.g., 1)
    Serial.println(paramsNr);
    Serial.println();
    
    AsyncWebParameter * j = request->getParam(0); // 1st parameter
    Serial.print("Size: ");
    Serial.print(j->value());                     // value ^
    Serial.println();
}

void setup(){
  Serial.println("--Setup starting--");
  
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(REDLIGHT_PIN, OUTPUT);
  pinMode(GREENLIGHT_PIN, OUTPUT);

  waitForResponse = false;
  
  Serial.begin(115200);
  Serial.println();
  
  // Setting the ESP as an access point
  Serial.print("Setting AP (Access Point)…");
  // Remove the password parameter, if you want the AP (Access Point) to be open
  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);
  Serial.print("SSID: ");
  Serial.println(ssid);

  server.on("/buzz", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readBuzz().c_str());
  });
//  server.on("/response", HTTP_POST, [](AsyncWebServerRequest *request){
//    isResponse(request);
//    request->send(200);
//  });
    
  // Start server
  server.begin();

  }

void loop(){
  if (digitalRead(BUTTON_PIN) == LOW) {
    buzzPressed = true;
    waitForResponse = true;
    digitalWrite(REDLIGHT_PIN, HIGH);
    startTime = millis();
  }

  if (millis() - startTime >= 5000) {
    digitalWrite(REDLIGHT_PIN, LOW);
  }
}
