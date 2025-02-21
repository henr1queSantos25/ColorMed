#include "buzzer.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"
#include "pico/stdlib.h"


void init_buzzer()
{
    gpio_init(BUZZER1);
    gpio_set_dir(BUZZER1, GPIO_OUT);
}

// Função para gerar som no buzzer por um tempo específico e com a frequência desejada
void buzzer_on(uint buzzer_pin, float frequency, uint duration_ms)
{
    float period = 1.0f / frequency;   // Calcula o período da onda
    float half_period = period / 2;    // Meio período em segundos
    uint cycles = (duration_ms * frequency) / 1000; // Número de ciclos necessários

    for (uint i = 0; i < cycles; i++)
    {
        gpio_put(buzzer_pin, 1);            // Liga o buzzer (HIGH)
        sleep_us((int)(half_period * 1e6)); // Aguarda meio período
        gpio_put(buzzer_pin, 0);            // Desliga o buzzer (LOW)
        sleep_us((int)(half_period * 1e6)); // Aguarda meio período
    }
}


// Função para gerar um som de alarme com variação de frequência
void buzzer_alarme() {
    float freq_baixa = 500.0f;  // Frequência mais grave
    float freq_alta = 1000.0f;  // Frequência mais aguda
    uint duracao_total = 60000; // Duração total do alarme (60 segundos)

    uint inicio = to_ms_since_boot(get_absolute_time()); // Marca o tempo inicial

    while (to_ms_since_boot(get_absolute_time()) - inicio < duracao_total) {
        if(gpio_get(BUTTON_CANCELAR) == 0) {
            break;
        }
        // Tom grave
        buzzer_on(BUZZER1, freq_baixa, 500);
        // Tom agudo
        buzzer_on(BUZZER1, freq_alta, 500);
    }
}

// Função para gerar um som curto de confirmação
void buzzer_confirmacao() {
    float freq_inicial = 800.0f;  // Frequência inicial
    float freq_final = 1200.0f;   // Frequência final
    uint duracao_bip = 80;        // Duração de cada bip em ms
    uint intervalo = 50;          // Pequena pausa entre bipes

    // Três bipes curtos e crescentes
    for (int i = 0; i < 3; i++) {
        buzzer_on(BUZZER1, freq_inicial + (i * 100), duracao_bip);
        sleep_ms(intervalo);
    }
}

// Função para gerar um som curto de erro
void buzzer_erro() {
    float freq_alta = 1000.0f;  // Frequência inicial (aguda)
    float freq_baixa = 600.0f;  // Frequência final (grave)
    uint duracao_bip = 100;     // Duração de cada bip em ms
    uint intervalo = 80;        // Pequena pausa entre os bipes

    // Dois bipes decrescentes
    buzzer_on(BUZZER1, freq_alta, duracao_bip);
    sleep_ms(intervalo);
    buzzer_on(BUZZER1, freq_baixa, duracao_bip);
}