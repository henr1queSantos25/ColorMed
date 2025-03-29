# ColorMed - Lembrete de Medicamentos por Cores

O **ColorMed** é um sistema embarcado projetado para facilitar a administração de medicamentos, com foco em idosos e pessoas com dificuldades de leitura. O sistema combina alarmes sonoros e visuais, permitindo uma identificação intuitiva por meio de LEDs coloridos.

**Atenção:** O **relógio interno (RTC)** precisa ser configurado na primeira inicialização e não pode ser desligado completamente após isso. O horário será resetado caso o microcontrolador seja desligado.

---

## Funcionalidades Principais

- **Configuração de Alarmes**: Defina horários para os medicamentos.
- **Identificação Visual**: Associe cada alarme a uma cor específica.
- **Alertas Sonoros e Visuais**: No horário programado, o buzzer emite um som e o LED RGB exibe a cor associada ao medicamento.
- **Interface Simples e Acessível**: Controle via botões físicos e exibição de informações no display OLED.

---

### Dica Prática  
Para otimizar ainda mais a identificação dos medicamentos:  
**Coloque etiquetas coloridas nas embalagens dos remédios**, correspondentes à cor selecionada no **ColorMed**. Essa simples ação pode tornar o processo ainda mais eficiente e intuitivo!

---

## Cores Associadas aos Medicamentos

- **Verde**  
- **Vermelho**  
- **Azul**  
- **Amarelo**  
- **Roxo**  

---

## Tecnologias Utilizadas

- **Linguagem de Programação**: C  
- **Microcontrolador**: Raspberry Pi Pico W  
- **Componentes Utilizados**:
  - Matriz de LED RGB 5x5  
  - Display OLED SSD1306 (I2C)  
  - Botões físicos  
  - Buzzer  
- **Bibliotecas e SDKs**:
  - `ssd1306` para o controle do display  
  - Pico SDK para manipulação de GPIO, temporizadores e interrupções  

---

## Como Usar

### Configurando um Alarme
1. Pressione o joystick para iniciar a configuração.
2. Ajuste as horas com o botão **Editar** e pressione **Confirmar** para avançar.
3. Configure os minutos e selecione uma cor para o medicamento.
4. Confirme para finalizar a configuração.

### Alarme Ativo
- Quando o horário do alarme chega:
  - O buzzer toca repetidamente.
  - A matriz de LED exibe a cor do medicamento.
  - A tela OLED exibe a mensagem **"ALARME ATIVO!"**.

---

## Configuração do Hardware

| Componente            | Pino do Pico        | Função                            |
|-----------------------|---------------------|-----------------------------------|
| Botão Configuração     | GP22                | Iniciar configuração do alarme    |
| Botão Editar           | GP5                 | Ajuste de horas, minutos e cor    |
| Botão Confirmar        | GP6                 | Confirma seleção                  |
| Matriz de LED RGB      | GP7                 | Exibir cor do medicamento         |
| Buzzer                 | GP10                | Alerta sonoro                     |
| Display OLED (I2C)     | SDA (GP14) / SCL (GP15) | Exibição de informações        |

---

## Estrutura do Repositório

- **`ColorMed.c`**: Código principal (RTC, alarmes, display e botões).  
- **`libs/buttons.h` & `libs/buttons.c`**: Controle dos botões e debounce.  
- **`libs/buzzer.h` & `libs/buzzer.c`**: Controle dos alertas sonoros.  
- **`libs/led_5x5.h` & `libs/led_5x5.c`**: Controle da matriz de LED.  
- **`libs/ssd1306.h` & `libs/ssd1306.c`**: Gerenciamento do display OLED.  
- **`libs/font.h`**: Fonte customizada para o display.  
- **`ColorMed.pio`**: Código PIO para controle eficiente dos LEDs.  
- **`diagram.json`**: Configuração do circuito para simulação no **Wokwi**.  
- **`Documentação/`**: Relatório técnico e detalhes do projeto.

---

## Demonstração do Projeto

Assista ao vídeo demonstrativo: [Link para o vídeo](https://youtu.be/iGTniXzGSGA)

---

## Conceitos Aplicados

- **RTC (Relógio de Tempo Real)** para armazenar o horário.
- **I2C** para comunicação com o display OLED.
- **Interrupções e debounce** para captura eficiente de eventos dos botões.

---

## Objetivos Alcançados

- **Facilidade de uso** para pessoas com dificuldades de leitura.  
- **Alertas eficientes** para garantir a correta administração dos medicamentos.  
- **Código modularizado**, permitindo expansões futuras.

---

## Links Úteis  

- [Documentação do Pico SDK](https://raspberrypi.github.io/pico-sdk-doxygen/index.html)  
- [Datasheet do Display SSD1306](https://cdn-shop.adafruit.com/datasheets/SSD1306.pdf)  

---

## Desenvolvido por  

- **Henrique Santos**  
- [LinkedIn](https://www.linkedin.com/in/dev-henriqueo-santos/)
