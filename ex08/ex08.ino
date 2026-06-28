// ex08.ino Step1 — WiFi AP + Web按钮布防/撤防 + 状态变量
#include <WiFi.h>
#include <WebServer.h>

const char* ap_ssid = "ESP32-ALARM";
const char* ap_pass = "12345678";
const int ledPin = 2;

enum State { DISARMED, ARMED, ALARM };
State sysState = DISARMED;

WebServer server(80);

void handleRoot() {
  String armedText = (sysState == DISARMED) ? "已撤防" :
                     (sysState == ARMED)   ? "已布防" : "⚠ 报警中！";
  String html = R"rawliteral(
<!DOCTYPE html>
<html lang="zh-CN">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>安防报警器</title>
  <style>
    body { font-family:Arial; text-align:center; margin-top:60px; }
    button { font-size:24px; padding:15px 40px; margin:10px; border:none; border-radius:8px; color:#fff; }
    #arm { background:#d32f2f; }
    #disarm { background:#388e3c; }
  </style>
</head>
<body>
  <h1>物联网安防报警器</h1>
  <p>状态: <b>)rawliteral" + armedText + R"rawliteral(</b></p>
  <a href="/arm"><button id="arm">🔒 布防</button></a>
  <a href="/disarm"><button id="disarm">🔓 撤防</button></a>
</body>
</html>
)rawliteral";
  server.send(200, "text/html; charset=UTF-8", html);
}

void handleArm() {
  sysState = ARMED;
  Serial.println("系统已布防");
  server.sendHeader("Location", "/");
  server.send(303);
}

void handleDisarm() {
  sysState = DISARMED;
  Serial.println("系统已撤防");
  server.sendHeader("Location", "/");
  server.send(303);
}

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  WiFi.mode(WIFI_AP);
  WiFi.softAP(ap_ssid, ap_pass);
  Serial.println("AP已开启");
  Serial.print("IP: ");
  Serial.println(WiFi.softAPIP());

  server.on("/", handleRoot);
  server.on("/arm", handleArm);
  server.on("/disarm", handleDisarm);
  server.begin();
  Serial.println("安防系统已启动");
}

void loop() {
  server.handleClient();
}
