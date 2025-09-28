#include <esp_now.h>
#include <WiFi.h>

typedef struct struct_message {
  float a;
} struct_message;

struct_message myData;

const int voltPin = 2;
const int trigPin = 5;
const int echoPin = 4;
const int ledPin = 18;
const int buzzerPin = 19;

float distance;
float duration;

bool bastonDetecta = false;
bool gafasDetecta = false;

void setup() {
  Serial.begin(9600);
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  pinMode(trigPin, OUTPUT);
  pinMode(voltPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(voltPin, HIGH);

  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecv));
}
// callback function that will be executed when data is received
void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.print("Distancia bastón: ");
  Serial.println(myData.a);
  if (myData.a >= 1 && myData.a <= 30) {
    bastonDetecta = true;
  } else {
    bastonDetecta = false;
  }
}

void loop() {
  //digitalWrite(trigPin, HIGH);
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;

  if (distance >= 1 && distance <= 50) {
    gafasDetecta = true;
    Serial.print("Distancia gafas: ");
    Serial.println(distance);
  } else {
    gafasDetecta = false;
  }
  // Lógica de activación de sonidos y LED
  if (bastonDetecta && gafasDetecta) {
    // Ambos detectan → sonido especial
    digitalWrite(ledPin, HIGH);
    tone(buzzerPin, 3000, 100);
  } else if (bastonDetecta) {
    // Solo bastón detecta
    //digitalWrite(ledPin, HIGH);
    tone(buzzerPin, 2000, 100);
  } else if (gafasDetecta) {
    // Solo gafas detecta
    digitalWrite(ledPin, HIGH);
    tone(buzzerPin, 1000, 100);
  } else {
    // Ninguno detecta
    noTone(buzzerPin);
    digitalWrite(ledPin, LOW);
  }

  delay(200);  // Pequeño delay para estabilidad
}