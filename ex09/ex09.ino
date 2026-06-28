// ex09.ino Step3 — 仪表盘样式美化 + 实时数值 + 触摸状态指示
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
  <title>实时传感器仪表盘</title>
  <style>
    * { margin:0; padding:0; box-sizing:border-box; }
    body { font-family:'Segoe UI',Arial; text-align:center;
           background:linear-gradient(135deg,#0d1117,#161b22); color:#c9d1d9;
           min-height:100vh; display:flex; flex-direction:column; align-items:center;
           justify-content:center; }
    .card { background:#21262d; border-radius:16px; padding:40px;
            box-shadow:0 8px 32px rgba(0,0,0,.4); min-width:320px; }
    h1 { font-size:22px; color:#58a6ff; margin-bottom:8px; }
    .sub { font-size:13px; color:#8b949e; margin-bottom:24px; }
    .value { font-size:96px; font-weight:bold; color:#58a6ff;
             font-variant-numeric:tabular-nums; line-height:1.1; transition:color .3s; }
    .value.touched { color:#ff7b72; }
    .label { font-size:14px; color:#8b949e; margin-top:8px; }
    .bar-bg { width:100%; height:8px; background:#30363d; border-radius:4px;
              margin-top:20px; overflow:hidden; }
    .bar-fg { height:100%; border-radius:4px; transition:width .2s,background .3s; }
    .status { margin-top:12px; font-size:15px; min-height:24px; }
  </style>
</head>
<body>
  <div class="card">
    <h1>📊 触摸传感器实时仪表盘</h1>
    <p class="sub">数值越小 → 手指越靠近触摸引脚</p>
    <div class="value" id="sensorVal">--</div>
    <div class="label">touchRead(T0) 原始值</div>
    <div class="bar-bg"><div class="bar-fg" id="bar" style="width:0%;background:#58a6ff;"></div></div>
    <div class="status" id="status">⏳ 等待数据...</div>
  </div>

  <script>
    function fetchSensor() {
      var xhr = new XMLHttpRequest();
      xhr.open('GET', '/api/touch', true);
      xhr.onload = function() {
        if (xhr.status == 200) {
          var val = parseInt(xhr.responseText);
          var elVal = document.getElementById('sensorVal');
          var elBar = document.getElementById('bar');
          var elSta = document.getElementById('status');

          elVal.textContent = val;

          // 进度条映射：val大→进度小，val小→进度大（触摸时数值小）
          var pct = Math.max(0, Math.min(100, (100 - val))); // 值越小进度越大
          elBar.style.width = pct + '%';

          if (val < 40) {
            elVal.className = 'value touched';
            elBar.style.background = '#ff7b72';
            elSta.innerHTML = '🔴 <b>已触摸！</b> 手指接触引脚';
          } else if (val < 60) {
            elVal.className = 'value';
            elBar.style.background = '#d29922';
            elSta.innerHTML = '🟡 <b>靠近中</b> 手指接近引脚';
          } else {
            elVal.className = 'value';
            elBar.style.background = '#58a6ff';
            elSta.innerHTML = '🟢 <b>空闲</b> 未检测到触摸';
          }
        }
      };
      xhr.send();
    }
    setInterval(fetchSensor, 200);
  </script>
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
  Serial.println("仪表盘服务器已就绪");
}

void loop() {
  server.handleClient();
}
