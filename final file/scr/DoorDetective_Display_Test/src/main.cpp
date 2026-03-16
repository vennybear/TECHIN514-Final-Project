#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>
#include <Stepper.h>

// 1. 硬件引脚
Stepper myStepper(600, D0, D2, D1, D3); 
const int buttonPin = D4; // 自锁大黑按钮
const int ledPin = D5;

// 2. 全局状态
String lastMsg = "";
bool wirelessTriggered = false;
int lastButtonState; 
const int motorSpeed = 20;

// 接收回调
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
    char buffer[len + 1];
    memcpy(buffer, incomingData, len);
    buffer[len] = '\0';
    lastMsg = String(buffer);
    wirelessTriggered = true;
}

// 自锁开关翻转检测
bool checkButtonToggle() {
    int currentState = digitalRead(buttonPin);
    if (currentState != lastButtonState) {
        delay(50); // 防抖
        lastButtonState = currentState;
        return true;
    }
    return false;
}

void setup() {
    Serial.begin(115200);
    WiFi.mode(WIFI_STA);
    if (esp_now_init() == ESP_OK) {
        esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecv));
    }

    myStepper.setSpeed(motorSpeed);
    pinMode(ledPin, OUTPUT);
    pinMode(buttonPin, INPUT_PULLUP);
    lastButtonState = digitalRead(buttonPin);

    // 开机闪烁
    for(int i=0; i<3; i++) { digitalWrite(ledPin, HIGH); delay(100); digitalWrite(ledPin, LOW); delay(100); }
    Serial.println("Display Unit READY for Classification!");
}

// 核心执行逻辑：根据不同步数转动，并支持 8s 内随时按按钮取消
void runNotification(int steps, int mode) {
    // mode 0 = 敲门（灯长亮），mode 1 = 自检（灯闪烁）
    Serial.print("Moving steps: "); Serial.println(steps);
    
    digitalWrite(ledPin, HIGH);
    myStepper.step(steps);

    // 8 秒停留，期间只要拨动按钮，立刻 break
    for (int i = 0; i < 80; i++) {
        if (mode == 0) digitalWrite(ledPin, HIGH);
        else digitalWrite(ledPin, (i % 2 == 0));
        
        delay(100);

        if (checkButtonToggle()) {
            Serial.println("Manual Dismissed!");
            break; 
        }
    }

    digitalWrite(ledPin, LOW);
    myStepper.step(-steps); // 复位
}

void loop() {
    if (wirelessTriggered) {
        int steps = 180; // 默认：正上方 (Guest)
        
        if (lastMsg == "DELIVERY") {
            steps = 90;  // 45度位置
        } else if (lastMsg == "FRIEND") {
            steps = 270; // 135度位置 (你说的 270 度应该就是这里)
        }
        
        Serial.print("Moving to steps: "); Serial.println(steps);
        runNotification(steps, 0); 
        wirelessTriggered = false;
    }

    // 逻辑 B: 手动按钮触发 (自检)
    if (checkButtonToggle()) {
        Serial.println("Manual Button Event");
        runNotification(150, 1); // 运行闪烁自检
    }
}