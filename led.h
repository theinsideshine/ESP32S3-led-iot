#ifndef CLED_H
#define CLED_H

#include <Adafruit_NeoPixel.h>

#define PIN_LED_RGB 48   // Pin conectado al LED RGB
#define NUM_LEDS 1       // Número de LEDs

class CLed {
public:
    CLed(uint8_t pin = PIN_LED_RGB, uint8_t num_leds = NUM_LEDS);
    void init();                        // Inicializa el LED
    void on();                          // Enciende el LED con el color actual
    void off();                         // Apaga el LED
    void set_color(uint8_t color_id);    // Configura el color usando un ID_
    void blink(uint8_t n_blink, unsigned long t_blink);  // Hace parpadear el LED

private:
    Adafruit_NeoPixel leds;
    uint8_t currentColor[3]; // Almacena el color actual en formato RGB

    // Mapa de colores (ID -> RGB)
    static const uint8_t COLORS[11][3];
};

#endif // CLED_H
