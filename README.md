# PicoRide

## Visão Geral

PicoRide é um projeto portátil de gestão de locação de veículos infantis, como motos elétricas, controlado via Bluetooth. Ele permite gerenciar o temporizador dos veículos, oferecendo uma solução prática e eficiente para locações temporárias. O projeto demonstra a integração de sistemas operacionais em tempo real com hardware embarcado para criar uma aplicação funcional.

## Funcionalidades

- **Comunicação Bluetooth**: Permite comunicação sem fio com dispositivos externos.
- **Display OLED**: Exibe informações em tempo real, como contagem regressiva do temporizador e status da conexão Bluetooth.
- **Controle de LEDs**: Indicadores visuais para estados do acionamento da energia do veiculo.
- **Controle de Buzzer**: Fornece feedback sonoro para ações do usuário.
- **Integração com FreeRTOS**: Gerencia múltiplas tarefas de forma eficiente.

## Instalação do FreeRTOS

Este projeto utiliza o FreeRTOS para gerenciamento de tarefas. Para instalar o FreeRTOS, clone o repositório para pasta raiz no link abaixo:

[Repositório FreeRTOS](https://github.com/FreeRTOS/FreeRTOS-Kernel.git)

### Passos para Instalar o FreeRTOS

1. Clone o repositório:

   ```bash
   git clone https://github.com/FreeRTOS/FreeRTOS-Kernel.git freertos
   ```

## Como Configurar o Ambiente

1. Instale o [Visual Studio Code (VS Code)](https://code.visualstudio.com/).
2. Instale o [Git](https://git-scm.com/).
3. No VS Code, instale a extensão "Raspberry Pi Pico".
4. Certifique-se de que a SDK do Raspberry Pi Pico W está configurada na versão 2.1.1.
5. Importe o projeto utilizando a extensão "Raspberry Pi Pico" no VS Code. Basta abrir a pasta do projeto e seguir as instruções da extensão para configurar o ambiente.

## Como Compilar e Executar

1. Certifique-se de que o SDK do Raspberry Pi Pico está instalado.
2. Use o botão "Compile" da extensão "Raspberry Pi Pico" no VS Code para compilar o projeto.
3. Conecte o Raspberry Pi Pico W ao computador via USB enquanto mantém o botão BOOTSEL pressionado para entrar no modo de carregamento do bootloader.
4. Após compilar, clique no botão "Run" da extensão para carregar o binário no dispositivo.

### Uso da Serial

Para utilizar a comunicação serial:

- Conecte um adaptador USB-Serial ao dispositivo.
- Conecte o GND do adaptador ao GND do Pico, o RX do adaptador ao TX do Pico (GPIO 0), e o TX do adaptador ao RX do Pico (GPIO 1).
- Configure a velocidade da comunicação serial para 115200 bps.

## Bluetooth

A biblioteca de Bluetooth utilizada neste projeto foi criada com base na seguinte biblioteca:

[MrGreensWorkshop RasPiPicoSDK_BT_Classic_SPP](https://github.com/MrGreensWorkshop/RasPiPicoSDK_BT_Classic_SPP/tree/main)

## Licença

Este projeto é open-source e está disponível sob a licença MIT.