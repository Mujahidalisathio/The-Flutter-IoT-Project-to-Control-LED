#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char *ssid = "PTCL-BB";
const char *password = "43434343";

ESP8266WebServer server(80);

const int ledPin = D1;

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);

  // Connect to Wi-Fi with retry mechanism
  connectToWiFi();

  // Define routes
  server.on("/", HTTP_GET, []() {
    server.send(200, "text/plain", "NodeMCU is running!");
  });

  server.on("/ledControl", HTTP_GET, []() {
    String statusParam = server.arg("status");
    if (statusParam == "1") {
      digitalWrite(ledPin, HIGH);
      server.send(200, "text/plain", "LED turned on");
    } else if (statusParam == "0") {
      digitalWrite(ledPin, LOW);
      server.send(200, "text/plain", "LED turned off");
    } else {
      server.send(400, "text/plain", "Invalid status parameter");
    }
  });

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}

void connectToWiFi() {
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    Serial.println("Connecting to WiFi...");
    WiFi.begin(ssid, password);
    delay(1000);
    attempts++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Connected to WiFi");
  } else {
    Serial.println("Failed to connect to WiFi. Please check your credentials.");
    // You might want to add more sophisticated error handling here, e.g., reset the device
  }
}
