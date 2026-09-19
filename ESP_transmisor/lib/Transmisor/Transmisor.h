#ifndef TRANSMISOR_H
#define TRANSMISOR_H

#include <Arduino.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#include "Paquete.h"

// ============================================================
// CLASE TRANSMISOR NRF24
// ============================================================

class Transmisor {

private:

    RF24 radio;

    // Direccion NRF24
    byte direccion[6];

    // Pines SPI
    uint8_t pinSCK;
    uint8_t pinMISO;
    uint8_t pinMOSI;
    uint8_t pinCSN;

public:

    // Constructor
    Transmisor(
        uint8_t pinCE,
        uint8_t pinCSN,
        uint8_t sck,
        uint8_t miso,
        uint8_t mosi
    );

    // Inicializar NRF24
    bool inicializar();

    // Enviar paquete
    bool enviar(const PaqueteRadio &paquete);
};

#endif