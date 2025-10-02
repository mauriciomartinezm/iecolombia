#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

void setup() {
  Serial.begin(9600);
  Serial.println("Iniciando ESP32 como servidor BLE...");

  // Inicializar BLE con el nombre que se anunciará
  BLEDevice::init("sala_sistema");

  // Crear un servidor BLE (aunque no tenga servicios, basta para anunciar)
  BLEServer *pServer = BLEDevice::createServer();

  // Configurar el advertising (anuncio)
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->start();

  Serial.println("✅ Anuncio iniciado como: sala_sistema");
}

void loop() {
  // Aquí no necesitas nada, solo anunciar constantemente
  delay(2000);
  Serial.println("📡 Anunciando como sala_sistema...");
}
