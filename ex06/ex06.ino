// ex06.ino Step2 — 双LED双通道PWM（同相渐变）
const int ledPinA = 2;
const int ledPinB = 4;
const int freq = 5000;
const int channelA = 0;
const int channelB = 1;
const int resolution = 8;

void setup() {
  ledcSetup(channelA, freq, resolution);
  ledcSetup(channelB, freq, resolution);
  ledcAttachPin(ledPinA, channelA);
  ledcAttachPin(ledPinB, channelB);
}

void loop() {
  // 两个LED同时渐亮
  for (int duty = 0; duty <= 255; duty++) {
    ledcWrite(channelA, duty);
    ledcWrite(channelB, duty);
    delay(5);
  }
  // 两个LED同时渐暗
  for (int duty = 255; duty >= 0; duty--) {
    ledcWrite(channelA, duty);
    ledcWrite(channelB, duty);
    delay(5);
  }
}
