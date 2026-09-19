#include "Transmisor.h"

Transmisor::Transmisor(uint8_t pinCE, uint8_t pinCSN, uint8_t sck, uint8_t miso, uint8_t mosi)
  : radio(pinCE, pinCSN) {
  pinSCK = sck;
  pinMISO = miso;
  pinMOSI = mosi;
  this->pinCSN = pinCSN;
}

bool Transmisor::inicializar() {
  // En el ESP32 hay que indicarle a SPI qué pines usar explícitamente
  // (aunque coincidan con los del bus VSPI por default, como en este caso).
  SPI.begin(pinSCK, pinMISO, pinMOSI, pinCSN);

  if (!radio.begin()) {
    return false;
  }

  // Misma configuración que ya confirmaron funcionando en el test:
  radio.setPALevel(RF24_PA_HIGH);
  radio.setDataRate(RF24_250KBPS);
  radio.setChannel(108);
  radio.setRetries(5, 15);
  radio.setPayloadSize(sizeof(PaqueteRadio)); // 24 bytes -- entra cómodo en el límite de 32
  radio.openWritingPipe(direccion);
  radio.stopListening();

  return true;
}

bool Transmisor::enviar(PaqueteRadio* datos) {
  return radio.write(datos, sizeof(PaqueteRadio));
}