// ex03. SOS灯光信号 millis非阻塞实现
const int ledPin = 16;
unsigned long preTime = 0;
unsigned long now;

// 时序定义
const int shortOn = 200;
const int shortOff = 200;
const int longOn = 600;
const int longOff = 200;
const int endPause = 1500; // 整套SOS结束停顿

// 状态机：0=等待开始，1=3短闪，2=3长闪，3=3短闪，4=结束停顿
int state = 0;
int cnt = 0; // 当前闪烁次数
bool ledState = LOW;

void setup() {
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
}

void loop() {
  now = millis();
  if (now - preTime < (ledState ? (state==1||state==3 ? shortOn : longOn) : (state==4 ? endPause : shortOff))) {
    return; // 时间未到，不执行切换
  }
  preTime = now;

  switch(state) {
    case 0: // 初始，进入第一段3短闪
      state = 1;
      cnt = 0;
      ledState = HIGH;
      digitalWrite(ledPin, HIGH);
      break;
    case 1: // 三段短闪
      ledState = !ledState;
      digitalWrite(ledPin, ledState);
      if(!ledState) cnt++;
      if(cnt >=3 && !ledState) {
        state = 2;
        cnt = 0;
      }
      break;
    case 2: // 三段长闪
      ledState = !ledState;
      digitalWrite(ledPin, ledState);
      if(!ledState) cnt++;
      if(cnt >=3 && !ledState) {
        state = 3;
        cnt = 0;
      }
      break;
    case 3: // 再次三段短闪
      ledState = !ledState;
      digitalWrite(ledPin, ledState);
      if(!ledState) cnt++;
      if(cnt >=3 && !ledState) {
        state = 4;
        digitalWrite(ledPin, LOW);
      }
      break;
    case 4: // 整套结束长停顿，重置循环
      state = 0;
      break;
  }
}