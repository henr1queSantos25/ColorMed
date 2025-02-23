#include <stdio.h>
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
#define BUTTON_Jytk 22
#define BUTTON_Editar 5
#define BUTTON_Confirmar 6

// Estrutura de Alarme
typedef struct
{
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
datetime_t t; // Estrutura de data e hora
uint32_t volatile last_time = 0; // Variável para debounce
uint volatile quant_horas = 0; // Variável para configuração do alarme
uint volatile quant_minutos = 0; // Variável para configuração do alarme
bool volatile configurando = false; // Indica se o usuário está configurando um alarme
volatile bool em_configuracao = false; // Indica se o usuário está configurando um alarme
volatile bool alarme_ativo = false; // Indica se há um alarme ativo
volatile bool precisa_verificar_alarmes = false; // Flag global para verificar alarmes

// Lista de cores disponíveis
int opcao_selecionada = 0;
const char *opcoes[] = {"1. Verde", "2. Vermelho", "3. Azul", "4. Amarelo", "5. Roxo"};

// Prototipação
void setup();
void configurar_alarme();
void initRTC();
void display_time();
void adicionar_alarme(int horas, int minutos, int cor);
void verificar_alarmes();
void exibir_lista_cores();
void gpio_irq_handler(uint gpio, uint32_t events);

/* ==========================================================
   LOOP PRINCIPAL
   ========================================================== */
int main()
{
    stdio_init_all();
    setup();
    initRTC();
    sleep_ms(500);

    // Interrupção do botão Jytk
    gpio_set_irq_enabled_with_callback(BUTTON_Jytk, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);  

    while (true)
    {
        display_time(); // Exibe o horário atual
        verificar_alarmes(); // Verifica se há alarmes ativos

        if (configurando) // Se o botão Jytk foi pressionado, inicia a configuração do alarme
        {
            configurar_alarme();
        }

        sleep_ms(10); 
    }
}

/* ==========================================================
   INTERRUPÇÃO DOS BOTÕES
   ========================================================== */
void gpio_irq_handler(uint gpio, uint32_t events)
{
    uint32_t time = to_us_since_boot(get_absolute_time());

    if (time - last_time > 4000)
    { // Debounce de 400ms
        last_time = time;

        if (gpio == BUTTON_Jytk && !em_configuracao)
        {
            configurando = true;
        }
    }
}

/* ==========================================================
   FUNÇÃO PARA CONFIGURAÇÃO DO ALARME
   ========================================================== */
void configurar_alarme()
{
    em_configuracao = true; // Indica que o usuário está configurando um alarme
    char time_config[6];   // Armazena o horário configurado

    // ETAPA 1: Configuração de HORAS
    while (gpio_get(BUTTON_Confirmar)) // Enquanto o botão de confirmação não for pressionado
    {
        snprintf(time_config, sizeof(time_config), "%02d:%02d", quant_horas, quant_minutos); // Formata o horário

        ssd1306_fill(&ssd, false);
        ssd1306_rect(&ssd, 1, 1, 126, 62, 1, 0);

        ssd1306_draw_string(&ssd, "Horas:", 10, 5);
        ssd1306_draw_string(&ssd, time_config, 44, 28);
        ssd1306_draw_string(&ssd, "^^", 44, 40); // Setinhas abaixo das horas

        ssd1306_send_data(&ssd);

        if (debounce_button(BUTTON_Editar)) // Se o botão de edição for pressionado, incrementa as horas
        {
            quant_horas = (quant_horas + 1) % 24;
            sleep_ms(20);
        }
    }
    buzzer_confirmacao(); 

    sleep_ms(150);

    // ETAPA 2: Configuração de MINUTOS
    while (gpio_get(BUTTON_Confirmar)) // Enquanto o botão de confirmação não for pressionado
    {
        snprintf(time_config, sizeof(time_config), "%02d:%02d", quant_horas, quant_minutos); // Formata o horário

        ssd1306_fill(&ssd, false);
        ssd1306_rect(&ssd, 1, 1, 126, 62, 1, 0);

        ssd1306_draw_string(&ssd, "Minutos: ", 10, 5);
        ssd1306_draw_string(&ssd, time_config, 44, 28);
        ssd1306_draw_string(&ssd, "   ^^", 44, 40); // Setinhas abaixo dos minutos

        ssd1306_send_data(&ssd);

        if (debounce_button(BUTTON_Editar)) // Se o botão de edição for pressionado, incrementa os minutos
        {
            quant_minutos = (quant_minutos + 1) % 60;
            sleep_ms(20);
        }
    }
    buzzer_confirmacao();
    sleep_ms(150);

    // Escolha de Cores
    while (gpio_get(BUTTON_Confirmar)) // Enquanto o botão de confirmação não for pressionado
    {
        exibir_lista_cores(); // Exibe a lista de cores disponíveis

        if (debounce_button(BUTTON_Editar)) // Se o botão de edição for pressionado, altera a cor selecionada
        {
            opcao_selecionada = (opcao_selecionada + 1) % 5; // Alterna entre as opções
        }

        if (debounce_button(BUTTON_Confirmar)) // Se o botão de confirmação for pressionado, finaliza a configuração
        {
            printf("Cor selecionada: %s\n", opcoes[opcao_selecionada]);
            break;
        }
        sleep_ms(30);
    }

    adicionar_alarme(quant_horas, quant_minutos, opcao_selecionada); // Adiciona o alarme configurado

    quant_horas = quant_minutos = opcao_selecionada = 0; // Reseta as variáveis
    em_configuracao = false; // Indica que o usuário terminou de configurar o alarme
    configurando = false; 
}

/* ==========================================================
   RTC: INICIALIZAÇÃO E GERENCIAMENTO DE HORÁRIO
   ========================================================== */
void initRTC()
{
    rtc_init(); 
    rtc_get_datetime(&t); // Obtém a data e hora atual

    if (t.year < 2024) // Se o RTC estiver zerado, define a data e hora atual
    { 
        printf("RTC zerado, definindo hora...\n");
        t.year = 2025;
        t.month = 2;
        t.day = 21;
        t.hour = 00;
        t.min = 4;
        t.sec = 0;
        rtc_set_datetime(&t);
    }
}

/* ==========================================================
   FUNÇÕES DO DISPLAY
   ========================================================== */
void display_time()
{
    rtc_get_datetime(&t);
    char time_str[10]; // Armazena o horário formatado
    snprintf(time_str, sizeof(time_str), "%02d:%02d", t.hour, t.min); // Formata o horário

    ssd1306_fill(&ssd, false);
    ssd1306_rect(&ssd, 1, 1, 126, 62, 1, 0);
    ssd1306_draw_string(&ssd, time_str, 44, 28); // Exibe o horário no display

    if (alarme_ativo) // Se houver um alarme ativo, exibe a mensagem "ALARME ATIVO!"
    {
        ssd1306_draw_string(&ssd, "ALARME ATIVO!", 10, 45);
    }

    ssd1306_send_data(&ssd);
}

void exibir_lista_cores()
{
    ssd1306_fill(&ssd, false);
    ssd1306_draw_string(&ssd, "Escolha uma cor", 0, 0); 

    for (int i = 0; i < 5; i++)
    {
        ssd1306_draw_string(&ssd, opcoes[i], 0, 10 + (i * 10)); // Exibe as opções de cores

        if (i == opcao_selecionada)
        {
            ssd1306_draw_char(&ssd, '<', 100, 10 + (i * 10)); // Indica a cor selecionada
        }
    }

    ssd1306_send_data(&ssd);
}

/* ==========================================================
   GERENCIAMENTO DE ALARMES
   ========================================================== */
void adicionar_alarme(int horas, int minutos, int cor)
{
    for(int i = 0; i < total_alarmes; i++)
    {
        if(alarmes[i].horas == horas && alarmes[i].minutos == minutos) // Verifica se o alarme já existe
        {
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
    if (total_alarmes < MAX_ALARMES) // Adiciona o alarme à lista
    {
        buzzer_confirmacao();
        alarmes[total_alarmes] = (Alarme){horas, minutos, cor, 0};
        total_alarmes++;
        printf("Alarme salvo: %02d:%02d | Cor: %d\n", horas, minutos, cor);
    }
    else 
    {
            ssd1306_fill(&ssd, false);
            ssd1306_rect(&ssd, 1, 1, 126, 62, 1, 0);
            ssd1306_draw_string(&ssd, "LIMITE ATINGIDO", 5, 25);
            ssd1306_send_data(&ssd);
            buzzer_erro();
            sleep_ms(2000);
    }
}

void verificar_alarmes()
{
    rtc_get_datetime(&t);

    for (int i = 0; i < total_alarmes; i++)
    {
        if (t.hour == alarmes[i].horas && t.min == alarmes[i].minutos && t.sec == alarmes[i].segundos) // Verifica se o alarme está ativo
        {
            alarme_ativo = true;
            em_configuracao = true;

            display_time();
            drawMatrix(alarmes[i].cor); // Exibe o alarme no LED
            buzzer_alarme(); // Emite o alarme sonoro

            drawMatrix(5); // Reseta o LED
            em_configuracao = false;
            alarme_ativo = false;
        }
    }
}

/* ==========================================================
   CONFIGURAÇÃO INICIAL
   ========================================================== */
void setup()
{
    setup_PIO();
    setup_I2C(I2C_PORT, I2C_SDA, I2C_SCL);
    setup_ssd1306(&ssd, endereco, I2C_PORT);
    setup_buttons(BUTTON_Jytk);
    setup_buttons(BUTTON_Editar);
    setup_buttons(BUTTON_Confirmar);
    init_buzzer();
}