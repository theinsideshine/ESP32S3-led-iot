/**
   File: esp32S3-led-iot.ino
   Arquitectura embebida con control por Blynk
   - Compiler:           Arduino IDE 2.3.3
   - Device:             ESP32S3 DEV Module
   - Autor:              educacion.ta@gmail.com
   - Fecha:              24-05-2025

   https://github.com/blynkkk/blynk-library/issues/312
   https://community.blynk.cc/t/blynk-multiple-definitions/31450

   C:\Users\ptavolaro\AppData\Local\arduino\sketches
*/

#define BLYNK_TEMPLATE_ID "TMPL2HAGgrm8D"
#define BLYNK_TEMPLATE_NAME "Control RGB ESP32"
#define BLYNK_AUTH_TOKEN "P7gOcYiNkVWtpIP4za5LeRr3vZ20UDwU"

#include <BlynkSimpleEsp32.h>

#include "cfg_iot.h"
#include "log.h"
#include "led.h"
#include "timer.h"
#include "precompilation.h"

const char* ssid = "Pablo";
const char* password = "01410398716";
char auth[] = BLYNK_AUTH_TOKEN;

Clog    Log;
CIotConfig Config;
CLed     Led;

#define ST_LOOP_INIT           0
#define ST_LOOP_IDLE           1
#define ST_LOOP_LED_ON         2
#define ST_LOOP_LED_OFF        3
#define ST_LOOP_END            4
#define ST_MODE_RUN_DEMO1      5

static bool experimentoActivo = false;

static void run_demo_serial_plotter(void);
static void end_experiment(void) {
  Log.msg(F("Ensayo terminado"));
  experimentoActivo = false;
  Config.set_st_test(false);
  Config.send_test_finish();
  Blynk.virtualWrite(V0, 0);
}

// --- BLYNK ---
BLYNK_WRITE(V0) {
  if (!experimentoActivo) {
    experimentoActivo = param.asInt();
    Config.set_st_test(experimentoActivo);
  }
}

BLYNK_WRITE(V1) {
  if (!experimentoActivo) {
    Config.set_led_color(param.asInt());
  }
}

BLYNK_WRITE(V2) {
  if (!experimentoActivo) {
    Config.set_led_blink_time(param.asInt() * 1000);
  }
}

BLYNK_WRITE(V3) {
  if (!experimentoActivo) {
    Config.set_led_blink_quantity(param.asInt());
  }
}

BLYNK_WRITE(V4) {
  if (!experimentoActivo) {
    Config.set_log_level(param.asInt());
  }
}

BLYNK_WRITE(V5) {
  if (!experimentoActivo) {
    Config.set_st_mode(param.asInt());
  }
}


void run_demo_serial_plotter(void) {
  uint16_t raw, filtered, danger_point = 2500;
  uint8_t state = 0;

  Led.blink(5, 500);
  for (raw = 0; raw < 5000; raw++) {
    filtered = raw + 500;
    if (filtered > danger_point) state = 1;
    Log.ctrl(raw, filtered, state, danger_point);
  }
  Config.set_st_mode(ST_MODE_TEST);
}

void setup() {
  Serial.begin(115200);
  delay(100);
  Config.init();
  Log.init(Config.get_log_level());

  Serial.println("Init Serial");

  /*
      Para activar la visualisacion  enviar por serie {log_level:'1'}
  */

  Log.msg(F("%s "), FIRMWARE_VERSION);
  Log.msg(F("%s "), FECHA_VERSION);
 

  // --- CONEXIÓN BLYNK (reemplaza WiFi.begin + server web) ---
  Blynk.begin(auth, ssid, password);

  Serial.println("Conectado a Wi-Fi (vía Blynk)");
  Serial.print("Dirección IP: ");
  Serial.println(WiFi.localIP());

  Led.init();
  Led.set_color(1);
  Led.on();
  delay(1000);
  Led.off();
  Serial.print("Log level inicial: ");
  Serial.println(Config.get_log_level());

  Log.msg(F("Sistema inicializado"));
}

void loop() {
  static uint8_t st_loop = ST_LOOP_INIT;
  static CTimer Timer_led;
  static uint32_t led_blink_qty = 2;

  if (!experimentoActivo) {
    Blynk.run();
  }

  Log.set_level(Config.get_log_level());
  Led.set_color(Config.get_led_color());

  switch (st_loop) {
    case ST_LOOP_INIT:
      if (Config.get_st_test()) {
        experimentoActivo = true;
        st_loop = ST_LOOP_IDLE;
      } else if (Config.get_st_mode() == ST_MODE_DEMO) {
        st_loop = ST_MODE_RUN_DEMO1;
      }
      break;

    case ST_LOOP_IDLE:
      led_blink_qty = Config.get_led_blink_quantity();
      Timer_led.start();
      Led.on();
      Log.msg(F("LED ON"));
      st_loop = ST_LOOP_LED_ON;
      break;

    case ST_LOOP_LED_ON:
      if (Timer_led.expired(Config.get_led_blink_time())) {
        Log.msg(F("LED OFF"));
        Led.off();
        Timer_led.start();
        st_loop = ST_LOOP_LED_OFF;
      }
      break;

    case ST_LOOP_LED_OFF:
      if (Timer_led.expired(Config.get_led_blink_time())) {
        led_blink_qty--;
        if (led_blink_qty > 0) {
          Led.on();
          Log.msg(F("LED ON"));
          Timer_led.start();
          st_loop = ST_LOOP_LED_ON;
        } else {
          st_loop = ST_LOOP_END;
        }
      }
      break;

    case ST_LOOP_END:
      end_experiment();
      st_loop = ST_LOOP_INIT;
      break;

    case ST_MODE_RUN_DEMO1:
      run_demo_serial_plotter();
      st_loop = ST_LOOP_INIT;
      break;

    default:
      st_loop = ST_LOOP_INIT;
      break;
  }

#ifdef ST_DEBUG
  Log.msg(F("ST_LOOP= %d"), st_loop);
#endif
}

