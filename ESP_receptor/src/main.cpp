#include <Arduino.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <WiFi.h>
#include <PubSubClient.h>

// ============================================================
//  Estación en tierra (ESP32) - Recibe por NRF24, arma el JSON
//  y lo publica por MQTT al broker de la compu del grupo.
// ============================================================

// ---------- NRF24 ----------
#define CE_PIN   4
#define CSN_PIN  5
#define SCK_PIN  18
#define MISO_PIN 19
#define MOSI_PIN 23

RF24 radio(CE_PIN, CSN_PIN);
const byte address[6] = "RNG01"; // misma dirección que el satélite

// MISMO struct, campo por campo, que Paquete.h del satélite.
struct __attribute__((packed)) PaqueteRadio {
  float temperatura;
  float humedad;
  float presion;
  float altitud;
  float luz;
  uint32_t timestamp;
};

PaqueteRadio datosRecibidos;
uint32_t paquetesRecibidos = 0;
unsigned long ultimaRecepcion = 0;

// ---------- WiFi ----------
//wifi de la facu
//const char* ssid = "Alumnos2.4GHz";
//const char* password = "alumnosfcyt";

//wifi cata
const char* ssid = "Personal-FF0-2.4GHz";
const char* password = "EE0E42BFF0";

// ---------- MQTT ----------
//const char* mqtt_server = "172.21.2.77"; // IP de la compu de Tisi
const char* mqtt_server = "192.168.0.15";  // IP de la compu de Cata
const int mqtt_port = 1883;
const char* mqtt_topic = "cansat/datos";

WiFiClient espClient;
PubSubClient client(espClient);

void conectarWiFi() {
  Serial.print(F("Conectando al WiFi"));
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(F("."));
  }

  Serial.println();
  Serial.println(F("WiFi conectado"));
  Serial.print(F("IP del ESP32-S: "));
  Serial.println(WiFi.localIP());
}

void conectarMQTT() {
  while (!client.connected()) {
    Serial.print(F("Conectando a Mosquitto..."));

    if (client.connect("ESP32_Receptor_CanSat")) {
      Serial.println(F(" conectado"));
    } else {
      Serial.print(F(" fallo, rc="));
      Serial.print(client.state());
      Serial.println(F(" - reintentando en 2 segundos"));
      delay(2000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println();
  Serial.println(F("===================================="));
  Serial.println(F(" NRF24 -> WiFi -> MQTT - Estacion en tierra"));
  Serial.println(F("===================================="));

  // ---------- Radio NRF24 ----------
  SPI.begin(SCK_PIN, MISO_PIN, MOSI_PIN, CSN_PIN);

  if (!radio.begin()) {
    Serial.println();
    Serial.println(F("ERROR: NRF24 NO DETECTADO"));
    while (1) { delay(1000); }
  }

  Serial.println(F("NRF24 detectado"));

  radio.setPALevel(RF24_PA_HIGH);
  radio.setDataRate(RF24_250KBPS);
  radio.setChannel(108);
  radio.setPayloadSize(sizeof(PaqueteRadio));
  radio.setAutoAck(false);

  radio.openReadingPipe(1, address);
  radio.startListening();

  Serial.println(F("Configuracion RF:"));
  Serial.println(F("PA Level  : HIGH"));
  Serial.println(F("Data Rate : 250 KBPS"));
  Serial.println(F("Channel   : 108"));
  Serial.println(F("Address   : RNG01"));
  Serial.println(F("Auto-ACK  : DESACTIVADO"));

  // ---------- WiFi + MQTT ----------
  conectarWiFi();
  client.setServer(mqtt_server, mqtt_port);

  Serial.println();
  Serial.println(F("Esperando telemetria del satelite..."));
  Serial.println();

  ultimaRecepcion = millis();
}

void loop() {
  // Mantenemos WiFi y MQTT conectados en todo momento
  if (!client.connected()) {
    conectarMQTT();
  }
  client.loop();

  if (radio.available()) {
    radio.read(&datosRecibidos, sizeof(datosRecibidos));
    paquetesRecibidos++;

    Serial.print(F("T=")); Serial.print(datosRecibidos.temperatura);
    Serial.print(F(" H=")); Serial.print(datosRecibidos.humedad);
    Serial.print(F(" P=")); Serial.print(datosRecibidos.presion);
    Serial.print(F(" Alt=")); Serial.print(datosRecibidos.altitud);
    Serial.print(F(" Luz=")); Serial.print(datosRecibidos.luz);
    Serial.print(F(" TS=")); Serial.print(datosRecibidos.timestamp);
    Serial.print(F(" | # ")); Serial.println(paquetesRecibidos);

    // Armamos el JSON con los mismos nombres que las columnas de Supabase
    char json[180];
    snprintf(json, sizeof(json),
             "{\"temperatura\":%.1f,\"humedad\":%.1f,\"presion\":%.1f,\"altitud\":%.1f,\"luz\":%.0f,\"timestamp\":%lu}",
             (double)datosRecibidos.temperatura, (double)datosRecibidos.humedad,
             (double)datosRecibidos.presion, (double)datosRecibidos.altitud,
             (double)datosRecibidos.luz, (unsigned long)datosRecibidos.timestamp);

    client.publish(mqtt_topic, json);
    Serial.print(F("Publicado por MQTT: "));
    Serial.println(json);

    ultimaRecepcion = millis();
  }

  if (millis() - ultimaRecepcion >= 5000) {
    Serial.println(F("SIN PAQUETES - esperando..."));
    ultimaRecepcion = millis();
  }
}