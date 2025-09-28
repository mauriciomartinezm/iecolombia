#include "WiFi.h"
#include <chrono> // Para la biblioteca chrono

const char* targetSSID = "ESP8266_Server";  // Nombre de la red que se quiere detectar
const int buzzerPin = 19;
const int ledPin = 18;

void setup() {
  Serial.begin(115200);
  pinMode(buzzerPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  WiFi.mode(WIFI_STA);   // Modo estación (solo escaneo, no se conecta)
  WiFi.disconnect();     // Desconectado de cualquier red
  delay(100);
  Serial.println("Escaneando redes WiFi...");
}

void loop() {
  auto start = std::chrono::high_resolution_clock::now();

  int n = WiFi.scanNetworks();  // Escanea todas las redes
  auto end = std::chrono::high_resolution_clock::now();

  auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
  double time_in_seconds = duration.count() / 1000000.0;

  Serial.print("\nEscaneo completado, tiempo de escaneo: ");
  Serial.println(time_in_seconds);

  for (int i = 0; i < n; ++i) {
    String ssid = WiFi.SSID(i);
    int rssi = WiFi.RSSI(i); // Intensidad de señal
    
    Serial.printf("%d: %s (%d dBm)\n", i + 1, ssid.c_str(), rssi);
    
    if (ssid == targetSSID && rssi > -60) {
      Serial.printf("%d: %s (%d dBm)\n", i + 1, ssid.c_str(), rssi);
      Serial.println("Red objetivo detectada cerca");
      //tone(buzzerPin, 3000, 100);
      digitalWrite(ledPin, HIGH);
    }
    else if (ssid == targetSSID && rssi < -60){
      digitalWrite(ledPin, LOW);
    }
  }
}
