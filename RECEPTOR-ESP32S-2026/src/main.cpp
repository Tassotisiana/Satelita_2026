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

const byte address[6] = "RNG01";

struct DataPacket {
  uint32_t packetNumber;
  int16_t testValue;
};

DataPacket data;

uint32_t packetsReceived = 0;
uint32_t lastPacketNumber = 0;
uint32_t packetsLost = 0;

unsigned long lastReceiveTime = 0;

void setup() {

  Serial.begin(115200);
  delay(1000);

  Serial.println();
  Serial.println("====================================");
  Serial.println(" NRF24 - RX ESP32-S - COM9");
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

  radio.setPayloadSize(sizeof(data));

  // PIPE 1
  radio.openReadingPipe(1, address);

  radio.startListening();

  Serial.println();
  Serial.println("Configuracion:");
  Serial.println("PA Level  : HIGH");
  Serial.println("Data Rate : 250 KBPS");
  Serial.println("Channel   : 108");
  Serial.println("Address   : RNG01");
  Serial.println("Pipe      : 1");

  Serial.print("Payload   : ");
  Serial.print(sizeof(data));
  Serial.println(" bytes");

  Serial.println("ACK       : ACTIVADO");

  Serial.println();
  Serial.println("Esperando transmisor...");
  Serial.println();

  lastReceiveTime = millis();
}

void loop() {

  if (radio.available()) {

    radio.read(&data, sizeof(data));

    packetsReceived++;

    if (lastPacketNumber > 0) {

      uint32_t diferencia =
          data.packetNumber - lastPacketNumber;

      if (diferencia > 1) {
        packetsLost += diferencia - 1;
      }
    }

    lastPacketNumber = data.packetNumber;

    Serial.print("RECIBIDO >>> Packet #");
    Serial.print(data.packetNumber);

    Serial.print(" | Value: ");
    Serial.print(data.testValue);

    Serial.print(" | Recibidos: ");
    Serial.print(packetsReceived);

    Serial.print(" | Perdidos: ");
    Serial.println(packetsLost);

    lastReceiveTime = millis();
  }

  if (millis() - lastReceiveTime >= 5000) {

    Serial.println("SIN PAQUETES - esperando...");

    lastReceiveTime = millis();
  }
}