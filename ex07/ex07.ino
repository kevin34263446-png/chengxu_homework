// ex07.ino Step2 — HTML页面加入滑动条控件
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
  String html = R"rawliteral(
<!DOCTYPE html>
<html lang="zh-CN">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>无极调光器</title>
</head>
<body style="font-family:Arial; text-align:center; margin-top:50px;">
  <h1>LED 无极调光器</h1>
  <p>拖动滑动条调节LED亮度</p>
  <input type="range" min="0" max="255" value="0"
         style="width:80%; height:40px;">
  <p>当前亮度: <span id="val">0</span> / 255</p>
</body>
</html>
)rawliteral";
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
