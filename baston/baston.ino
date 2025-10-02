#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#define DEVICE_NAME "baston_alerta"
#define trigPin 15
#define echoPin 2

BLEServer* pServer = NULL;
BLECharacteristic* pCharacteristic = NULL;

BLEAdvertising *pAdvertising;
bool anunciando = false;

void setup() {
  Serial.begin(4800);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);


  // Inicia BLE con el nombre del dispositivo
  BLEDevice::init(DEVICE_NAME);
  BLEServer* pServer = BLEDevice::createServer();
  pAdvertising = BLEDevice::getAdvertising();

  Serial.println("✅ Bastón listo, esperando detección...");
}

void loop() {
  // medir distancia
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH);  // timeout 20ms
  float distance = duration * 0.034 / 2;
  Serial.print("distancia: ");
  Serial.println(distance);
  if (distance > 0 && distance < 30) {
    if (!anunciando) {
      Serial.println("🚨 Obstáculo detectado -> iniciando anuncio BLE");
      pAdvertising->start();
      anunciando = true;
    }
  } else {
    if (anunciando) {
      Serial.println("✅ Obstáculo superado -> deteniendo anuncio BLE");
      pAdvertising->stop();
      anunciando = false;
    }
  }

  delay(500);
}
