#include <Arduino.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <math.h>

#define CE_PIN   4
#define CSN_PIN  5

#define SCK_PIN  18
#define MISO_PIN 19
#define MOSI_PIN 23

RF24 radio(CE_PIN, CSN_PIN);

const byte address[6] = "RNG01";

struct DataPacket {
  uint32_t packetNumber;
  int16_t testValue;
};

DataPacket data;

uint32_t packetCount = 0;

void setup() {

  Serial.begin(115200);
  delay(1000);

  Serial.println();
  Serial.println("====================================");
  Serial.println(" NRF24 - TX ESP32-S - COM4");
  Serial.println("====================================");

  Serial.println("CE   = GPIO4");
  Serial.println("CSN  = GPIO5");
  Serial.println("SCK  = GPIO18");
  Serial.println("MISO = GPIO19");
  Serial.println("MOSI = GPIO23");

  SPI.begin(SCK_PIN, MISO_PIN, MOSI_PIN, CSN_PIN);

  if (!radio.begin()) {

    Serial.println();
    Serial.println("ERROR: NRF24 NO DETECTADO");

    while (1) {
      delay(1000);
    }
  }

  Serial.println();
  Serial.println("NRF24 detectado");

  Serial.print("Chip conectado: ");
  Serial.println(radio.isChipConnected() ? "SI" : "NO");

  // ===============================
  // CONFIGURACION RF
  // ===============================

  radio.setPALevel(RF24_PA_HIGH);

  radio.setDataRate(RF24_250KBPS);

  radio.setChannel(108);

  radio.setRetries(5, 15);

  radio.setPayloadSize(sizeof(data));

  radio.openWritingPipe(address);

  radio.stopListening();

  Serial.println();
  Serial.println("Configuracion:");
  Serial.println("PA Level  : HIGH");
  Serial.println("Data Rate : 250 KBPS");
  Serial.println("Channel   : 108");
  Serial.println("Address   : RNG01");

  Serial.print("Payload   : ");
  Serial.print(sizeof(data));
  Serial.println(" bytes");

  Serial.println("ACK       : ACTIVADO");

  Serial.println();
  Serial.println("Transmitiendo...");
  Serial.println();
}

void loop() {

  packetCount++;

  data.packetNumber = packetCount;

  data.testValue =
      (int16_t)(sin(packetCount * 0.1) * 1000);

  bool success =
      radio.write(&data, sizeof(data));

  Serial.print("Packet #");
  Serial.print(data.packetNumber);

  Serial.print(" | Value: ");
  Serial.print(data.testValue);

  Serial.print(" | Status: ");

  if (success) {
    Serial.println("ACK RECEIVED");
  }
  else {
    Serial.println("FAILED - NO ACK");
  }

  delay(1000);
}