#ifndef CFGIOT_H
#define CFGIOT_H

#include "Arduino.h"
#include <ArduinoJson.h>

#define MAGIC_NUMBER                    100
#define LED_BLINK_TIME_DEFAULT          1010
#define LED_BLINK_QUANTITY_DEFAULT      3
#define LED_COLOR_DEFAULT               1
#define ST_TEST_DEFAULT                 0
#define ST_MODE_DEFAULT                 ST_MODE_TEST

#define EEPROM_ADDRESS_MAGIC_NUMBER       0
#define EEPROM_ADDRESS_LED_BLINK_TIME     (EEPROM_ADDRESS_MAGIC_NUMBER + sizeof(uint32_t))
#define EEPROM_ADDRESS_LED_BLINK_QUANTITY (EEPROM_ADDRESS_LED_BLINK_TIME + sizeof(uint32_t))
#define EEPROM_ADDRESS_LED_COLOR          (EEPROM_ADDRESS_LED_BLINK_QUANTITY + sizeof(uint32_t))
#define EEPROM_ADDRESS_LOG_LEVEL          (EEPROM_ADDRESS_LED_COLOR + sizeof(uint32_t))
#define EEPROM_ADDRESS_ST_TEST            (EEPROM_ADDRESS_LOG_LEVEL + sizeof(uint32_t))
#define EEPROM_ADDRESS_ST_MODE            (EEPROM_ADDRESS_ST_TEST + sizeof(uint32_t))

#define ST_MODE_TEST 0
#define ST_MODE_DEMO 200

class CIotConfig {
  public:
    CIotConfig();
    void init();

    uint32_t get_led_blink_time();
    void set_led_blink_time(uint32_t);

    uint32_t get_led_blink_quantity();
    void set_led_blink_quantity(uint32_t);

    uint32_t get_led_color();
    void set_led_color(uint32_t);

    uint32_t get_log_level();
    void set_log_level(uint32_t);

    uint32_t get_st_test();
    void set_st_test(uint32_t);
    void send_st_test_to_blynk(uint32_t);

    uint32_t get_st_mode();
    void set_st_mode(uint32_t);

    void send_test_finish();
    void send_demo_finish();

  private:
    uint32_t log_level;
    uint32_t led_blink_time;
    uint32_t led_blink_quantity;
    uint32_t led_color;
    uint32_t st_test;
    uint32_t st_mode;
};

extern CIotConfig Config;

#endif
