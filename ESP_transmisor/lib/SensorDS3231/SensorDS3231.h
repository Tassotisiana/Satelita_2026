#ifndef SENSOR_DS3231_H
#define SENSOR_DS3231_H

#include "Sensor.h"
#include <RTClib.h>

class SensorDS3231 : public Sensor {
  private:
    RTC_DS3231 rtc;
    uint32_t ultimoTimestamp;

  public:
    SensorDS3231();

    bool inicializar() override;

    // Devuelve el timestamp como float SOLO para cumplir con la
    // interfaz común de Sensor. 
    float leer() override;

    // Valor a usar en el paquete: un entero de 32 bits. 
    uint32_t obtenerTimestamp();
};

#endif