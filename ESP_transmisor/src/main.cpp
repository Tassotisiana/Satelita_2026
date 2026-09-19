#include <Arduino.h>
#include <Wire.h>

#include "Sensor.h"
#include "SensorBMP280.h"
#include "SensorDHT22.h"
#include "SensorLDR.h"
#include "SensorDS3231.h"
#include "Paquete.h"
#include "Transmisor.h"
#include "TimerConfig.h"

// ============================================================
//  ESP32 (satélite) - Lee los 4 sensores, arma el paquete (struct)
//  y lo transmite por NRF24 al ESP32 receptor.
// ============================================================

// ---------- Pines de los sensores (I2C y digitales/analógicos) ----------
#define PIN_SDA   21  // BMP280 y DS3231 comparten el mismo bus I2C
#define PIN_SCL   22
#define PIN_DHT22 27
#define PIN_LDR   34  // pin de ADC1 -- evitar ADC2, puede fallar con WiFi activo

// ---------- Pines del NRF24 (los mismos que ya confirmaste funcionando) ----------
#define PIN_CE   4
#define PIN_CSN  5
#define PIN_SCK  18
#define PIN_MISO 19
#define PIN_MOSI 23

// ---------- Objetos ----------
SensorBMP280 sensorPresion;
SensorDHT22  sensorHumedad(PIN_DHT22);
SensorLDR    sensorLuz(PIN_LDR);
SensorDS3231 sensorReloj;

Sensor* sensores[4] = { &sensorPresion, &sensorHumedad, &sensorLuz, &sensorReloj };
const uint8_t CANTIDAD_SENSORES = 4;

Paquete paquete;
Transmisor transmisor(PIN_CE, PIN_CSN, PIN_SCK, PIN_MISO, PIN_MOSI);

bool transmisorDisponible = false; // evita que enviar() se cuelgue si el módulo no inicializó bien

void setup() {
  Serial.begin(115200); // mismo baudrate que ya usabas en el test del NRF24
  delay(1000);

  Wire.begin(PIN_SDA, PIN_SCL);

  Serial.println();
  Serial.println(F("===================================="));
  Serial.println(F(" ESP32 (satélite) - CanSat"));
  Serial.println(F("===================================="));

  // Inicializamos los 4 objetos (3 sensores + el reloj) de forma
  // polimórfica, a través del array.
  for (uint8_t i = 0; i < CANTIDAD_SENSORES; i++) {
    if (!sensores[i]->inicializar()) {
      Serial.print(F("Error al inicializar sensor n° "));
      Serial.println(i);
    }
  }

  // Inicializamos el NRF24 con la configuración que ya confirmaste
  transmisorDisponible = transmisor.inicializar();
  if (!transmisorDisponible) {
    Serial.println(F("ERROR: NRF24 no detectado -- se omitirá la transmisión"));
  } else {
    Serial.println(F("NRF24 detectado y configurado (canal 108, PA_HIGH, 250KBPS)"));
  }

  configurarTimer(); // arranca el conteo de 3 segundos por hardware

  Serial.println(F("Sistema listo. Transmitiendo cada 3 segundos..."));
  Serial.println();
}

void loop() {
  if (banderaLectura) {
    banderaLectura = false;

    // Leemos los 4 objetos a través del array (polimorfismo)
    for (uint8_t i = 0; i < CANTIDAD_SENSORES; i++) {
      sensores[i]->leer();
    }

    float presion      = sensorPresion.obtenerUltimaLectura();
    float altitud       = sensorPresion.leerAltitud();
    float humedad       = sensorHumedad.obtenerUltimaLectura();
    float temperatura   = sensorHumedad.leerTemperatura();
    float luz           = sensorLuz.leerLux();
    uint32_t timestamp  = sensorReloj.obtenerTimestamp();

    // Armamos el paquete (struct) con todos los valores juntos
    paquete.actualizar(temperatura, humedad, presion, altitud, luz, timestamp);
    paquete.imprimir(); // para verificar por Serial que los valores estén bien

    // Solo intentamos transmitir si el módulo se inicializó bien --
    // llamar a enviar() con un módulo que nunca respondió puede colgar
    // el programa esperando una respuesta que nunca llega.
    if (transmisorDisponible) {
      PaqueteRadio datosParaEnviar = paquete.obtenerDatos();
      bool enviado = transmisor.enviar(&datosParaEnviar);

      if (enviado) {
        Serial.println(F("Transmision OK: el receptor confirmo recepcion (ACK)"));
      } else {
        Serial.println(F("Aviso: el receptor no confirmo recepcion del paquete"));
      }
    } else {
      Serial.println(F("Transmision omitida: modulo NRF24 no disponible"));
    }

    Serial.println();
  }
}