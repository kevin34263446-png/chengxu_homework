// ex06.ino Step3 — 反相PWM：警车双闪灯效，最终版
const int ledPinA = 2;
const int ledPinB = 4;
const int freq = 5000;
const int resolution = 8;

void setup() {
  Serial.begin(115200);
  ledcAttach(ledPinA, freq, resolution);
  ledcAttach(ledPinB, freq, resolution);
  // 初始：A全亮 B全灭
  ledcWrite(ledPinA, 255);
  ledcWrite(ledPinB, 0);
  Serial.println("ex06: 警车双闪灯效");
}

void loop() {
  // 反相渐变1：A渐亮(0→255)  B渐暗(255→0)
  for (int duty = 0; duty <= 255; duty++) {
    ledcWrite(ledPinA, duty);
    ledcWrite(ledPinB, 255 - duty);
    delay(5);
  }
  // 反相渐变2：A渐暗(255→0)  B渐亮(0→255)
  for (int duty = 255; duty >= 0; duty--) {
    ledcWrite(ledPinA, duty);
    ledcWrite(ledPinB, 255 - duty);
    delay(5);
  }
}
