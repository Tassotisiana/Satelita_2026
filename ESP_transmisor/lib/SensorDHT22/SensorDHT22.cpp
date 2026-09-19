#include "SensorDHT22.h"

SensorDHT22::SensorDHT22(uint8_t pinAsignado) : Sensor(pinAsignado), dht(pinAsignado, DHT22) {}

bool SensorDHT22::inicializar() {
  dht.begin();
  delay(100);
  inicializado = true;
  return inicializado;
}

float SensorDHT22::leer() {
  float humedad = dht.readHumidity();
  if (!isnan(humedad)) {
    ultimaLectura = humedad;
  }
  return ultimaLectura;
}

float SensorDHT22::leerTemperatura() {
  return dht.readTemperature(); // puede devolver NAN si falla, chequear al usarlo
}