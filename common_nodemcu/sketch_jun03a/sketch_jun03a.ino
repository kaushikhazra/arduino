#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

const char* ssid = "your_wifi_ssid";
const char* password = "your_wifi_password";

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
}

void loop() {
  HTTPClient http;

  http.begin("https://192.168.1.10/api/myUser", "CA:3F:5A:6A:2D:5F:5A:F2:A3:9C:FA:F2:BD:7F:18:1E:79:6C:4C:65:61:63:DC:37:4A:2E:20:53:C0:17:C4"); // Replace with your server certificate fingerprint
  http.setAuthorization("user3", "accesscode::code::1234");

  int httpCode = http.GET();

  if (httpCode == HTTP_CODE_OK) {
    String response = http.getString();
    Serial.println(response);
    String xsrf;

    for (int i = 0; i < http.cookies(); ++i) {
      if (http.getCookieName(i) == "XSRF-TOKEN") {
        xsrf = http.getCookieValue(i);
        break;
      }
    }

    String cookies;

    for (int i = 0; i < http.cookies(); ++i) {
      cookies += http.getCookieName(i) + "=" + http.getCookieValue(i) + "; ";
    }

    Serial.println(http.headers());

    while (true) {
      String user_inputs;

      // Get user input for each instruction
      // Replace the input() function with your own implementation for getting user input

      String result = "{\"id\": " + String(id) + ", \"results\": " + user_inputs + "}";
      Serial.println(result);

      http.begin("https://192.168.1.10/workflow/instructions");
      http.addHeader("Cookie", cookies);
      http.addHeader("X-XSRF-TOKEN", xsrf);
      http.addHeader("Content-Type", "application/json");
      http.addHeader("SiteId", "-1");

      httpCode = http.POST(result);

      if (httpCode == HTTP_CODE_OK) {
        String resp = http.getString();
        DynamicJsonDocument respJson(1024);
        deserializeJson(respJson, resp);
        int id = respJson["id"];

        // Process the response and perform further actions

      } else {
        Serial.println("Error sending POST request");
      }

      http.end();

      delay(1000);
    }
  } else {
    Serial.println("Error sending GET request");
  }

  http.end();

  delay(1000);
}
