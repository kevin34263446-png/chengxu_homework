// ex05.ino Step1 — 基础PWM呼吸灯（渐亮渐暗循环）
const int ledPin = 2;
const int freq = 5000;
const int ledChannel = 0;
const int resolution = 8;

void setup() {
  ledcSetup(ledChannel, freq, resolution);
  ledcAttachPin(ledPin, ledChannel);
}

void loop() {
  // 渐亮
  for (int duty = 0; duty <= 255; duty++) {
    ledcWrite(ledChannel, duty);
    delay(10);
  }
  // 渐暗
  for (int duty = 255; duty >= 0; duty--) {
    ledcWrite(ledChannel, duty);
    delay(10);
  }
}
