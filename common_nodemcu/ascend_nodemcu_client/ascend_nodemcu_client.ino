  #include "ESP8266WiFi.h"
#include "ESP8266HTTPClient.h"
#include <WiFiClientSecureBearSSL.h>
#include <ArduinoJson.h>

#define URL_MYUSER "https://192.168.1.10/api/myUser" 
#define URL_INSTRUCTION "https://192.168.1.10/workflow/instructions" 

struct SEC_TOKEN
{
  String xsrf;
  String jsse;
};

SEC_TOKEN tokens = {"", ""};
String response = "\0";

void setup() 
{
  Serial.begin(9600);
  connect_to_wifi();
  authenticate();
}

void loop() 
{
  long rssi = get_signal_strength();
  response = getInstructions(response);
  
//  Serial.print("Signal ");
//  Serial.println(rssi);

  Serial.print("ID: ");
  Serial.println(response);
  
  delay(1000);
}

void connect_to_wifi()
{
  const char* ssid = "sarmistha"; 
  const char* password = "Xs28@pr24";

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) 
  {
     delay(500);
     Serial.print(".");
  }

  Serial.println("WiFi connected");
  Serial.println(WiFi.localIP());
}


long get_signal_strength(){
  long rssi = 0;
  
  if (WiFi.status() == WL_CONNECTED)
  {
    rssi = WiFi.RSSI();
  }
  return rssi;
}

void authenticate()
{
  if (WiFi.status() == WL_CONNECTED)
  {
    std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
    
    // Ignore SSL certificate validation
    client->setInsecure();

    HTTPClient https;
    if (!https.begin(*client,URL_MYUSER))
    {
      Serial.println("Unable to connect");
      return;
    }
    
    https.setAuthorization("nodemcu", "accesscode::code::1234"); 
    const char *headers[1] = {"Set-Cookie"};
    https.collectHeaders(headers, 1);
    int code = https.GET();
    Serial.print("HTTP Response Code: ");
    Serial.println(code);
    
    if (code != -1)
    {
      
      String str_cookie = https.header("Set-Cookie");

      char *pnt_cookie = new char[str_cookie.length() + 1];
      strcpy(pnt_cookie,str_cookie.c_str());
      
      char *tok_cookie = strtok(pnt_cookie,";");
      String str_xsrf;
      String str_jse;
      String str_empty = "";
      while(tok_cookie != NULL)
      {
        if (strstr(tok_cookie,"XSRF-TOKEN"))
        {
          str_xsrf = tok_cookie;
          str_xsrf.replace("XSRF-TOKEN=","");
          tokens.xsrf = str_xsrf;
        }
        
        if (strstr(tok_cookie,"JSESSIONID"))
        {
          str_jse = tok_cookie;
          str_jse.replace(" Secure,JSESSIONID=","");
          tokens.jsse = str_jse;
        }
        
        tok_cookie = strtok(NULL,";");
      }

      Serial.println("Tokens: ");
      Serial.println(tokens.xsrf);
      Serial.println(tokens.jsse);
    }
  }
}

String getInstructions(String response)
{
    String instruction = "";
    String id = "";
    
    std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
    
    // Ignore SSL certificate validation
    client->setInsecure();

    HTTPClient https;
    if (!https.begin(*client,URL_INSTRUCTION))
    {
      Serial.println("Unable to connect");
      return instruction;
    }

    https.addHeader("Cookie", "JSESSIONID="+tokens.jsse+";XSRF-TOKEN="+tokens.xsrf);
    https.addHeader("X-XSRF-TOKEN", tokens.xsrf);
    https.addHeader("Content-Type", "application/json");
    https.addHeader("SiteId", "-1");

    String payLoad = ""; 
    if (response != "\0")
    {
      payLoad = "{'id': "+response+",'results': [{'instructionName': 'moveHand','result': '','isAdditionalCommand': False,'isScanned': False,'isScreenInput': False,'anchorWord': ''}]}";
    }
    else
    {
      payLoad = "";
    }
 
    int code = https.POST(payLoad);
    Serial.print("HTTP Response Code: ");
    Serial.println(code);

    if (code == 200)
    {
      String payload = https.getString();

      DynamicJsonDocument doc(payload.length());

      deserializeJson(doc,payload);
      if (doc.isNull()) {
        Serial.println(F("JSON Parsing failed!"));
        return instruction;
      }

      instruction = doc["instructions"][0]["prompt"].as<String>();
      Serial.println(instruction);
      id = doc["id"].as<String>();
    }

    return id;
}
