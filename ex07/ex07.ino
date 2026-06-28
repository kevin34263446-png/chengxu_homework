// ex07.ino Step3 — JS fetch 发送滑动条数值，ESP32解析URL并控制PWM
#include <WiFi.h>
#include <WebServer.h>

const char* ap_ssid = "ESP32-DIMMER";
const char* ap_pass = "12345678";
const int ledPin = 2;

const int freq = 5000;
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
  <style>
    body { font-family:Arial; text-align:center; margin-top:60px; background:#1a1a2e; color:#eee; }
    input[type=range] { width:80%; height:30px; margin:20px 0; }
    #val { font-size:48px; font-weight:bold; color:#00d4ff; }
  </style>
</head>
<body>
  <h1>LED 无极调光器</h1>
  <p>拖动滑动条实时调节LED亮度</p>
  <input type="range" min="0" max="255" value="0" id="slider">
  <p>当前亮度: <span id="val">0</span> / 255</p>

  <script>
    const slider = document.getElementById('slider');
    const valEl  = document.getElementById('val');
    slider.oninput = function() {
      valEl.textContent = this.value;
      fetch('/set?val=' + this.value);
    };
  </script>
</body>
</html>
)rawliteral";
  server.send(200, "text/html; charset=UTF-8", html);
}

void handleSet() {
  if (server.hasArg("val")) {
    int duty = server.arg("val").toInt();
    duty = constrain(duty, 0, 255);
    ledcWrite(ledPin, duty);
    server.send(200, "text/plain", "OK");
  } else {
    server.send(400, "text/plain", "Missing val");
  }
}

void setup() {
  Serial.begin(115200);
  ledcAttach(ledPin, freq, resolution);
  ledcWrite(ledPin, 0);

  WiFi.mode(WIFI_AP);
  delay(100);
  bool apOK = WiFi.softAP(ap_ssid, ap_pass);
  Serial.print("AP启动状态: ");
  Serial.println(apOK ? "成功" : "失败");
  Serial.print("SSID: ");
  Serial.println(ap_ssid);
  Serial.print("IP: ");
  Serial.println(WiFi.softAPIP());
  Serial.print("MAC: ");
  Serial.println(WiFi.softAPmacAddress());

  server.on("/", handleRoot);
  server.on("/set", handleSet);
  server.begin();
  Serial.println("无极调光器已就绪");
}

void loop() {
  server.handleClient();
}
