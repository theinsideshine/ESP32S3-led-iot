#include "Led.h"

// Define el mapa de colores
const uint8_t CLed::COLORS[11][3] = {
    {0, 0, 0},        // 0: Apagado
    {255, 0, 0},      // 1: Rojo
    {255, 255, 0},    // 2: Amarillo
    {0, 255, 0},      // 3: Verde
    {255, 255, 255},  // 4: Blanco
    {0, 0, 255},      // 5: Azul
    {0, 255, 255},    // 6: Cian
    {255, 0, 255},    // 7: Magenta
    {255, 165, 0},    // 8: Naranja
    {238, 130, 238},  // 9: Violeta
    {169, 169, 169}   // 10: Gris
};

// Constructor
CLed::CLed(uint8_t pin, uint8_t num_leds)
    : leds(num_leds, pin, NEO_GRB + NEO_KHZ800) {}

// Inicializa el LED
void CLed::init() {
    leds.begin();
    leds.show(); // Asegúrate de apagar el LED al inicio
    off();       // Apaga el LED al inicio
}

// Enciende el LED con el color actual
void CLed::on() {
    for (int i = 0; i < leds.numPixels(); i++) {
        leds.setPixelColor(i, leds.Color(currentColor[0], currentColor[1], currentColor[2]));
    }
    leds.show();
}

// Apaga el LED
void CLed::off() {
    for (int i = 0; i < leds.numPixels(); i++) {
        leds.setPixelColor(i, 0); // Apaga todos los LEDs
    }
    leds.show();
}

// Configura el color usando un ID
void CLed::set_color(uint8_t color_id) {
    if (color_id < 11) { // Verifica que el ID sea válido
        currentColor[0] = COLORS[color_id][0];
        currentColor[1] = COLORS[color_id][1];
        currentColor[2] = COLORS[color_id][2];
    }
}

// Hace parpadear el LED
void CLed::blink(uint8_t n_blink, unsigned long t_blink) { // Cambiado a unsigned long
    for (int i = 0; i < n_blink; i++) {
        on();
        delay(t_blink);
        off();
        delay(t_blink);
    }
}


