#include "Paquete.h"

Paquete::Paquete() {
  datos.temperatura = 0;
  datos.humedad = 0;
  datos.presion = 0;
  datos.altitud = 0;
  datos.luz = 0;
  datos.timestamp = 0;
}

void Paquete::actualizar(float temp, float hum, float pres, float alt, float lux, uint32_t timestamp) {
  datos.temperatura = temp;
  datos.humedad = hum;
  datos.presion = pres;
  datos.altitud = alt;
  datos.luz = lux;
  datos.timestamp = timestamp;
}

PaqueteRadio Paquete::obtenerDatos() {
  return datos;
}

void Paquete::imprimir() {
  Serial.print("T="); Serial.print(datos.temperatura);
  Serial.print(" H="); Serial.print(datos.humedad);
  Serial.print(" P="); Serial.print(datos.presion);
  Serial.print(" Alt="); Serial.print(datos.altitud);
  Serial.print(" Luz="); Serial.print(datos.luz);
  Serial.print(" TS="); Serial.println(datos.timestamp);
}