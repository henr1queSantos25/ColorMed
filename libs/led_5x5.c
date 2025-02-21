#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/clocks.h"
#include "hardware/pio.h"
#include "led_5x5.h"

// ARQUIVO .pio
#include "build/ColorMed.pio.h"

// NÚMERO DE LEDS
#define NUM_PIXELS 25

// PINO DA MATRIZ DE LED
#define OUT_PIN 7

// INICIALIZAÇÃO E CONFIGURAÇÃO DO PIO
void setup_PIO()
{
    PIO pio = pio0;
    uint offset = pio_add_program(pio, &ColorMed_program);
    uint sm = pio_claim_unused_sm(pio, true);
    ColorMed_program_init(pio, sm, offset, OUT_PIN);
}


// LIGA OS LEDS NECESSÁRIOS
static inline void put_pixel(uint32_t pixel_grb)
{
    pio_sm_put_blocking(pio0, 0, pixel_grb << 8u);
}

uint32_t matrix_rgb(double b, double r, double g)
{
    unsigned char R, G, B;
    R = r * 255;
    G = g * 255;
    B = b * 255;
    return (G << 24) | (R << 16) | (B << 8);
}

// DESENHA O NÚMERO NA MATRIZ 5X5
void drawMatrix(uint cor)
{
    uint32_t valor_led;
    switch (cor)
    {
    case 0:
        for (int16_t i = 0; i < NUM_PIXELS; i++)
        {
            valor_led = matrix_rgb(0.0, 0.0, 0.2);
            pio_sm_put_blocking(pio0, 0, valor_led);
        }
        break;
    case 1:
        for (int16_t i = 0; i < NUM_PIXELS; i++)
        {
            valor_led = matrix_rgb(0.0, 0.2, 0.0);
            pio_sm_put_blocking(pio0, 0, valor_led);
        }
        break;
    case 2:
        for (int16_t i = 0; i < NUM_PIXELS; i++)
        {
            valor_led = matrix_rgb(0.2, 0.0, 0.0);
            pio_sm_put_blocking(pio0, 0, valor_led);
        }
        break;
    case 3:
        for (int16_t i = 0; i < NUM_PIXELS; i++)
        {
            valor_led = matrix_rgb(0.0, 0.6, 0.2);
            pio_sm_put_blocking(pio0, 0, valor_led);
        }
        break;
    case 4:
        for (int16_t i = 0; i < NUM_PIXELS; i++)
        {
            valor_led = matrix_rgb(0.2, 0.2, 0.0);
            pio_sm_put_blocking(pio0, 0, valor_led);
        }
        break;
    case 5:
        for (int16_t i = 0; i < NUM_PIXELS; i++)
        {
            valor_led = matrix_rgb(0.0, 0.0, 0.0);
            pio_sm_put_blocking(pio0, 0, valor_led);
        }
        break;
    }
    
    

}