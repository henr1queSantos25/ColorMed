#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/rtc.h"
#include "pico/util/datetime.h"
#include "hardware/timer.h"

// Bibliotecas externas
#include "libs/font.h"
#include "libs/ssd1306.h"
#include "libs/buzzer.h"
#include "libs/led_5x5.h"
#include "libs/buttons.h"

// Configuração do Display OLED
#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define endereco 0x3C
ssd1306_t ssd;

// Configuração dos Botões
#define BUTTON_Configuracao 22
#define BUTTON_Editar 5
#define BUTTON_Confirmar 6

// Estrutura de Alarme
typedef struct {
    int horas;
    int minutos;
    int cor;
    int segundos;
} Alarme;

// Definições Globais
#define MAX_ALARMES 10
Alarme alarmes[MAX_ALARMES];
int total_alarmes = 0;

// Variáveis Globais
datetime_t t;                                    
uint32_t volatile last_time = 0;                 
uint quant_horas = 0; // armazena a hora de configuração     
uint quant_minutos = 0; // armazena o minuto de configuração      
bool volatile configurando = false;  // indica se está configurando o alarme            
volatile bool em_configuracao = false; // flag para indicar se está em configuração
volatile bool alarme_ativo = false; // indica se o alarme está ativo       

// Lista de cores disponíveis
int opcao_selecionada = 0; 
const char *opcoes[] = {"1. Verde", "2. Vermelho", "3. Azul", "4. Amarelo", "5. Roxo"};

// Prototipação
void setup();
void configurar_alarme();
void definir_hora_atual();
void initRTC();
void display_time();
void adicionar_alarme(int horas, int minutos, int cor);
void verificar_alarmes();
void exibir_lista_cores();
void gpio_irq_handler(uint gpio, uint32_t events);
void configurar_tempo(const char *label, uint *quantidade, uint max_valor);

/* ==========================================================
   LOOP PRINCIPAL
   ========================================================== */
int main() {
    setup(); // Inicializa os componentes
    initRTC(); // Inicializa o RTC
    sleep_ms(500);

    // Interrupção do botão Jytk
    gpio_set_irq_enabled_with_callback(BUTTON_Configuracao, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);

    while (true) {
        display_time(); // exibe a hora atual
        verificar_alarmes(); // 

        if (configurando) { // Interrupção do botão Jytk
            configurar_alarme();
        }

        sleep_ms(10);
    }
}

/* ==========================================================
   INTERRUPÇÃO DOS BOTÕES
   ========================================================== */
void gpio_irq_handler(uint gpio, uint32_t events) {
    uint32_t time = to_us_since_boot(get_absolute_time());

    if (time - last_time > 4000) { 
        last_time = time; // atualiza o tempo

        if (gpio == BUTTON_Configuracao && !em_configuracao) { // se o botão Jytk for pressionado
            configurando = true;
        }
    }
}

/* ==========================================================
   FUNÇÃO PARA CONFIGURAÇÃO DO ALARME
   ========================================================== */
void configurar_tempo(const char *label, uint *quantidade, uint max_valor) {
    char time_config[6]; 
    int setinha_pos = (strcmp(label, "Minutos:") == 0) ? 68 : 44; // posição da setinha

    while (gpio_get(BUTTON_Confirmar)) { // enquanto o botão não for pressionado
        snprintf(time_config, sizeof(time_config), "%02d:%02d", quant_horas, quant_minutos); 

        ssd1306_fill(&ssd, false);
        ssd1306_rect(&ssd, 1, 1, 126, 62, 1, 0);

        ssd1306_draw_string(&ssd, label, 10, 5);
        ssd1306_draw_string(&ssd, time_config, 44, 28);
        ssd1306_draw_string(&ssd, "^^", setinha_pos, 40); 

        ssd1306_send_data(&ssd);

        if (debounce_button(BUTTON_Editar)) {
            *quantidade = (*quantidade + 1) % max_valor;
            sleep_ms(20);
        }
    }
    buzzer_confirmacao();
    sleep_ms(150);
}

void configurar_alarme() {
    em_configuracao = true; 

    configurar_tempo("Horas:", &quant_horas, 24); // horas

    configurar_tempo("Minutos:", &quant_minutos, 60); // minutos

    while (gpio_get(BUTTON_Confirmar)) { 
        exibir_lista_cores(); 

        if (debounce_button(BUTTON_Editar)) {
            opcao_selecionada = (opcao_selecionada + 1) % 5; 
        }

        if (debounce_button(BUTTON_Confirmar)) {
            printf("Cor selecionada: %s\n", opcoes[opcao_selecionada]);
            break;
        }
        sleep_ms(30);
    }

    adicionar_alarme(quant_horas, quant_minutos, opcao_selecionada);  // salva o alarme

    quant_horas = quant_minutos = opcao_selecionada = 0; 
    em_configuracao = false;                             
    configurando = false;
}

