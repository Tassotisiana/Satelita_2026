
// #include "Sensor.h"
// #include <Adafruit_BMP280.h>

// class SensorBMP280 : public Sensor {
//   private:
//     Adafruit_BMP280 bmp;
//     float presionReferencia;
//     float ultimaAltitud;

//   public:
//     SensorBMP280();

//     bool inicializar() override;
//     float leer() override;        // devuelve presión en hPa

//     float leerTemperatura();      // temperatura interna (dato secundario/control)
//     float leerAltitud();          // altitud relativa al punto de partida
// };

#include "SensorBMP280.h"

SensorBMP280::SensorBMP280() : Sensor(0) {
  presionReferencia = 1013.25;
  ultimaAltitud = 0;
}

bool SensorBMP280::inicializar() {
  inicializado = bmp.begin(0x76) || bmp.begin(0x77);
  if (!inicializado) return false;

  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,
                   Adafruit_BMP280::SAMPLING_X2,
                   Adafruit_BMP280::SAMPLING_X16,
                   Adafruit_BMP280::FILTER_X16,
                   Adafruit_BMP280::STANDBY_MS_500);

  presionReferencia = bmp.readPressure() / 100.0F;
  return true;
}

float SensorBMP280::leer() {
  ultimaLectura = bmp.readPressure() / 100.0F;
  return ultimaLectura;
}

float SensorBMP280::leerTemperatura() {
  return bmp.readTemperature();
}

float SensorBMP280::leerAltitud() {
  ultimaAltitud = bmp.readAltitude(presionReferencia);
  return ultimaAltitud;
}