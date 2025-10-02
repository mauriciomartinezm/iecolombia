#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include "Arduino.h"
#include "DFRobotDFPlayerMini.h"


DFRobotDFPlayerMini myDFPlayer;

const char* targetDeviceName = "sala_sistema";

int scanTime = 1;
BLEScan* pBLEScan;

// bandera para controlar si ya se disparó el audio
bool dispositivoCerca = false;

// Histéresis de RSSI
const int RSSI_CERCA = -50;  // umbral para "cerca"
const int RSSI_LEJOS = -65;  // umbral para "resetear"

// Pines sensor ultrasónico
const int PIN_VCC = 13;
const int PIN_TRIG = 12;
const int PIN_ECHO = 14;
int duration = 0;
int distance = 0;

class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks {
  void onResult(BLEAdvertisedDevice advertisedDevice) {
    String deviceName = advertisedDevice.getName().c_str();
    int rssi = advertisedDevice.getRSSI();

    if (deviceName == targetDeviceName) {
      Serial.printf("Encontrado %s con RSSI: %d dBm\n", deviceName, rssi);

      // Entrando cerca
      if (rssi > RSSI_CERCA && !dispositivoCerca) {
        Serial.println("📢 Dispositivo objetivo CERCA -> Reproduciendo audio");
        myDFPlayer.play(1);
        delay(2000);
        dispositivoCerca = true;
      }
      // Alejándose lo suficiente -> reset
      else if (rssi < RSSI_LEJOS && dispositivoCerca) {
        Serial.println("↩️  Se alejó, reseteo bandera");
        dispositivoCerca = false;
      }
    }
  }
};

void setup() {
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, 26, 25);

  if (!myDFPlayer.begin(Serial2, true, false)) {
    Serial.println("❌ No inicializa DFPlayer");
    return;
  }
  Serial.println("✅ DFPlayer listo");
  myDFPlayer.volume(20);
  // Pines sensor ultrasónico
  pinMode(PIN_VCC, OUTPUT);
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);
  digitalWrite(PIN_VCC, HIGH);  // dar alimentación al sensor

  Serial.println("Iniciando escaneo BLE...");
  BLEDevice::init("");
  pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true);  // true = más rápido, gasta más energía
  pBLEScan->setInterval(100);
  pBLEScan->setWindow(99);
}

void loop() {
  BLEScanResults* foundDevices = pBLEScan->start(1, false);
  pBLEScan->clearResults();  // limpia memoria
  //sensor distancia
  digitalWrite(PIN_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(PIN_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);
  duration = pulseIn(PIN_ECHO, HIGH);
  distance = duration * 0.034 / 2;
  if (distance >= 1 && distance <= 50) {
    Serial.print("Distancia gafas: ");
    Serial.println(distance);
    myDFPlayer.play(2);
  }
}
