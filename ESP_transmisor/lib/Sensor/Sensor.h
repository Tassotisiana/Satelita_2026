#ifndef SENSOR_H
#define SENSOR_H

#include <Arduino.h>

// Clase base ABSTRACTA para todos los sensores (y el reloj) del ESP8266.
class Sensor {
  protected:
    uint8_t pin;          // pin al que está conectado (0 si usa I2C)
    float ultimaLectura;  // último valor leído
    bool inicializado;    // si inicializar() se ejecutó con éxito

  public:
    Sensor(uint8_t pinAsignado);

    virtual bool inicializar() = 0;
    virtual float leer() = 0;

    float obtenerUltimaLectura();
    bool estaInicializado();

    virtual ~Sensor();
};

#endif