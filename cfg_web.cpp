#include "cfg_web.h"
#include "log.h"
#include <ArduinoJson.h>
#include <EEPROM.h>

CConfig::CConfig()
{

}

void CConfig::init() {
    // Iniciar el puerto serial para depuración
    Serial.begin(115200);
    delay(100); // Espera breve para asegurar que el puerto esté listo

    uint32_t magic_number;

    // Iniciar la EEPROM con un tamaño adecuado
    EEPROM.begin(64);  // Cambia 64 por el tamaño que necesites
    EEPROM.get(EEPROM_ADDRESS_MAGIC_NUMBER, magic_number);

    // Imprimir el valor inicial del magic_number leído desde EEPROM
    Serial.print("Valor de magic_number leído: ");
    Serial.println(magic_number);

    if (magic_number != MAGIC_NUMBER) {
        // No coincide, inicializamos valores por defecto
        Serial.println("Magic number no coincide. Configurando valores por defecto...");
        magic_number = MAGIC_NUMBER;
        EEPROM.put(EEPROM_ADDRESS_MAGIC_NUMBER, magic_number);        
        
        set_led_blink_time(LED_BLINK_TIME_DEFAULT);
        set_led_blink_quantity(LED_BLINK_QUANTITY_DEFAULT);
        set_led_color(LED_COLOR_DEFAULT);
        
        set_log_level(LOG_MSG); // Setea log por defecto
        set_st_test(ST_TEST_DEFAULT);
        set_st_mode(ST_MODE_DEFAULT);

        EEPROM.commit();  // Asegura que los valores se escriben en la EEPROM

        Serial.println("Valores de configuración por defecto establecidos y guardados en EEPROM.");
    } else {
        // Cargar los valores desde la EEPROM
        Serial.println("Magic number coincide. Cargando valores de EEPROM...");      
        EEPROM.get(EEPROM_ADDRESS_LED_BLINK_TIME, led_blink_time);
        EEPROM.get(EEPROM_ADDRESS_LED_BLINK_QUANTITY, led_blink_quantity);
        EEPROM.get(EEPROM_ADDRESS_LED_COLOR, led_color);

        EEPROM.get(EEPROM_ADDRESS_LOG_LEVEL, log_level);

        // Imprimir valores leídos
        
        Serial.print("Led blink time: ");
        Serial.println(led_blink_time);
        Serial.print("Led blink quantity: ");
        Serial.println(led_blink_quantity);
        Serial.print("Log level: ");
        Serial.println(log_level);
        Serial.print("Led color: ");
        Serial.println(led_color);

        set_st_test(ST_TEST_DEFAULT);
        set_st_mode(ST_MODE_DEFAULT);
    }
}

void CConfig::handleHttpRequest(WebServer &server) {
    bool known_key = false;

    if (server.method() == HTTP_GET) {
        StaticJsonDocument<512> doc;  // Documento JSON para llenar la respuesta
    
        // Llenar el doc con todos los parámetros
        send_all_params(doc);

        // Serializar el documento JSON en una cadena
        String response;
        serializeJson(doc, response);
    
        // Enviar la respuesta con código 200 (OK) y el contenido en JSON
        server.send(200, "application/json", response);

    } else if (server.method() == HTTP_PUT) {
        if (server.hasArg("plain")) {
            StaticJsonDocument<512> doc;
            auto error = deserializeJson(doc, server.arg("plain"));
            
            if (!error) {
                // Procesar los parámetros recibidos
                
                if ( doc.containsKey("led_blink_time") ) {
                    set_led_blink_time( doc["led_blink_time"] );
                    known_key = true;
                }

                if ( doc.containsKey("led_blink_quantity") ) {
                    set_led_blink_quantity( doc["led_blink_quantity"] );
                    known_key = true;
                }

                if ( doc.containsKey("led_color") ) {
                    set_led_color( doc["led_color"] );
                    known_key = true;
                }
                       
                if ( doc.containsKey("log_level") ) {
                    set_log_level( doc["log_level"] );
                    known_key = true;
                }  

                if ( doc.containsKey("st_mode") ) {
                    set_st_mode( doc["st_mode"] );
                    known_key = true;
                } 

                if ( doc.containsKey("info") ) {
                String key = doc["info"];

                    if( key == "all-params" ) {
                        send_all_params( doc );
                    }            
                }

                if (doc.containsKey("cmd")) {
                    String key = doc["cmd"];                
                              
                    if (key == "start") {
                        set_st_test(1);  // Comienza el ensayo.
                        send_ack(doc);
                    }
                } else if (known_key == true) {
                    send_ok(doc);
                }
                // Serializar el documento JSON en una cadena
                String response;
                serializeJson(doc, response);               
                // Enviar la respuesta con código 200 (OK) y el contenido en JSON
                server.send(200, "application/json", response);

            } else {
                // Enviar una respuesta de error si el JSON no es válido
                server.send(400, "application/json", "{\"result\":\"error\", \"message\":\"Invalid JSON\"}");
            }

        } else {
            // Enviar una respuesta de error si no hay cuerpo en la solicitud
            server.send(400, "application/json", "{\"result\":\"error\", \"message\":\"No body\"}");
        }

    } else {
        // Enviar una respuesta de error si el método no es permitido
        server.send(405, "application/json", "{\"result\":\"error\", \"message\":\"Method not allowed\"}");
    }
}

