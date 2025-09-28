#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char* ssid = "ESP8266_Server";   // Nombre de la red WiFi
const char* password = "12345678";     // Clave de la red

ESP8266WebServer server(80);

void handleRoot() {
  server.send(200, "text/plain", "Hola desde ESP8266 como servidor!");
}

void setup() {
  Serial.begin(9600);

  // Modo Access Point
  WiFi.softAP(ssid, password);

  Serial.println("Servidor iniciado");
  Serial.print("IP del ESP8266: ");
  Serial.println(WiFi.softAPIP());

  // Rutas del servidor
  server.on("/", handleRoot);
  server.begin();
}

void loop() {
  server.handleClient();
}
