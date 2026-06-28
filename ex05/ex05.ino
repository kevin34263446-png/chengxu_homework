// ex05.ino — 多档位触摸调速呼吸灯 (非阻塞版)
const int touchPin = T0;
const int ledPin   = 2;
const int freq = 5000;
const int resolution = 8;
const int touchThreshold = 400;
const unsigned long debounceDelay = 200;

const int TOTAL_LEVELS = 3;
const int speedDelays[TOTAL_LEVELS] = {3, 10, 25};  // 快/中/慢 (ms/步)

int  speedLevel      = 0;       // 当前档位 0/1/2
int  duty            = 0;       // 当前占空比
bool fadingUp        = true;    // 呼吸方向：渐亮/渐暗
unsigned long lastStepTime    = 0;
bool lastTouchState  = false;
unsigned long lastDebounceTime = 0;

void setup() {
  Serial.begin(115200);
  ledcAttach(ledPin, freq, resolution);
  ledcWrite(ledPin, 0);
  lastStepTime = millis();
  Serial.println("ex05: 多档位触摸调速呼吸灯");
  Serial.print("当前档位: ");
  Serial.println(speedLevel + 1);
}

void loop() {
  // --- 触摸检测 (每次loop都检查, 即时响应) ---
  int touchVal = touchRead(touchPin);
  bool curTouch = (touchVal < touchThreshold);

  if (curTouch && !lastTouchState) {
    if (millis() - lastDebounceTime > debounceDelay) {
      speedLevel = (speedLevel + 1) % TOTAL_LEVELS;
      Serial.print("切换档位: ");
      Serial.print(speedLevel + 1);
      Serial.print(" (步进延迟 ");
      Serial.print(speedDelays[speedLevel]);
      Serial.println("ms)");
      lastDebounceTime = millis();
    }
  }
  lastTouchState = curTouch;

  // --- 呼吸灯 (非阻塞, 按档位速度更新) ---
  int stepDelay = speedDelays[speedLevel];
  if (millis() - lastStepTime >= (unsigned long)stepDelay) {
    lastStepTime = millis();

    if (fadingUp) {
      duty++;
      if (duty >= 255) {
        duty = 255;
        fadingUp = false;
      }
    } else {
      duty--;
      if (duty <= 0) {
        duty = 0;
        fadingUp = true;
      }
    }
    ledcWrite(ledPin, duty);
  }

  // 给WiFi/系统留点时间, 也避免loop空转
  delay(1);
}
