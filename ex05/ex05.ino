// ex05.ino Step3 — 加入边缘检测 + 软件防抖，最终版
const int touchPin = T0;
const int ledPin   = 2;
const int freq = 5000;
const int ledChannel = 0;
const int resolution = 8;
const int touchThreshold = 400;
const unsigned long debounceDelay = 200;

const int TOTAL_LEVELS = 3;
const int speedDelays[TOTAL_LEVELS] = {3, 10, 25};  // 快/中/慢

int  speedLevel      = 0;
bool lastTouchState  = false;
unsigned long lastDebounceTime = 0;

void setup() {
  Serial.begin(115200);
  ledcSetup(ledChannel, freq, resolution);
  ledcAttachPin(ledPin, ledChannel);
  Serial.println("ex05: 多档位触摸调速呼吸灯");
}

void loop() {
  int touchVal = touchRead(touchPin);
  bool curTouch = (touchVal < touchThreshold);

  // 边缘检测 + 防抖
  if (curTouch && !lastTouchState) {
    if (millis() - lastDebounceTime > debounceDelay) {
      speedLevel = (speedLevel + 1) % TOTAL_LEVELS;
      Serial.print("切换档位: ");
      Serial.println(speedLevel);
      lastDebounceTime = millis();
    }
  }
  lastTouchState = curTouch;

  int stepDelay = speedDelays[speedLevel];

  for (int duty = 0; duty <= 255; duty++) {
    ledcWrite(ledChannel, duty);
    delay(stepDelay);
  }
  for (int duty = 255; duty >= 0; duty--) {
    ledcWrite(ledChannel, duty);
    delay(stepDelay);
  }
}
