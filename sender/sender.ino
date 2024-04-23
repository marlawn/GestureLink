#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>

const int flex_pinky = 36; 
const int flex_ring = 39;
const int flex_middle = 34;
const int flex_index = 35;
const int flex_thumb = 32;

uint8_t broadcastAddress[] = {0xE8, 0x6B, 0xEA, 0xDE, 0xBB, 0x6C};
typedef struct struct_message {
  char message[50];
} struct_message;
struct_message myData;
esp_now_peer_info_t peerInfo;


void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }

  pinMode(flex_pinky, INPUT);
  pinMode(flex_ring, INPUT);
  pinMode(flex_middle, INPUT);
  pinMode(flex_index, INPUT);
  pinMode(flex_thumb, INPUT);
}

void loop() {
  if (Serial.available() > 0) {
    String gesture = Serial.readStringUntil('\n');
    gesture.toCharArray(myData.message, sizeof(myData.message));
    esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *)&myData, sizeof(myData));
    // Serial.println("GESTURE: " + gesture);
  }

  int sensor_pinky = analogRead(flex_pinky);
  int sensor_ring = analogRead(flex_ring);
  int sensor_middle = analogRead(flex_middle);
  int sensor_index = analogRead(flex_index);
  int sensor_thumb = analogRead(flex_thumb);

  Serial.print(sensor_pinky);
  Serial.print(",");
  Serial.print(sensor_ring);
  Serial.print(",");
  Serial.print(sensor_middle);
  Serial.print(",");
  Serial.print(sensor_index);
  Serial.print(",");
  Serial.println(sensor_thumb);

  delay(100);
}