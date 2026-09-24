#ifndef PAQUETE_H
#define PAQUETE_H

#include <Arduino.h>


struct __attribute__((packed)) PaqueteRadio {
  float temperatura;   // 4 bytes
  float humedad;       // 4 bytes
  float presion;       // 4 bytes
  float altitud;       // 4 bytes
  float luz;           // 4 bytes
  uint32_t timestamp;  // 4 bytes
  // Total: 24 bytes
};

// Clase que agrupa los valores actuales y arma el struct final.
class Paquete {
  private:
    PaqueteRadio datos;

  public:
    Paquete();

    // Carga los valores actuales en el struct interno
    void actualizar(float temp, float hum, float pres, float alt, float lux, uint32_t timestamp);

    // Devuelve una copia del struct, lista para pasarle a radio.write()
    PaqueteRadio obtenerDatos();

    // Imprime el contenido por Serial, para debug mientras no está el NRF24
    void imprimir();
};

#endif