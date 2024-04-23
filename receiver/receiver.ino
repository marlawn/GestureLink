#include <WiFi.h>
#include <esp_now.h>
#include <string.h>

const int led_default = 32;
const int led_peace = 33;
const int led_point = 25;
const int led_rock = 26;
const int led_fist = 27;

typedef struct struct_message {
  char message[50];
} struct_message;
struct_message receivedData;

void setup() {
  Serial.begin(9600);
  WiFi.mode(WIFI_STA);

  pinMode(led_default, OUTPUT);
  pinMode(led_peace, OUTPUT);
  pinMode(led_point, OUTPUT);
  pinMode(led_rock, OUTPUT);
  pinMode(led_fist, OUTPUT);
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_recv_cb(onDataReceived);
}

// Callback function to handle received data
void onDataReceived(const uint8_t *mac_addr, const uint8_t *data, int data_len) {
  if (data_len == sizeof(struct_message)) {
    memcpy(&receivedData, data, sizeof(receivedData));
    Serial.println("Received gesture: " + String(receivedData.message));
  } else {
    Serial.println("Received invalid data");
  }

  const char* receivedMessage = receivedData.message;

  if (strcmp(receivedMessage, "peace") == 0) {
    digitalWrite(led_default, LOW);
    digitalWrite(led_peace, HIGH);
    digitalWrite(led_point, LOW);
    digitalWrite(led_rock, LOW);
    digitalWrite(led_fist, LOW);
  } else if (strcmp(receivedMessage, "point") == 0) {
    digitalWrite(led_default, LOW);
    digitalWrite(led_peace, LOW);
    digitalWrite(led_point, HIGH);
    digitalWrite(led_rock, LOW);
    digitalWrite(led_fist, LOW);
  } else if (strcmp(receivedMessage, "rock") == 0) {
    digitalWrite(led_default, LOW);
    digitalWrite(led_peace, LOW);
    digitalWrite(led_point, LOW);
    digitalWrite(led_rock, HIGH);
    digitalWrite(led_fist, LOW);
  } else if (strcmp(receivedMessage, "fist") == 0) {
    digitalWrite(led_default, LOW);
    digitalWrite(led_peace, LOW);
    digitalWrite(led_point, LOW);
    digitalWrite(led_rock, LOW);
    digitalWrite(led_fist, HIGH);
  } else {
    digitalWrite(led_default, HIGH);
    digitalWrite(led_peace, LOW);
    digitalWrite(led_point, LOW);
    digitalWrite(led_rock, LOW);
    digitalWrite(led_fist, LOW);
  }
}
void loop() {
  // Your main loop code here
  // Since the receiving code is event-driven, there's no need for additional logic here.
}
