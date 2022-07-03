#include <Arduino.h>
#include "ESP8266WiFi.h"

#define WIFI_NAME "iBall-Baton"
#define WIFI_PASSWORD "8450975955"
#define LED D7
void wifiSetup(const char* wifi_name , const char* wifi_password){

  WiFi.mode(WIFI_STA);
  WiFi.begin(wifi_name,wifi_password);

  while(WiFi.status() != WL_CONNECTED ){
    Serial.print(".");
    delay(100);
  }

  if(WiFi.status() == WL_CONNECTED){
    Serial.print("\n Connected to : ");
    Serial.print( wifi_name );
    Serial.print("\t local IP : ");
    Serial.println( WiFi.localIP());
  }
  else{
    Serial.print("Failed :(");
  }
}
void setup() {
  pinMode(D7 , OUTPUT);
  Serial.begin(9600);
  wifiSetup(WIFI_NAME,WIFI_PASSWORD);
}

void loop() {

  if(WiFi.status() == WL_CONNECTED){
    digitalWrite(D7 , HIGH);
  }


}