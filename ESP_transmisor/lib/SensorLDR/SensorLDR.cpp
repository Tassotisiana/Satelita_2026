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
  // OJO: el ADC del ESP32 tiene 12 bits (0-4095), a diferencia de los
  // 10 bits (0-1023) del ESP8266 -- este es el único valor que cambió
  // al pasar de una placa a la otra.
  const float ADC_MAXIMO = 4095.0F;
  const float LUX_MAXIMO = 6000.0F;

  float valorCrudo = analogRead(pin);
  ultimaLectura = valorCrudo;

  float lux = (ADC_MAXIMO - valorCrudo) * (LUX_MAXIMO / ADC_MAXIMO);

  if (lux < 0) lux = 0;
  if (lux > LUX_MAXIMO) lux = LUX_MAXIMO;

  return lux;
}