/* ==========================================================
   RTC: INICIALIZAÇÃO E GERENCIAMENTO DE HORÁRIO
   ========================================================== */

void definir_hora_atual() {
    t.year = 2025;
    t.month = 2;
    t.day = 26;
    t.hour = 12;
    t.min = 13;
    t.sec = 0;
    rtc_set_datetime(&t);
}
void initRTC() {
    rtc_init();
    rtc_get_datetime(&t); 

    if (t.year < 2024) {   // RTC zerado
        printf("RTC zerado, definindo hora...\n");
        definir_hora_atual();
    }
}

/* ==========================================================
   FUNÇÕES DO DISPLAY
   ========================================================== */
void display_time() {
    rtc_get_datetime(&t); // hora atual
    char time_str[10];                                                
    snprintf(time_str, sizeof(time_str), "%02d:%02d", t.hour, t.min); // formata a string

    ssd1306_fill(&ssd, false);
    ssd1306_rect(&ssd, 1, 1, 126, 62, 1, 0);
    ssd1306_draw_string(&ssd, time_str, 44, 28); 

    if (alarme_ativo) {
        ssd1306_draw_string(&ssd, "ALARME ATIVO!", 10, 45);
    }

    ssd1306_send_data(&ssd);
}

void exibir_lista_cores() {
    ssd1306_fill(&ssd, false);
    ssd1306_draw_string(&ssd, "Escolha uma cor", 0, 0);

    for (int i = 0; i < 5; i++) {
        ssd1306_draw_string(&ssd, opcoes[i], 0, 10 + (i * 10));  // exibe as opções

        if (i == opcao_selecionada) {
            ssd1306_draw_char(&ssd, '<', 100, 10 + (i * 10));  // exibe a setinha
        }
    }

    ssd1306_send_data(&ssd);
}

/* ==========================================================
   GERENCIAMENTO DE ALARMES
   ========================================================== */
void adicionar_alarme(int horas, int minutos, int cor) {
    for (int i = 0; i < total_alarmes; i++) {
        if (alarmes[i].horas == horas && alarmes[i].minutos == minutos) { // já existe
            ssd1306_fill(&ssd, false);
            ssd1306_rect(&ssd, 1, 1, 126, 62, 1, 0);
            ssd1306_draw_string(&ssd, "ALARME JA", 30, 20);
            ssd1306_draw_string(&ssd, " EXISTE!", 30, 32);
            ssd1306_send_data(&ssd);
            buzzer_erro();
            sleep_ms(2000);
            return;
        }
    }
    if (total_alarmes < MAX_ALARMES) { // ainda há espaço
        buzzer_confirmacao();
        alarmes[total_alarmes] = (Alarme){horas, minutos, cor, 0};
        total_alarmes++;
        printf("Alarme salvo: %02d:%02d | Cor: %d\n", horas, minutos, cor);
    } else { // atingiu o limite
        ssd1306_fill(&ssd, false);
        ssd1306_rect(&ssd, 1, 1, 126, 62, 1, 0);
        ssd1306_draw_string(&ssd, "LIMITE ATINGIDO", 5, 25);
        ssd1306_send_data(&ssd);
        buzzer_erro();
        sleep_ms(2000);
    }
}

void verificar_alarmes() {
    rtc_get_datetime(&t);

    for (int i = 0; i < total_alarmes; i++) {
        if (t.hour == alarmes[i].horas && t.min == alarmes[i].minutos && t.sec == alarmes[i].segundos) {
            alarme_ativo = true;
            em_configuracao = true;

            display_time();
            drawMatrix(alarmes[i].cor); // ativa o LED
            buzzer_alarme(); // emite o som do alarme        

            drawMatrix(5); // desliga o LED
            em_configuracao = false;
            alarme_ativo = false;
        }
    }
}

/* ==========================================================
   CONFIGURAÇÃO INICIAL
   ========================================================== */
void setup() {
    stdio_init_all();
    setup_PIO();
    setup_I2C(I2C_PORT, I2C_SDA, I2C_SCL);
    setup_ssd1306(&ssd, endereco, I2C_PORT);
    setup_buttons(BUTTON_Configuracao);
    setup_buttons(BUTTON_Editar);
    setup_buttons(BUTTON_Confirmar);
    init_buzzer();
}