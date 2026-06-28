// ex04.ino Step2 — 加入布尔状态变量 + 边缘检测（按下瞬间翻转LED）
const int touchPin = T0;
const int ledPin   = 2;
const int touchThreshold = 400;

bool ledState       = false;
bool lastTouchState = false;

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
}

void loop() {
  int touchVal = touchRead(touchPin);
  bool curTouch = (touchVal < touchThreshold);

  // 边缘检测：上次未触摸，当前被触摸 → 按下瞬间
  if (curTouch && !lastTouchState) {
    ledState = !ledState;                       // 翻转LED状态
    digitalWrite(ledPin, ledState ? HIGH : LOW);
    Serial.print("触发！LED: ");
    Serial.println(ledState ? "ON" : "OFF");
  }

  lastTouchState = curTouch;
  delay(10);
}