uint32_t CConfig::get_led_blink_time( void )
{
    return led_blink_time;
}

void CConfig::set_led_blink_time( uint32_t val )
{
    led_blink_time = val;
    EEPROM.put( EEPROM_ADDRESS_LED_BLINK_TIME, val );
    EEPROM.commit();  // Asegura que el valor se escriba
}

uint32_t CConfig::get_led_blink_quantity( void )
{
    return led_blink_quantity;
}

void CConfig::set_led_blink_quantity( uint32_t val )
{
    led_blink_quantity = val;
    EEPROM.put( EEPROM_ADDRESS_LED_BLINK_QUANTITY, val );
    EEPROM.commit();  // Asegura que el valor se escriba
}

uint32_t CConfig::get_led_color( void )
{
    return led_color;
}

void CConfig::set_led_color( uint32_t val )
{
    led_color = val;
    EEPROM.put( EEPROM_ADDRESS_LED_COLOR, val );
    EEPROM.commit();  // Asegura que el valor se escriba
}

uint32_t CConfig::get_log_level( void )
{
    return log_level;
}

void CConfig::set_log_level( uint32_t enable )
{
    log_level = enable;
    EEPROM.put( EEPROM_ADDRESS_LOG_LEVEL, log_level );
    EEPROM.commit();  // Asegura que el valor se escriba
}
/*
 * Estos deberia ser booleano !!!!!
 * 
 */
 
uint32_t CConfig::get_st_test( void )
{
    return st_test;
}

void CConfig::set_st_test( uint32_t enable )
{
    st_test = enable;
    EEPROM.put( EEPROM_ADDRESS_ST_TEST, st_test );
    EEPROM.commit();  // Asegura que el valor se escriba
}

uint32_t CConfig::get_st_mode( void )
{
    return st_mode;
}

void CConfig::set_st_mode( uint32_t mode )
{
    st_mode = mode;
    EEPROM.put( EEPROM_ADDRESS_ST_MODE, st_mode );
    EEPROM.commit();  // Asegura que el valor se escriba
}


void CConfig::send_all_params( JsonDocument& doc )
{     
    doc["led_blink_time"]     = get_led_blink_time();  
    doc["led_blink_quantity"] = get_led_blink_quantity();  
    doc["led_color"]          = get_led_color();       
    doc["st_test"]            = get_st_test();  
    doc["st_mode"]            = get_st_mode();  
    doc["log_level"]          = get_log_level();
   
    serializeJsonPretty( doc, Serial );
}

//Envia el status del test
void CConfig::send_test_finish( void )
{
    StaticJsonDocument<512> doc;   
    doc["st_test"] = get_st_test();  
    serializeJsonPretty( doc, Serial );
}

// Envia el resultado en formato json
void CConfig::send_ok( JsonDocument& doc )
{
    doc[ "result" ] = "ok";

    serializeJsonPretty( doc, Serial );
}

// Envia el resultado en formato json
void CConfig::send_ack( JsonDocument& doc )
{
    doc[ "result" ] = "ack";

    serializeJsonPretty( doc, Serial );
}