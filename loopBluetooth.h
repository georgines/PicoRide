#pragma once

#include "Sistema.h"
#include "auxiliarBuzzer.h"

void receberSerialUsbEEnviarParaFila()
{
    while (stdio_usb_connected())
    {
        int c = getchar_timeout_us(0);
        if (c == PICO_ERROR_TIMEOUT)
            break;
        char ch = (char)c;
        xQueueSend(fila_comandos, &ch, pdMS_TO_TICKS(10));
    }
}

void loopBluetooth(void *parametro)
{
    Sistema *sistema = (Sistema *)parametro;

    while (true)
    {
        receberSerialUsbEEnviarParaFila();
        vTaskDelay(pdMS_TO_TICKS(1));
    }
}