#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include "Arduino.h"
#include "DFRobotDFPlayerMini.h"

DFRobotDFPlayerMini myDFPlayer;

const char* targetDeviceName = "sala_sistema";
//const int buzzerPin = 19;
//const int ledPin = 18;

int scanTime = 1;
BLEScan* pBLEScan;

class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks {
  void onResult(BLEAdvertisedDevice advertisedDevice) {
    String deviceName = advertisedDevice.getName().c_str();
    //Serial.print("Nombre dispositivos: ");
    //Serial.println(deviceName);
    int rssi = advertisedDevice.getRSSI();
    //Serial.printf("Encontrado %s con RSSI: %d dBm\n", deviceName, rssi);
    if (deviceName == targetDeviceName) {
      Serial.printf("Encontrado %s con RSSI: %d dBm\n", deviceName, rssi);

      if (rssi > -50) {  // señal fuerte = cerca
        //digitalWrite(ledPin, HIGH);
        //tone(buzzerPin, 3000, 200); // beep corto
        Serial.println("Dispositivo objetivo CERCA");
        myDFPlayer.play(1);
      } else {
        //digitalWrite(ledPin, LOW);
        Serial.println("Dispositivo detectado, pero señal débil");
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
  int total = myDFPlayer.readFileCounts();
  int actual = myDFPlayer.readCurrentFileNumber();
  Serial.print("Archivos totales: ");
  Serial.println(total);

  //pinMode(buzzerPin, OUTPUT);
  //pinMode(ledPin, OUTPUT);

  Serial.println("Iniciando escaneo BLE...");

  BLEDevice::init("");
  pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true);  // true = más rápido, gasta más energía
  pBLEScan->setInterval(100);
  pBLEScan->setWindow(99);
}

void loop() {
  //puntero en lugar de objeto
  BLEScanResults* foundDevices = pBLEScan->start(1, false);
  int count = foundDevices->getCount();

  //Serial.printf("Escaneo completado. %d dispositivos encontrados\n", count);
  pBLEScan->clearResults();  // limpia memoria
}