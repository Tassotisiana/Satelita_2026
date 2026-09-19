#ifndef SENSOR_BMP280_H
#define SENSOR_BMP280_H

#include "Sensor.h"
#include <Adafruit_BMP280.h>

class SensorBMP280 : public Sensor {
  private:
    Adafruit_BMP280 bmp;
    float presionReferencia;
    float ultimaAltitud;

  public:
    SensorBMP280();

    bool inicializar() override;
    float leer() override;        // devuelve presión en hPa

    float leerTemperatura();      // temperatura interna (dato secundario/control)
    float leerAltitud();          // altitud relativa al punto de partida
};

#endif