#pragma once

#include "Sistema.h"
#include "auxiliarBuzzer.h"
#include "auxiliarTempo.h"

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

    // bt.enviarString("Received: ");
    // bt.enviar(data, len);
    char ch = data[0];
    xQueueSend(fila_comandos, &ch, pdMS_TO_TICKS(10));
    // bt.enviarStringFormatada("Tempo Total:  %u\n", temporizador_ms);
}

void onDataExterno()
{
    receberSerialUsbEEnviarParaFila();
    // vTaskDelay(pdMS_TO_TICKS(25));
}

void loopBluetooth(void *parametro)
{
    bt.definirCallbackRecebimento(onData);
    bt.iniciarLoopTimer(onDataExterno, 50);
    bt.executar();
}