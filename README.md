# 🏥 ColorMed - Lembrete de Medicamentos por Cores  

O **ColorMed** é um sistema embarcado desenvolvido para auxiliar na administração de medicamentos, especialmente para idosos e pessoas com dificuldades de leitura. Ele permite configurar alarmes que acendem LEDs coloridos e emitem alertas sonoros, tornando a identificação dos medicamentos mais intuitiva e acessível.  

📌 **Atenção:** O **relógio interno (RTC)** precisa ser configurado **na primeira vez que o sistema for iniciado** e **não pode ser desligado completamente depois**. Caso o microcontrolador seja desligado, o horário será resetado e precisará ser configurado novamente.  

---

## 🚀 Funcionalidades  

### ⏰ Lembrete Inteligente de Medicamentos  
- **Configuração de Alarmes**: O usuário define horários para os medicamentos.  
- **Associação de Cores**: Cada alarme pode ser vinculado a uma cor específica.  
- **Alerta Sonoro e Visual**: No horário programado, um **buzzer** toca e um **LED RGB 5x5** exibe a cor do medicamento correspondente.  

### 🎛️ Interface Simples e Acessível  
- **Botões para Configuração**:  
  - **Confirmar**: Avança nas etapas da configuração.  
  - **Editar**: Ajusta horas, minutos e cores.  
  - **Joystick**: Inicia a configuração do alarme.  
- **Display OLED**: Exibe as informações do sistema, facilitando o uso.  

### 🎨 Identificação Visual  
- **Cada medicamento é representado por uma cor**:  
  - 🟢 **Verde**  
  - 🔴 **Vermelho**  
  - 🔵 **Azul**  
  - 🟡 **Amarelo**  
  - 🟣 **Roxo**  

### 🔔 Alarme Programado  
- Quando o horário do alarme chega:  
  - O **buzzer** toca um som característico.  
  - A **matriz de LED** exibe a cor associada ao medicamento.  
  - A tela exibe **"ALARME ATIVO!"** para reforçar o aviso.  

📌 **Edição e remoção de alarmes** não foram implementadas devido às limitações dos botões disponíveis na **BitDogLab**, mas essa funcionalidade poderá ser adicionada no futuro.  

---

## 🛠️ Tecnologias Utilizadas  

- **Linguagem de Programação**: C  
- **Microcontrolador**: Raspberry Pi Pico W
- **Componentes**:  
  - **Matriz de LED RGB 5x5**  
  - **Display OLED SSD1306 (I2C)**  
  - **Botões físicos**  
  - **Buzzer para alarmes**  
- **Bibliotecas**:  
  - **ssd1306** para o controle do display  
  - **Pico SDK** para manipulação de GPIO, temporizadores e interrupções  

---

## 🕹️ Como Usar  

### 1️⃣ Configurando um Alarme  
- Pressione **o Joystick** para iniciar a configuração.  
- Use o **botão Editar** para ajustar as horas.  
- Pressione **Confirmar** para salvar e passar para os minutos.  
- Ajuste os **minutos** da mesma forma.  
- Escolha uma **cor** para representar o medicamento.  
- Confirme para finalizar a configuração.

  💡 **Dica:** Para facilitar ainda mais a identificação, **coloque etiquetas coloridas nas embalagens dos remédios**, correspondentes à cor selecionada no **ColorMed**. Isso torna o sistema ainda mais eficiente e intuitivo!  

### 2️⃣ Alarme Ativo  
- No horário programado:  
  - A tela exibe **"ALARME ATIVO!"**.  
  - O buzzer toca repetidamente.  
  - A matriz de LED exibe a **cor do medicamento**.  

---

## 📟 Configuração do Hardware  

| Componente             | Pino do Pico | Função |
|------------------------|-------------|-------------------------------|
| **Botão Configuração**     | GP22        | Inicia a configuração do alarme |
| **Botão Editar**       | GP5         | Ajusta horas, minutos e cor |
| **Botão Confirmar**    | GP6         | Confirma a seleção |
| **Matriz de LED RGB**  | GP7         | Exibe a cor do medicamento |
| **Buzzer**            | GP10        | Emite som no alarme |
| **Display OLED (I2C)** | SDA (GP14) / SCL (GP15) | Exibe interface |

---

## 🗂️ Estrutura do Repositório  


🔍 **Descrição dos Arquivos Principais**  

- **`ColorMed.c`** → Código principal que gerencia RTC, alarmes, display e botões.  
- **`libs/buttons.h` & `libs/buttons.c`** → Controle dos botões, incluindo debounce.  
- **`libs/buzzer.h` & `libs/buzzer.c`** → Controle do buzzer para sons de alerta e confirmação.  
- **`libs/led_5x5.h` & `libs/led_5x5.c`** → Controle da matriz de LED para exibir cores dos alarmes.  
- **`libs/ssd1306.h` & `libs/ssd1306.c`** → Gerenciamento do display OLED.  
- **`libs/font.h`** → Fonte customizada para caracteres no display.
- **`ColorMed.pio`**: Código PIO para controle eficiente dos LEDs.
- **`diagram.json`**: Configuração do circuito para simulação no **Wokwi**.  
- **`Documentação/`** → Pasta com o relatório técnico e detalhes do projeto.  

---

## 📹 Demonstração do Projeto  

🎥 **[Assista ao vídeo demonstrativo do ColorMed](https://youtu.be/iGTniXzGSGA)** 

---

## 📚 Conceitos Aplicados  

- **RTC (Relógio de Tempo Real)** para armazenar o horário. 
- **I2C** para comunicação com o display OLED.  
- **Interrupções e debounce** para captura de eventos dos botões.  

---

## 🎯 Objetivos Alcançados  

✅ **Facilidade de uso** para pessoas com dificuldades de leitura.  
✅ **Alertas visuais e sonoros** para garantir que os medicamentos sejam tomados no horário certo.  
✅ **Sistema modularizado**, permitindo futuras expansões.  

---

## 🔗 Links Úteis  

- [Documentação do Pico SDK](https://raspberrypi.github.io/pico-sdk-doxygen/index.html)  
- [Datasheet do Display SSD1306](https://cdn-shop.adafruit.com/datasheets/SSD1306.pdf)  

---

## 💡 Desenvolvido por  

- **Henrique Santos**  
- [LinkedIn](https://www.linkedin.com/in/dev-henriqueo-santos/)
