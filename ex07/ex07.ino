// ex07.ino — Web无极调光器 (AP模式) — 三路LED，滑动条控制亮度
#include <WiFi.h>
#include <WebServer.h>

// AP 热点配置
const char* ap_ssid = "ESP32-LED-602";
const char* ap_pass = "12345678";   // 至少8位，连接时输入

// 三路LED引脚
const int ledPins[3] = {2, 4, 5};
const int ledCount = 3;
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
  <title>三路无极调光器</title>
  <style>
    body { font-family:Arial; text-align:center; margin-top:40px; background:#1a1a2e; color:#eee; }
    .ch { margin:20px 0; padding:15px; background:#16213e; border-radius:12px; width:85%; margin-left:auto; margin-right:auto; }
    .ch h3 { margin:0 0 10px 0; color:#00d4ff; }
    input[type=range] { width:80%; height:24px; margin:8px 0; }
    .val { font-size:32px; font-weight:bold; color:#00d4ff; }
  </style>
</head>
<body>
  <h1>LED 三路无极调光器</h1>
  <p>分别拖动滑动条调节三路LED亮度</p>

  <div class="ch">
    <h3>LED 1 (引脚 2)</h3>
    <input type="range" min="0" max="255" value="0" id="slider0">
    <p>亮度: <span class="val" id="val0">0</span> / 255</p>
  </div>

  <div class="ch">
    <h3>LED 2 (引脚 4)</h3>
    <input type="range" min="0" max="255" value="0" id="slider1">
    <p>亮度: <span class="val" id="val1">0</span> / 255</p>
  </div>

  <div class="ch">
    <h3>LED 3 (引脚 5)</h3>
    <input type="range" min="0" max="255" value="0" id="slider2">
    <p>亮度: <span class="val" id="val2">0</span> / 255</p>
  </div>

  <script>
    for (let i = 0; i < 3; i++) {
      const slider = document.getElementById('slider' + i);
      const valEl  = document.getElementById('val' + i);
      slider.oninput = function() {
        valEl.textContent = this.value;
        fetch('/set?led=' + i + '&val=' + this.value);
      };
    }
  </script>
</body>
</html>
)rawliteral";
  server.send(200, "text/html; charset=UTF-8", html);
}

void handleSet() {
  if (server.hasArg("led") && server.hasArg("val")) {
    int led  = server.arg("led").toInt();
    int duty = server.arg("val").toInt();
    led  = constrain(led, 0, ledCount - 1);
    duty = constrain(duty, 0, 255);
    ledcWrite(ledPins[led], duty);  // 3.x 用引脚号
    server.send(200, "text/plain", "OK");
  } else {
    server.send(400, "text/plain", "Missing led or val");
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  // 初始化三路LED PWM通道
  for (int i = 0; i < ledCount; i++) {
    ledcAttach(ledPins[i], freq, resolution);
    ledcWrite(ledPins[i], 0);  // 3.x 用引脚号，不用通道号
  }

  // 启动AP模式 — ESP32自己当热点
  Serial.print("启动AP热点: ");
  Serial.println(ap_ssid);
  WiFi.mode(WIFI_AP);
  boolean ok = WiFi.softAP(ap_ssid, ap_pass);

  if (ok) {
    Serial.println("AP热点已创建");
    Serial.print("IP地址: ");
    Serial.println(WiFi.softAPIP());  // 默认 192.168.4.1
    Serial.print("用手机/电脑连接WiFi: ");
    Serial.print(ap_ssid);
    Serial.print("  密码: ");
    Serial.println(ap_pass);
    Serial.print("然后在浏览器打开: http://");
    Serial.println(WiFi.softAPIP());
  } else {
    Serial.println("AP创建失败！");
  }

  server.on("/", handleRoot);
  server.on("/set", handleSet);
  server.begin();
  Serial.println("三路无极调光器已就绪");
}

void loop() {
  server.handleClient();
}
