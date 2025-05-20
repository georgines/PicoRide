#pragma once

#include "Sistema.h"
#include "auxiliarBuzzer.h"

void receberSerialUsbEEnviarParaFila()
{
    while (uart_is_readable(uart0))
    {
        char ch = uart_getc(uart0);                      
        xQueueSend(fila_comandos, &ch, pdMS_TO_TICKS(10)); 
    }
}

void onData(const uint8_t *data, uint16_t len)
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
    bt.definirCallbackRecebimento(onData);
    bt.iniciarLoopTimer(callbackLoopBluetooth, 50);
    bt.executar();
}