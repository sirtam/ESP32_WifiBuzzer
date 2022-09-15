/*
  Author Thomas Transeth
  Code not optimized for low power use
  
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

const int redTimer = 2000;
const int greenTimer = 10000;

bool buzzPressed = false;
unsigned long startTime;
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

String onAnswer(){
  highPin(GREENLIGHT_PIN);
  return "Ok";
}

void setup(){
  Serial.println("--Setup starting--");
  
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(REDLIGHT_PIN, OUTPUT);
  pinMode(GREENLIGHT_PIN, OUTPUT);
  
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
  //server.on("/response", HTTP_POST, [](AsyncWebServerRequest *request){
  server.on("/response", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", onAnswer().c_str());
  });
  // Start server
  server.begin();

  }

void loop(){
  if (digitalRead(BUTTON_PIN) == LOW) {
    buzzPressed = true;
    highPin(REDLIGHT_PIN);
    startTime = millis();
  }

  if (millis() - startTime >= redTimer) {
    lowPin(REDLIGHT_PIN);
  }
  if (millis() - startTime >= greenTimer) {
    lowPin(GREENLIGHT_PIN);
  }
}

void togglePin(int port) {
  digitalWrite(port, !digitalRead(port));
}

void lowPin(int port) {
  digitalWrite(port, LOW);
}

void highPin(int port) {
  digitalWrite(port, HIGH);
}
