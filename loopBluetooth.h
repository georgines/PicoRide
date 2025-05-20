#pragma once

#include "Sistema.h"
#include "auxiliarBuzzer.h"

void enviarComandoBluetooth(const char *mensagem)
{
    if (!dispostivoBluetoothConectado)
        return;   
        bt.enviarString(mensagem);      
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
    bt.definirCallbackRecebimento(callbackRecebimentoDados);
    bt.definirCallbackConexao(aoConectar);
    bt.definirCallbackDesconexao(aoDesconectar);
    bt.iniciarLoopTimer(callbackLoopBluetooth, 50);
    bt.executar();
}