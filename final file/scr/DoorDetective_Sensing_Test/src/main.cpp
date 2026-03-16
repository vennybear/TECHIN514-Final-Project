#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>
#include <Adafruit_LIS3DH.h>
#include <Adafruit_Sensor.h>
#include "esp_sleep.h"

/* 
 * Door Detective - Final Sensing Node (PCB)
 * Logic: Temporal Pattern Recognition (DSP)
 * Author: Yuna Xiong
 */

// 1. 硬件与通信设置
Adafruit_LIS3DH lis = Adafruit_LIS3DH();
// 这里是你的 Display 端 XIAO C3 的 MAC 地址
uint8_t broadcastAddress[] = {0x58, 0x8C, 0x81, 0x9D, 0x62, 0x34};

// 2. 算法变量
unsigned long firstKnockTime = 0;
int knockStep = 0;           // 状态机：0-等待第一下，1-等待第二下
const float threshold = 13.5; // 震动阈值（根据实测微调：8.0灵敏，15.0迟钝）
const int minInterval = 200;  // 两次敲击之间最小间隔（ms），用于防抖

// 发送状态反馈
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nPacket Send Status: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Success" : "Fail");
}

void setup() {
  Serial.begin(115200);

  // 初始化 WiFi 和 ESP-NOW
  WiFi.mode(WIFI_STA);
  WiFi.disconnect(); 
  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW Init Failed");
    return;
  }
  esp_now_register_send_cb(OnDataSent);

  // 绑定接收端
  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }

  // 初始化传感器
  if (!lis.begin(0x18)) {
    Serial.println("LIS3DH Not Found!");
    while (1);
  }
  lis.setRange(LIS3DH_RANGE_2_G); // 最灵敏模式
  Serial.println(">>> Door Detective: Sensing Node Ready <<<");
}

void loop() {
  sensors_event_t event;
  lis.getEvent(&event);

  // --- [DSP 步骤 1: Signal Magnitude Vector] ---
  // 计算合成加速度，过滤掉安装方向的影响
  float smv = sqrt(sq(event.acceleration.x) + sq(event.acceleration.y) + sq(event.acceleration.z));

  // --- [逻辑 2: 节奏特征提取] ---
  if (smv > threshold) {
    if (knockStep == 0) {
      // 捕获到第一下
      firstKnockTime = millis();
      knockStep = 1;
      Serial.println("First knock...");
      delay(minInterval); // 机械防抖
    } 
    else if (knockStep == 1 && (millis() - firstKnockTime > minInterval)) {
      // 捕获到第二下，计算间隔
      unsigned long interval = millis() - firstKnockTime;
      Serial.print("Interval measured: "); Serial.print(interval); Serial.println("ms");

      String result = "GUEST"; // 默认普通访客
      if (interval < 500) {
        result = "DELIVERY"; // 快速两连击：砰砰！
      } else if (interval < 1200) {
        result = "FRIEND";   // 慢速两连击：砰...砰
      }

      Serial.print("Classification: "); Serial.println(result);
      esp_now_send(broadcastAddress, (uint8_t *) result.c_str(), result.length());
      
      knockStep = 0; // 重置状态机
      delay(2000);   // 冷却，防止余震触发
    }
  }

  // 超时保护：如果敲了第一下后 2 秒没动静，归零
  if (knockStep == 1 && (millis() - firstKnockTime > 2000)) {
    knockStep = 0;
    Serial.println("Timeout, resetting pattern...");
  }

 // 先定个 50 毫秒的闹钟
  esp_sleep_enable_timer_wakeup(50 * 1000); 
  // 然后闭眼睡觉
  esp_light_sleep_start();
}