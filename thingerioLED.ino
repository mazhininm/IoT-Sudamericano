#include <WiFi.h>
#include <ThingerESP32.h>
/* ========= THINGER.IO ========= */
#define USERNAME          "mazhininm"
#define DEVICE_ID         "esp32_led"
#define DEVICE_CREDENTIAL "abcd1234"
/* ========= WIFI ========= */
#define SSID              "damatiz"
#define SSID_PASSWORD     "abcd1234"   // Reemplaza por tu clave
/* ========= LED ========= */
#define LED_PIN 2   // LED interno del ESP32 (GPIO2)
ThingerESP32 thing(USERNAME, DEVICE_ID, DEVICE_CREDENTIAL);
void setup() {
  Serial.begin(115200);
  delay(1000);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  thing.add_wifi(SSID, SSID_PASSWORD);
  // Recurso para control desde Thinger (Switch)
  thing["led"] << digitalPin(LED_PIN);
  Serial.println("ESP32 iniciado. Revisa Thinger.io (ONLINE).");
}
void loop() {
  thing.handle();
}
