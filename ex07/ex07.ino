// ex07.ino Step1 — WiFi AP + Web服务器 + PWM基础搭建
#include <WiFi.h>
#include <WebServer.h>

const char* ap_ssid = "ESP32-DIMMER";
const char* ap_pass = "12345678";
const int ledPin = 2;

const int freq = 5000;
const int ledChannel = 0;
const int resolution = 8;

WebServer server(80);

void handleRoot() {
  String html = "<!DOCTYPE html><html lang='zh-CN'><head><meta charset='UTF-8'>";
  html += "<title>无极调光器</title></head><body>";
  html += "<h1>LED 无极调光器</h1>";
  html += "<p>Web服务器已启动</p>";
  html += "</body></html>";
  server.send(200, "text/html; charset=UTF-8", html);
}

void setup() {
  Serial.begin(115200);
  ledcSetup(ledChannel, freq, resolution);
  ledcAttachPin(ledPin, ledChannel);
  ledcWrite(ledChannel, 0);

  WiFi.mode(WIFI_AP);
  WiFi.softAP(ap_ssid, ap_pass);
  Serial.println("AP已开启");
  Serial.print("IP: ");
  Serial.println(WiFi.softAPIP());

  server.on("/", handleRoot);
  server.begin();
  Serial.println("Web服务器已启动");
}

void loop() {
  server.handleClient();
}
