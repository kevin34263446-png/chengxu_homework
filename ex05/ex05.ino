// ex05.ino Step2 — 加入触摸引脚，切换三档呼吸速度
const int touchPin = T0;
const int ledPin   = 2;
const int freq = 5000;
const int ledChannel = 0;
const int resolution = 8;
const int touchThreshold = 400;

const int TOTAL_LEVELS = 3;
const int speedDelays[TOTAL_LEVELS] = {3, 10, 25};  // 快/中/慢

int speedLevel = 0;

void setup() {
  Serial.begin(115200);
  ledcSetup(ledChannel, freq, resolution);
  ledcAttachPin(ledPin, ledChannel);
  Serial.println("ex05: 触摸调速呼吸灯");
}

void loop() {
  // 触摸切换档位
  int touchVal = touchRead(touchPin);
  if (touchVal < touchThreshold) {
    speedLevel = (speedLevel + 1) % TOTAL_LEVELS;  // 0→1→2→0...
    Serial.print("档位: ");
    Serial.println(speedLevel);
    delay(300);  // 简单防抖
  }

  int stepDelay = speedDelays[speedLevel];

  // 渐亮
  for (int duty = 0; duty <= 255; duty++) {
    ledcWrite(ledChannel, duty);
    delay(stepDelay);
  }
  // 渐暗
  for (int duty = 255; duty >= 0; duty--) {
    ledcWrite(ledChannel, duty);
    delay(stepDelay);
  }
}
