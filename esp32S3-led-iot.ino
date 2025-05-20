/**
   File:  
          Se plantea la arquitectura para dar soporte a los problemas tipicos 
          en la construccion de un firmware con control y monitorio con el uso de BLYNK(IOT) 

 * - Compiler:           Arduino IDE 2.3.6
 * - Supported devices:  ESP32S3 DEV Module. = ESP32-S3DEVKITC-1_V1.1
 *
 * -author               educacion.ta@gmail.com
 *                       
 *                       
* Date:  28-11-2024
 *
 *      The inside shine.
 *
 */

#include "log.h"
#include "cfg_web.h"
#include "led.h"
#include "precompilation.h"
#include <WiFi.h>
#include <WiFiClient.h>
#include "timer.h"

const char* ssid = "Pablo";
const char* password = "01410398716";


/*
 * Estado del loop principal de ejecucion
 */

#define ST_LOOP_INIT                    0     // Espera el modo de ejecucion.
#define ST_LOOP_IDLE                    1     // Empieza el ensayo.
#define ST_LOOP_LED_ON                  2     // Prende led.
#define ST_LOOP_LED_OFF                 3     // Apaga led.
#define ST_LOOP_END                     4     // Termina el ensayo.
#define ST_MODE_RUN_DEMO1               5     // Demo del uso del estado mode y serialmode.

/*
 * Clases del sistema instanciadas
 */
Clog    Log;
CConfig Config;
CLed     Led;
WebServer server(8080);  // Inicializa el servidor web en el puerto 8080


static void run_demo_serial_plotter( void);            // Demo del uso del serial_plotter

/*
 * Realiza el final de ensayo
 * vuelve el st_test = 0 y
 */
void end_experiment( void ) {
  
  Log.msg( F("Ensayo terminado"));  
  Config.set_st_test( false );
  Config.send_test_finish(); // Informa por el puerto serie que termino el ensayo 
}


/*
 * Simula el muestre de una señal.
 * raw: valor leido.
 * filtered: valor luedo de aplicar a raw un filtro. 
 * danger: punto de comparacion. 
 * state: estado de la comparacion.
 */
 static void run_demo_serial_plotter( void ){
  
    uint16_t raw, filtered,danger_point=2500;
    uint8_t state=0; 
    
    Led.blink(5, 500);
    for (raw=0;raw<5000;raw++){
        filtered = raw+ 500 ;
        if (filtered >danger_point ){ 
          state=1 ;
        }
        Log.ctrl (raw, filtered,state,danger_point);
    }
     Config.set_st_mode( ST_MODE_TEST );
 }

/*
 *  Inicializa los dispositivos del ensayo segun opciones de precompilacion.
 */
void setup()
{  
  
  Serial.begin(LOG_SERIAL_SPEED); 
  delay(100);                          // Espera breve para asegurar que el puerto esté listo
  Config.init();
  Log.init( Config.get_log_level() );  
  
  Serial.println("Init Serial");

  /*
      Para activar la visualisacion  enviar por serie {log_level:'1'}
  */

  Log.msg( F("%s "), FIRMWARE_VERSION );
  Log.msg( F("Noviembre-2024") );

  // Inicialización de la conexión Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Conectando a Wi-Fi...");
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("\nConectado a Wi-Fi");
  Serial.print("Dirección IP: ");
  Serial.println(WiFi.localIP());

  // Inicializa el servidor web
  server.on("/config", HTTP_GET, []() {
    Config.handleHttpRequest(server);
  });  
  
  server.on("/config", HTTP_PUT, []() {
    Config.handleHttpRequest(server);
  });  
  
  server.begin();  // Inicia el servidor web
  Serial.println("Servidor web iniciado");

 
    Led.init();
    Led.set_color(1); // Configura el color a rojo (ID = 1)
    Led.on();        // Enciende el LED
    delay(1000);     // Espera 1 segundo
    Led.off();       // Apaga el LED
 
  Log.msg( F("Sistema inicializado") );
}


void loop()
{
  static uint8_t  st_loop = ST_LOOP_INIT;   
  static CTimer   Timer_led;
  static uint32_t led_blink_qty = 2;
   
  server.handleClient();  // Maneja las solicitudes del cliente
  
  // Actualiza el nivel de log para detener en tiempo real el envio de parametros.
  Log.set_level( Config.get_log_level() );  

  // Actualiza el color de led.
  Led.set_color( Config.get_led_color() );  

  switch ( st_loop ) {

    case ST_LOOP_INIT:

       if (Config.get_st_test() == true ) {                   // Espera que se comienzo al ensayo.        
          st_loop = ST_LOOP_IDLE; 
       }else if (Config.get_st_mode() == ST_MODE_DEMO ) { // Espera el modo runExample1
        st_loop =  ST_MODE_RUN_DEMO1 ;
      }
      
    break;

     // Espera "eventos" de ejecucion
    case ST_LOOP_IDLE:   
          led_blink_qty =  Config.get_led_blink_quantity();     // Carga las veces a blinkear
          Timer_led.start();
          Led.on();
          Log.msg( F("LED ON"));
          st_loop = ST_LOOP_LED_ON;       
      
    break;    
    
    case ST_LOOP_LED_ON:         

      if( Timer_led.expired( Config.get_led_blink_time() ) ) {
                
           Log.msg( F("LED OFF"));      
           Led.off();             
           Timer_led.start();
           st_loop = ST_LOOP_LED_OFF; 
          }

    break;
   
    case ST_LOOP_LED_OFF:
    
      if( Timer_led.expired( Config.get_led_blink_time() ) ) {

          led_blink_qty--; // Decrementa la cantidad de blinks restantes
          if (led_blink_qty > 0) {
            Led.on();
            Log.msg(F("LED ON"));
            Timer_led.start();
            st_loop = ST_LOOP_LED_ON;
          } else {
            st_loop = ST_LOOP_END; // Termina el experimento después de 3 blinks
          }
               
        }

    break;

    case ST_LOOP_END: 

    // Envia comando de final de experimento  
       end_experiment();   
      st_loop = ST_LOOP_INIT;
    break; 

    case ST_MODE_RUN_DEMO1: 

    // Envia comando de final de experimento  
       run_demo_serial_plotter();   
      st_loop = ST_LOOP_INIT;
    break;      
      
      default:
      st_loop = ST_LOOP_INIT;

  }
  
#ifdef ST_DEBUG
  Log.msg( F("ST_LOOP= %d"), st_loop );
#endif //ST_DEBUG

}





