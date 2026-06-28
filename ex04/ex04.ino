// ex04.ino Step1 — 基础触摸读取 + LED控制（摸着亮，松开灭）
const int touchPin = T0;
const int ledPin   = 2;
const int touchThreshold = 400;

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
}

void loop() {
  int touchVal = touchRead(touchPin);
  if (touchVal < touchThreshold) {
    digitalWrite(ledPin, HIGH);   // 触摸 → 亮
  } else {
    digitalWrite(ledPin, LOW);    // 松开 → 灭
  }
  delay(10);
}
