#include "pico/stdlib.h"

#define BUZZER1 10
#define BUTTON_CANCELAR 6


void init_buzzer();
void buzzer_on(uint buzzer_pin, float frequency, uint duration_ms);
void buzzer_alarme();
void buzzer_confirmacao();
void buzzer_erro();
