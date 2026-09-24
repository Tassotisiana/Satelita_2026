#ifndef SENSOR_LDR_H
#define SENSOR_LDR_H

#include "Sensor.h"

class SensorLDR : public Sensor {
  public:
    SensorLDR(uint8_t pinAsignado);

    bool inicializar() override;
    float leer() override; 
    
    float leerLux();
};

#endif