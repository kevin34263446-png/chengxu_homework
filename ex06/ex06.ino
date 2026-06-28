// ex06.ino Step1 — 单LED PWM渐变闪烁
const int ledPinA = 2;
const int freq = 5000;
const int channelA = 0;
const int resolution = 8;

void setup() {
  ledcSetup(channelA, freq, resolution);
  ledcAttachPin(ledPinA, channelA);
}

void loop() {
  // 渐亮
  for (int duty = 0; duty <= 255; duty++) {
    ledcWrite(channelA, duty);
    delay(5);
  }
  // 渐暗
  for (int duty = 255; duty >= 0; duty--) {
    ledcWrite(channelA, duty);
    delay(5);
  }
}
