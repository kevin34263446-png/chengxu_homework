// ex02.ino 1Hz LED闪烁 millis实现
const int ledPin = 16;
unsigned long preTime = 0; // 记录上次状态切换时间
const unsigned long interval = 1000; // 间隔1000ms=1s

void setup() {
  pinMode(ledPin, OUTPUT);
}

void loop() {
  unsigned long curTime = millis();
  // 时间差超过1000ms切换LED状态
  if (curTime - preTime >= interval) {
    digitalWrite(ledPin, !digitalRead(ledPin));
    preTime = curTime;
  }
}