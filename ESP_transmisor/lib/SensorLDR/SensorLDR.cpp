#include "SensorLDR.h"

SensorLDR::SensorLDR(uint8_t pinAsignado) : Sensor(pinAsignado) {}

bool SensorLDR::inicializar() {
  pinMode(pin, INPUT);
  inicializado = true;
  return inicializado;
}

float SensorLDR::leer() {
  ultimaLectura = analogRead(pin);
  return ultimaLectura;
}

float SensorLDR::leerLux() {
  const float ADC_MAXIMO = 4095.0F;
  const float LUX_MAXIMO = 6000.0F;

  float valorCrudo = analogRead(pin);
  ultimaLectura = valorCrudo;

  float lux = (ADC_MAXIMO - valorCrudo) * (LUX_MAXIMO / ADC_MAXIMO);

  if (lux < 0) lux = 0;
  if (lux > LUX_MAXIMO) lux = LUX_MAXIMO;

  return lux;
}