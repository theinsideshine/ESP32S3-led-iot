#ifndef CONFIGWEB_H
#define CONFIGWEB_H

#include "Arduino.h"
#include <WebServer.h>
#include "precompilation.h"
#include <ArduinoJson.h>

                                                  

#define MAGIC_NUMBER                    100    // Numero magico para detectar memoria sin inicializar.


#define LED_BLINK_TIME_DEFAULT              1010          // Tiempo en ms del blinkeo del led.
#define LED_BLINK_QUANTITY_DEFAULT          3             // Numero de blinkeo del led.
#define LED_COLOR_DEFAULT                   1             // Color del led ROJO
#define ST_TEST_DEFAULT                     0             //  Estado del test pòr defecto.
#define ST_MODE_DEFAULT                     ST_MODE_TEST  //  Modo de operacion del sistema. 


// Mapa de direcciones de los campos de configuracion en la EEPROM. en esp el word es 4 bytes

#define EEPROM_ADDRESS_MAGIC_NUMBER     0
#define EEPROM_ADDRESS_LED_BLINK_TIME     (EEPROM_ADDRESS_MAGIC_NUMBER + sizeof(uint32_t))        // Cambiado a uint32_t
#define EEPROM_ADDRESS_LED_BLINK_QUANTITY (EEPROM_ADDRESS_LED_BLINK_TIME + sizeof(uint32_t))      // Cambiado a uint32_t
#define EEPROM_ADDRESS_LED_COLOR          (EEPROM_ADDRESS_LED_BLINK_QUANTITY + sizeof(uint32_t))  // Cambiado a uint32_t
#define EEPROM_ADDRESS_LOG_LEVEL          (EEPROM_ADDRESS_LED_COLOR + sizeof(uint32_t))           // Cambiado a uint32_t
#define EEPROM_ADDRESS_ST_TEST            (EEPROM_ADDRESS_LOG_LEVEL + sizeof(uint32_t))           // Cambiado a uint32_t
#define EEPROM_ADDRESS_ST_MODE            (EEPROM_ADDRESS_ST_TEST + sizeof(uint32_t))             // Cambiado a uint32_t

/*
 *  Para poder leer los dispositivo y ejecutar accciones se pone un modo de operacion .
 *  
 */

#define ST_MODE_TEST                    0               // Modo de operacion normal, ensayo activado.
#define ST_MODE_DEMO                    200             //  Demo: Uso del estado modo.


class CConfig
{
  public:
    CConfig();
    void handleHttpRequest(WebServer &server);
    void init();
    uint32_t get_led_blink_time( void );
    void set_led_blink_time( uint32_t ); 

    uint32_t get_led_blink_quantity( void );
    void set_led_blink_quantity( uint32_t ); 

    uint32_t get_led_color( void );
    void set_led_color( uint32_t );

    uint32_t get_log_level( void );
    void set_log_level( uint32_t enable );

    uint32_t get_st_test( void );
    void set_st_test( uint32_t enable );

    uint32_t get_st_mode( void );
    void set_st_mode( uint32_t mode ); 

    void host_cmd( void );
    void send_test_finish(void);

  private:
    uint32_t log_level;          // 0 = log de informacion de control desactivada.
    uint32_t st_test;            // Estado del ensayo 
    uint32_t st_mode;            // Modo del ensayo      
    uint32_t led_blink_time;     // Tiempo en ms de blinkeo del led. 
    uint32_t led_blink_quantity; // Veces de blinkeo del led. 
    uint32_t led_color;          // Color del led.
      
    void send_all_params( JsonDocument& );      
    void send_ok( JsonDocument& );
    void send_ack( JsonDocument& );
     
    
};

#endif // CONFIGWEB_H