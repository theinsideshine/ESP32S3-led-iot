#include "cfg_iot.h"
#include <EEPROM.h>

CIotConfig::CIotConfig() {}

void CIotConfig::init() {
    Serial.begin(115200);
    delay(100); // Espera breve para asegurar que el puerto esté listo

    uint32_t magic_number;

    EEPROM.begin(64);
    EEPROM.get(EEPROM_ADDRESS_MAGIC_NUMBER, magic_number);

    Serial.print("Valor de magic_number leído: ");
    Serial.println(magic_number);

    if (magic_number != MAGIC_NUMBER) {
        Serial.println("Magic number no coincide. Configurando valores por defecto...");
        magic_number = MAGIC_NUMBER;
        EEPROM.put(EEPROM_ADDRESS_MAGIC_NUMBER, magic_number);

        set_led_blink_time(LED_BLINK_TIME_DEFAULT);
        set_led_blink_quantity(LED_BLINK_QUANTITY_DEFAULT);
        set_led_color(LED_COLOR_DEFAULT);
        set_log_level(0);
        set_st_test(ST_TEST_DEFAULT);
        set_st_mode(ST_MODE_DEFAULT);

        EEPROM.commit();
        Serial.println("Valores de configuración por defecto establecidos y guardados en EEPROM.");
    } else {
        Serial.println("Magic number coincide. Cargando valores de EEPROM...");
        EEPROM.get(EEPROM_ADDRESS_LED_BLINK_TIME, led_blink_time);
        EEPROM.get(EEPROM_ADDRESS_LED_BLINK_QUANTITY, led_blink_quantity);
        EEPROM.get(EEPROM_ADDRESS_LED_COLOR, led_color);
        EEPROM.get(EEPROM_ADDRESS_LOG_LEVEL, log_level);
        EEPROM.get(EEPROM_ADDRESS_ST_TEST, st_test);
        EEPROM.get(EEPROM_ADDRESS_ST_MODE, st_mode);

        Serial.print("Led blink time: ");
        Serial.println(led_blink_time);
        Serial.print("Led blink quantity: ");
        Serial.println(led_blink_quantity);
        Serial.print("Led color: ");
        Serial.println(led_color);
        Serial.print("Log level: ");
        Serial.println(log_level);
    }
}

uint32_t CIotConfig::get_led_blink_time() { return led_blink_time; }
void CIotConfig::set_led_blink_time(uint32_t val) {
    led_blink_time = val;
    EEPROM.put(EEPROM_ADDRESS_LED_BLINK_TIME, val);
    EEPROM.commit();
}

uint32_t CIotConfig::get_led_blink_quantity() { return led_blink_quantity; }
void CIotConfig::set_led_blink_quantity(uint32_t val) {
    led_blink_quantity = val;
    EEPROM.put(EEPROM_ADDRESS_LED_BLINK_QUANTITY, val);
    EEPROM.commit();
}

uint32_t CIotConfig::get_led_color() { return led_color; }
void CIotConfig::set_led_color(uint32_t val) {
    led_color = val;
    EEPROM.put(EEPROM_ADDRESS_LED_COLOR, val);
    EEPROM.commit();
}

uint32_t CIotConfig::get_log_level() { return log_level; }
void CIotConfig::set_log_level(uint32_t val) {
    log_level = val;
    EEPROM.put(EEPROM_ADDRESS_LOG_LEVEL, val);
    EEPROM.commit();
}

/*
 * Estos deberían ser booleano !!!!! 
 */
uint32_t CIotConfig::get_st_test() { return st_test; }

void CIotConfig::set_st_test(uint32_t val) {
    st_test = val;
    EEPROM.put(EEPROM_ADDRESS_ST_TEST, val);
    EEPROM.commit();
}

uint32_t CIotConfig::get_st_mode() { return st_mode; }

void CIotConfig::set_st_mode(uint32_t val) {
    st_mode = val;
    EEPROM.put(EEPROM_ADDRESS_ST_MODE, val);
    EEPROM.commit();
}

void CIotConfig::send_st_test_to_blynk(uint32_t value) {
    // vacía por ahora — futura integración si se desea
}

void CIotConfig::send_test_finish() {
    StaticJsonDocument<256> doc;
    doc["st_test"] = get_st_test();
    serializeJsonPretty(doc, Serial);
}


void CIotConfig::send_demo_finish() {
    StaticJsonDocument<256> doc;
    doc["st_demo"] = get_st_mode();
    serializeJsonPretty(doc, Serial);
}
