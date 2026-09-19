#ifndef SENSOR_DHT22_H
#define SENSOR_DHT22_H

#include "Sensor.h"
#include <DHT.h>

class SensorDHT22 : public Sensor {
  private:
    DHT dht;

  public:
    SensorDHT22(uint8_t pinAsignado);

    bool inicializar() override;
    float leer() override;        // devuelve humedad en %
    float leerTemperatura();      // temperatura en °C 
};

#endif