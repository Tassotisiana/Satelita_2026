#include "TimerConfig.h"

volatile bool banderaLectura = false;

// El ESP32 usa su propia API de timers (hw_timer_t), distinta a la
// del ESP8266 (timer1_write) y a la del Arduino clásico (TCCR1A).
hw_timer_t *timer = NULL;

// El ESP32 tiene DOS núcleos -- este spinlock evita que el núcleo que
// corre loop() y el que dispara la interrupción choquen leyendo la
// bandera al mismo tiempo. No hace falta esto en Arduino ni ESP8266,
// que son de un solo núcleo.
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

// IRAM_ATTR (equivalente al ICACHE_RAM_ATTR del ESP8266): obliga a que
// la interrupción viva en RAM, no en Flash.
void IRAM_ATTR onTimer() {
  portENTER_CRITICAL_ISR(&timerMux);
  banderaLectura = true;
  portEXIT_CRITICAL_ISR(&timerMux);
}

void configurarTimer() {
  // timerBegin(numero_timer, divisor, cuenta_ascendente). El ESP32
  // corre a 80MHz; con divisor 80, el timer cuenta en microsegundos.
  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, 3000000, true); // 3.000.000 us = 3 segundos, autoreload
  timerAlarmEnable(timer);
}