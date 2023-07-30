#include "ESP8266WiFi.h"



void setup() {
  Serial.begin(9600);
  
  connect_to_wifi();
}

void loop() {
  if (WiFi.status() == WL_CONNECTED){
    long rssi = WiFi.RSSI();
    Serial.print("Signal ");
    Serial.println(rssi);
  }

  delay(5000);

}

void connect_to_wifi(){
  const char* ssid = "sarmistha"; 
  const char* password = "Xs28@pr24";

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
     delay(500);
     Serial.print(".");
  }

  Serial.println("WiFi connected");
  Serial.println(WiFi.localIP());
}
