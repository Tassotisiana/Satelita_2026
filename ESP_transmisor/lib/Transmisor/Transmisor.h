#ifndef TRANSMISOR_H
#define TRANSMISOR_H

#include <Arduino.h>
#include <SPI.h>
#include <RF24.h>
#include "Paquete.h" // necesita conocer el struct PaqueteRadio para poder enviarlo

// Encapsula el módulo NRF24: el resto del programa no necesita saber
// nada de SPI, canales de radio ni direcciones -- solo llama
// inicializar() y enviar(). Configuración basada en el test que ya
// confirmaron funcionando (canal 108, PA_HIGH, 250KBPS).
class Transmisor {
  private:
    RF24 radio;
    const byte direccion[6] = "RNG01"; // debe coincidir con el receptor

    uint8_t pinSCK, pinMISO, pinMOSI, pinCSN;

  public:
    // pinCE y pinCSN son obligatorios para el objeto RF24. Los pines
    // de SPI (SCK/MISO/MOSI) se guardan aparte porque en el ESP32 hay
    // que pasárselos explícitamente a SPI.begin().
    Transmisor(uint8_t pinCE, uint8_t pinCSN, uint8_t sck, uint8_t miso, uint8_t mosi);

    bool inicializar();

    // Recibe un puntero al struct armado por la clase Paquete y lo
    // transmite tal cual, como bytes crudos.
    bool enviar(PaqueteRadio* datos);
};

#endif