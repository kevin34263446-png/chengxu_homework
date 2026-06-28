// ex09.ino Step1 — Web服务器 + 触摸传感器数据API接口
#include <WiFi.h>
#include <WebServer.h>

const char* ap_ssid = "ESP32-DASHBOARD";
const char* ap_pass = "12345678";
const int touchPin = T0;

WebServer server(80);

void handleRoot() {
  String html = R"rawliteral(
<!DOCTYPE html>
<html lang="zh-CN">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>传感器仪表盘</title>
</head>
<body style="font-family:Arial; text-align:center; margin-top:60px;">
  <h1>实时传感器仪表盘</h1>
  <p>触摸传感器数值:</p>
  <p style="font-size:64px;" id="sensorVal">--</p>
  <p id="status">等待数据...</p>
</body>
</html>
)rawliteral";
  server.send(200, "text/html; charset=UTF-8", html);
}

void handleApiTouch() {
  int touchVal = touchRead(touchPin);
  server.send(200, "text/plain", String(touchVal));
}

void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_AP);
  WiFi.softAP(ap_ssid, ap_pass);
  Serial.println("AP已开启");
  Serial.print("IP: ");
  Serial.println(WiFi.softAPIP());

  server.on("/", handleRoot);
  server.on("/api/touch", handleApiTouch);
  server.begin();
  Serial.println("仪表盘服务器已启动");
}

void loop() {
  server.handleClient();
}
