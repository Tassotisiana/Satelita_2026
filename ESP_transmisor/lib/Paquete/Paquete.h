#ifndef PAQUETE_H
#define PAQUETE_H

#include <Arduino.h>

// Struct que se va a mandar tal cual, como bytes crudos, por NRF24.
// "packed" evita que el compilador agregue relleno (padding) entre
// campos para alinearlos en memoria -- así el struct ocupa EXACTAMENTE
// la suma de sus campos, y significa lo mismo en el ESP8266 (transmisor)
// y en el ESP32 (receptor), aunque sean arquitecturas distintas.
struct __attribute__((packed)) PaqueteRadio {
  float temperatura;   // 4 bytes
  float humedad;       // 4 bytes
  float presion;       // 4 bytes
  float altitud;       // 4 bytes
  float luz;           // 4 bytes
  uint32_t timestamp;  // 4 bytes
  // Total: 24 bytes -- entra cómodo en el límite de 32 bytes del NRF24
};

// Clase que agrupa los valores actuales y arma el struct final.
// No hereda de Sensor: no es un sensor, es un objeto que combina
// los datos de todos los sensores en un solo paquete.
class Paquete {
  private:
    PaqueteRadio datos;

  public:
    Paquete();

    // Carga los valores actuales en el struct interno
    void actualizar(float temp, float hum, float pres, float alt, float lux, uint32_t timestamp);

    // Devuelve una copia del struct, lista para pasarle a radio.write()
    // cuando agreguemos el Transmisor (radio.write(&paqueteRadio, sizeof(paqueteRadio)))
    PaqueteRadio obtenerDatos();

    // Imprime el contenido por Serial, para debug mientras no está el NRF24
    void imprimir();
};

#endif