#include <ESP8266WiFi.h>
#include <ESP8266Ping.h>
#define LED_Green 12
#define LED_Red 16
const char* ssid     = "Network Name";
const char* password = "Password";
const char* remote_host = "tropicalengineer.com";
void setup() {
  pinMode(LED_Green, OUTPUT);
  pinMode(LED_Red, OUTPUT);
  
  Serial.begin(9600);
  delay(10);
  Serial.println("");
  Serial.println("");
  Serial.println("Connecting to WiFi");
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    
    delay(100);
    Serial.print(".");
    
  }
  Serial.println("");
  Serial.print("Local IP Address: ");  
  Serial.print(WiFi.localIP());
  Serial.println("");
  Serial.print("Pinging: ");
  Serial.print(remote_host);
  Serial.println("");
  Serial.println("");
}
void loop() { 
  
    if(Ping.ping(remote_host)) {
    Serial.println("Online");
    delay(1000);
    digitalWrite(LED_Green, HIGH);
    digitalWrite(LED_Red, LOW);
    
  } else {
    
    Serial.println("Offline");
    delay(1000);
    digitalWrite(LED_Green, LOW);
    digitalWrite(LED_Red, HIGH);
    
  }
}
