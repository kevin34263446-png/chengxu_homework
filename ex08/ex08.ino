// ex08.ino — 物联网安防报警器 (STA模式)
#include <WiFi.h>
#include <WebServer.h>

// ====== 改成你家的WiFi名和密码 ======
const char* sta_ssid = "shulaoda";
const char* sta_pass = "skwskw123";

const int ledPin  = 2;
const int touchPin = T0;
const int touchThreshold = 400;
const unsigned long debounceDelay = 200;

enum State { DISARMED, ARMED, ALARM };
State sysState = DISARMED;

unsigned long preTime = 0;
unsigned long lastDebounceTime = 0;
bool lastTouchState = false;
bool ledOn = false;

WebServer server(80);

void handleRoot() {
  String stateText = (sysState == DISARMED) ? "已撤防" :
                     (sysState == ARMED)   ? "已布防 🔒" : "⚠ 报警中！";
  String html = R"rawliteral(
<!DOCTYPE html>
<html lang="zh-CN">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>安防报警器</title>
  <style>
    body { font-family:Arial; text-align:center; margin-top:60px; background:#1a1a2e; color:#eee; }
    button { font-size:24px; padding:15px 40px; margin:10px; border:none; border-radius:8px; color:#fff; cursor:pointer; }
    #arm { background:#d32f2f; }
    #disarm { background:#388e3c; }
    .alarm { color:#ff1744; font-size:32px; }
  </style>
</head>
<body>
  <h1>物联网安防报警器</h1>
  <p>状态: <b class=")rawliteral" + String(sysState == ALARM ? "alarm" : "") + R"rawliteral(">)rawliteral" + stateText + R"rawliteral(</b></p>
  <a href="/arm"><button id="arm">🔒 布防</button></a>
  <a href="/disarm"><button id="disarm">🔓 撤防</button></a>
</body>
</html>
)rawliteral";
  server.send(200, "text/html; charset=UTF-8", html);
}

void handleArm() {
  sysState = ARMED;
  Serial.println("🔒 系统已布防");
  server.sendHeader("Location", "/");
  server.send(303);
}

void handleDisarm() {
  sysState = DISARMED;
  digitalWrite(ledPin, LOW);
  ledOn = false;
  Serial.println("🔓 系统已撤防，报警解除");
  server.sendHeader("Location", "/");
  server.send(303);
}

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  Serial.print("连接WiFi: ");
  Serial.println(sta_ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(sta_ssid, sta_pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("WiFi已连接");
  Serial.print("请在浏览器打开: http://");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/arm", handleArm);
  server.on("/disarm", handleDisarm);
  server.begin();
  Serial.println("安防系统已就绪");
}

void loop() {
  server.handleClient();

  if (sysState == ARMED) {
    int touchVal = touchRead(touchPin);
    bool curTouch = (touchVal < touchThreshold);
    if (curTouch && !lastTouchState) {
      if (millis() - lastDebounceTime > debounceDelay) {
        sysState = ALARM;
        Serial.println("⚠ 报警触发！");
        lastDebounceTime = millis();
      }
    }
    lastTouchState = curTouch;
  }

  if (sysState == ALARM) {
    if (millis() - preTime >= 150) {
      ledOn = !ledOn;
      digitalWrite(ledPin, ledOn ? HIGH : LOW);
      preTime = millis();
    }
  }
}
