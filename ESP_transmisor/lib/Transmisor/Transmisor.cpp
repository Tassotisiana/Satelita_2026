#include "Transmisor.h"

// ============================================================
// CONSTRUCTOR
// ============================================================

Transmisor::Transmisor(
    uint8_t pinCE,
    uint8_t pinCSN,
    uint8_t sck,
    uint8_t miso,
    uint8_t mosi
)
    : radio(pinCE, pinCSN)
{
    pinSCK = sck;
    pinMISO = miso;
    pinMOSI = mosi;

    this->pinCSN = pinCSN;

    // ========================================================
    // DIRECCION NRF24 = "RNG01"
    // ========================================================

    direccion[0] = 'R';
    direccion[1] = 'N';
    direccion[2] = 'G';
    direccion[3] = '0';
    direccion[4] = '1';
    direccion[5] = '\0';
}


// ============================================================
// INICIALIZAR NRF24
// ============================================================

bool Transmisor::inicializar() {

    // Inicializar SPI del ESP32-S
    SPI.begin(
        pinSCK,
        pinMISO,
        pinMOSI,
        pinCSN
    );

    // Inicializar NRF24
    if (!radio.begin()) {

        return false;
    }


    // ========================================================
    // CONFIGURACION RF
    // DEBE COINCIDIR CON EL RECEPTOR
    // ========================================================

    // Potencia
    radio.setPALevel(RF24_PA_HIGH);

    // Velocidad de transmision
    radio.setDataRate(RF24_250KBPS);

    // Canal
    radio.setChannel(108);

    // Tamaño del paquete
    radio.setPayloadSize(sizeof(PaqueteRadio));

    // Reintentos para ACK
    radio.setRetries(5, 15);

    // Direccion de destino
    radio.openWritingPipe(direccion);

    // Modo transmisor
    radio.stopListening();

    return true;
}


// ============================================================
// ENVIAR PAQUETE
// ============================================================

bool Transmisor::enviar(const PaqueteRadio &paquete) {

    bool enviado = radio.write(
        &paquete,
        sizeof(PaqueteRadio)
    );

    return enviado;
}