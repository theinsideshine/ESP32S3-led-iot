#ifndef LOG_H
#define LOG_H

#include "Arduino.h"

#define LOG_SERIAL_SPEED                115200

#define LOG_DISABLED                    0           // Log desactivado.
#define LOG_MSG                         1           // Logea mensajes
#define LOG_CTRL_JSON                   2           // Log habilitado en formato json.
#define LOG_CTRL_ARDUINO_PLOTTER        3           // Log habilitado en formato arduino serial plotter.

class Clog
{
  public:
    Clog();
    void init( uint8_t );
    void msg( const __FlashStringHelper *fmt, ... );
    void ctrl( uint16_t raw, uint16_t filtered, uint8_t state, uint16_t danger_point );
    void set_level(uint8_t);

  private:
    void msg_ctrl( const __FlashStringHelper *fmt, ... );

    uint8_t level;            // Nivel de log (0 = desactivado).
};

#endif // LOG_H
