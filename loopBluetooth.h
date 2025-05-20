#pragma once

#include "Sistema.h"
#include "auxiliarBuzzer.h"

#define TAMANHO_DA_PILHA_LOOP_BLUETOOTH 8192
#define PRIORIDADE_LOOP_BLUETOOTH 2

void enviarComandoBluetooth(const char *mensagem)
{
    if (!dispostivoBluetoothConectado)
        return;
    bluetooth.enviarString(mensagem);
}

void aoConectar()
{
    printf("Dispositivo conectado\n");
    dispostivoBluetoothConectado = true;
}

void aoDesconectar()
{
    printf("Dispositivo desconectado\n");
    dispostivoBluetoothConectado = false;
}

void receberSerialUsbEEnviarParaFila()
{
    while (uart_is_readable(uart0))
    {
        char ch = uart_getc(uart0);
        xQueueSend(fila_comandos, &ch, pdMS_TO_TICKS(10));
    }
}

void callbackRecebimentoDados(const uint8_t *data, uint16_t len)
{

    char ch = data[0];
    xQueueSend(fila_comandos, &ch, pdMS_TO_TICKS(10));
}

void callbackLoopBluetooth()
{

    receberSerialUsbEEnviarParaFila();
}

void loopBluetooth(void *parametro)
{
    bluetooth.definirCallbackRecebimento(callbackRecebimentoDados);
    bluetooth.definirCallbackConexao(aoConectar);
    bluetooth.definirCallbackDesconexao(aoDesconectar);
    bluetooth.iniciarLoopTimer(callbackLoopBluetooth, 50);
    bluetooth.executar();
}

void inicializarLoopBluetooth()
{
    xTaskCreate(loopBluetooth, "loopBluetooth", TAMANHO_DA_PILHA_LOOP_BLUETOOTH, nullptr, PRIORIDADE_LOOP_BLUETOOTH, nullptr);
}