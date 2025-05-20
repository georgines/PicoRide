#include "SerialBluetooth.h"
#include "pico/stdlib.h"
#include <cstdio>
#include "hardware/uart.h"

BluetoothSerial bt("MeuBluetooth");

void onData(const uint8_t *data, uint16_t len)
{
    bt.enviarStringFormatada("Recebido %c\n", data[0]);
}

void onDataExterno()
{
    printf("Callback externo chamado\n");
}

void onConnect() {
    printf("Dispositivo conectado\n");
}

void onDisconnect() {
    printf("Dispositivo desconectado\n");
}

int main()
{
     stdio_init_all();
    uart_init(uart0, 115200);

    // Configurar GPIO 0 e 1 para UART0
    gpio_set_function(0, GPIO_FUNC_UART); // TX
    gpio_set_function(1, GPIO_FUNC_UART); // RX

    uart_set_format(uart0, 8, 1, UART_PARITY_NONE);

    // Habilitar FIFO da UART
    uart_set_fifo_enabled(uart0, true);

    printf("Iniciando...\n");

    bt.definirCallbackRecebimento(onData);
    bt.definirCallbackConexao(onConnect);
    bt.definirCallbackDesconexao(onDisconnect);
    bt.iniciarLoopTimer(onDataExterno, 1000);
    bt.executar();
}
