#include <Arduino.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define CE_PIN   4
#define CSN_PIN  5

#define SCK_PIN  18
#define MISO_PIN 19
#define MOSI_PIN 23

RF24 radio(CE_PIN, CSN_PIN);

// Misma dirección que usa Transmisor.cpp del satélite
const byte address[6] = "RNG01";

// MISMO struct, campo por campo, que Paquete.h del satélite.
// El "packed" es obligatorio también acá: sin él, el compilador
// podría alinear los campos distinto y los bytes recibidos se
// interpretarían mal (temperatura mezclada con humedad, etc).
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

void setup() {

  Serial.begin(115200);
  delay(1000);

  Serial.println();
  Serial.println("====================================");
  Serial.println(" NRF24 - Estacion en tierra (ESP32)");
  Serial.println("====================================");

  SPI.begin(SCK_PIN, MISO_PIN, MOSI_PIN, CSN_PIN);

  if (!radio.begin()) {
    Serial.println();
    Serial.println("ERROR: NRF24 NO DETECTADO");
    while (1) { delay(1000); }
  }

  Serial.println();
  Serial.println("NRF24 detectado");

  // ===============================
  // CONFIGURACION RF - IDENTICA a Transmisor.cpp
  // ===============================
  radio.setPALevel(RF24_PA_HIGH);
  radio.setDataRate(RF24_250KBPS);
  radio.setChannel(108);
  radio.setPayloadSize(sizeof(PaqueteRadio));

  radio.openReadingPipe(1, address);
  radio.startListening(); // modo receptor

  Serial.println();
  Serial.println("Configuracion:");
  Serial.println("PA Level  : HIGH");
  Serial.println("Data Rate : 250 KBPS");
  Serial.println("Channel   : 108");
  Serial.println("Address   : RNG01");
  Serial.print("Payload   : ");
  Serial.print(sizeof(PaqueteRadio));
  Serial.println(" bytes");

  Serial.println();
  Serial.println("Esperando telemetria del satelite...");
  Serial.println();

  ultimaRecepcion = millis();
}

void loop() {

  if (radio.available()) {

    radio.read(&datosRecibidos, sizeof(datosRecibidos));
    paquetesRecibidos++;

    Serial.print("T=");    Serial.print(datosRecibidos.temperatura);
    Serial.print(" H=");   Serial.print(datosRecibidos.humedad);
    Serial.print(" P=");   Serial.print(datosRecibidos.presion);
    Serial.print(" Alt="); Serial.print(datosRecibidos.altitud);
    Serial.print(" Luz="); Serial.print(datosRecibidos.luz);
    Serial.print(" TS=");  Serial.print(datosRecibidos.timestamp);
    Serial.print(" | # "); Serial.println(paquetesRecibidos);

    ultimaRecepcion = millis();
  }

  if (millis() - ultimaRecepcion >= 5000) {
    Serial.println("SIN PAQUETES - esperando...");
    ultimaRecepcion = millis();
  }
}