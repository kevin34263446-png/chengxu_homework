// ex04.ino Step3 — 加入软件防抖，最终版：摸一下亮、再摸一下灭
const int touchPin = T0;
const int ledPin   = 2;
const int touchThreshold = 400;
const unsigned long debounceDelay = 200;  // 防抖延时200ms

bool ledState       = false;
bool lastTouchState = false;
unsigned long lastDebounceTime = 0;

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
}

void loop() {
  int touchVal = touchRead(touchPin);
  bool curTouch = (touchVal < touchThreshold);

  // 边缘检测 + 软件防抖
  if (curTouch && !lastTouchState) {
    if (millis() - lastDebounceTime > debounceDelay) {
      ledState = !ledState;
      digitalWrite(ledPin, ledState ? HIGH : LOW);
      Serial.print("触摸触发！LED: ");
      Serial.println(ledState ? "ON" : "OFF");
      lastDebounceTime = millis();
    }
  }

  lastTouchState = curTouch;
  delay(10);
}
