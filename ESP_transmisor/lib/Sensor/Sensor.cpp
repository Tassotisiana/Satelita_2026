#include "Sensor.h"

Sensor::Sensor(uint8_t pinAsignado) {
  pin = pinAsignado;
  ultimaLectura = 0.0;
  inicializado = false;
}

float Sensor::obtenerUltimaLectura() {
  return ultimaLectura;
}

bool Sensor::estaInicializado() {
  return inicializado;
}

Sensor::~Sensor() {}