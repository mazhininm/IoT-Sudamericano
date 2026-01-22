#include <WiFi.h>
#include <ThingerESP32.h>

//   Thinger.io
#define USERNAME "mazhininm"
#define DEVICE_ID "esp32_led"
#define DEVICE_CREDENTIAL "abcd1234"

// WiFi
#define SSID "TELECOMUNICACIONES"
#define SSID_PASSWORD "abcd1234"

ThingerESP32 thing(USERNAME, DEVICE_ID, DEVICE_CREDENTIAL);

const int echoPin = 26;
const int trigPin = 25;

float distance = 0;

void setup() {
  Serial.begin(9600);

  pinMode(echoPin, INPUT);
  pinMode(trigPin, OUTPUT);

  Serial.println("Ultrasonic sensor:");

  //  Conectar WiFi para Thinger
  thing.add_wifi(SSID, SSID_PASSWORD);

  //  Recurso en Thinger.io
  thing["distancia"] >> outputValue(distance);
}

void loop() {
  distance = readDistance();

  Serial.print(distance);
  Serial.println(" cm");

  thing.handle();   //  Mantiene conexión y envía datos
  delay(400);
}

// Función para medir distancia
float readDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);

  digitalWrite(trigPin, LOW);

  float d = pulseIn(echoPin, HIGH) / 58.00;
  return d;
}
