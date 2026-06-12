/*
In this program, we will test the ESP32 by creating a simple web server that can be accessed from any device connected to the same router. 
The web server will display LED ON and LED OFF buttons. 
Clicking these buttons will control the onboard LED of the ESP32, which is connected to GPIO2.
When you click LED ON, the onboard LED will turn on, and clicking LED OFF will turn it off. 
This provides a simple and interactive way to test the ESP32's functionality.
*/
#include <WiFi.h>
#include <WebServer.h>
#include <Adafruit_NeoPixel.h>

#define LED_PIN 48
#define NUM_LEDS 1

Adafruit_NeoPixel rgb(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

// Replace with your network credentials
const char* ssid = "SSID";
const char* password = "PASSWORD";

WebServer server(80); // Web server on port 80

bool ledState = false; // Track LED state

// HTML content for the web page
String HTMLPage() {
  String html = "<!DOCTYPE html><html>";
  html += "<head><title>ESP32 LED Control</title></head>";
  html += "<body style='text-align: center; font-family: Arial;'>";

  html += "<h1>ESP32 LED Control</h1>";
  html += ledState ? "<p>LED is <strong>ON</strong></p>" : "<p>LED is <strong>OFF</strong></p>";
  html += "<a href='/on' style='padding: 10px 20px; background-color: green; color: white; text-decoration: none;'>Turn ON</a>&nbsp;";
  html += "<a href='/off' style='padding: 10px 20px; background-color: red; color: white; text-decoration: none;'>Turn OFF</a>";

  html += "</body></html>";
  return html;
}

// Handle the root path "/"
void handleRoot() {
  server.send(200, "text/html", HTMLPage());
}

// Handle LED ON request
void handleLEDOn() {
  ledState = true;
  rgb.setPixelColor(0, rgb.Color(0, 255, 0));
  rgb.show();
  server.send(200, "text/html", HTMLPage());
}

// Handle LED OFF request
void handleLEDOff() {
  ledState = false;
  rgb.setPixelColor(0, rgb.Color(0, 0, 0));
  rgb.show();
  server.send(200, "text/html", HTMLPage());
}

void setup() {
  Serial.begin(115200);
  rgb.begin();
  rgb.show();

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to: ");
  Serial.println(ssid);

  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
      delay(500);
      Serial.print(".");
      attempts++;
  }

  if (WiFi.status() == WL_CONNECTED) {
      Serial.println("\nConnected!");
      Serial.println(WiFi.localIP());
  } else {
      Serial.println("\nFailed! Status code: ");
      Serial.println(WiFi.status());
  }

  delay(1000); // give WiFi stack time to stabilize

  server.on("/", handleRoot);
  server.on("/on", handleLEDOn);
  server.on("/off", handleLEDOff);

  server.begin();
  Serial.println("Server started");;
}

void loop() {
  server.handleClient(); // Handle client requests
}