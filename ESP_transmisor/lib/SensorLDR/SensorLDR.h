#ifndef SENSOR_LDR_H
#define SENSOR_LDR_H

#include "Sensor.h"

class SensorLDR : public Sensor {
  public:
    SensorLDR(uint8_t pinAsignado);

    bool inicializar() override;
    float leer() override; 

    // Convierte el valor crudo a una escala aproximada de "lux".
    // En este cableado, a MÁS luz el ADC da un valor MÁS BAJO, por eso
    // la conversión es invertida: ADC máximo (oscuridad) -> 0 lux aprox,
    // ADC mínimo (luz máxima) -> 6000 lux aprox. Es una escala relativa/estimada.
    float leerLux();
};

#endif