#pragma once

#include "Sistema.h"
#include "auxiliarBuzzer.h"

void loopPrincipal(void *parametro)
{

    Sistema *sistema = (Sistema *)parametro;

    char comando;
    uint32_t temporizador = 0;
    bool iniciarContagemTempo = false;
    uint64_t tempo_final = 0;

    while (true)
    {
        if (xQueueReceive(fila_comandos, &comando, portMAX_DELAY))
        {
            switch (comando)
            {
            case 'a':
                temporizador += TEMPO_05000_MS;
                break;
            case 'b':
                temporizador += TEMPO_10000_MS;
                break;
            case 'c':
                temporizador += TEMPO_20000_MS;
                break;
            case 'd':
                temporizador += TEMPO_30000_MS;
                break;
            case 'e':
                temporizador += TEMPO_40000_MS;
                break;
            case 'f':
                temporizador += TEMPO_50000_MS;
                break;
            case 'g':
                temporizador += TEMPO_60000_MS;
                break;
            case 'i':
                if (temporizador > 0)
                {
                    iniciarContagemTempo = true;
                    tempo_final = time_us_64() + (uint64_t)temporizador * 1000;
                }
                break;
            case 'p':
                iniciarContagemTempo = false;
                break;
            case 'r':
                temporizador = 0;
                break;
            default:
                break;
            }
        }

        if (iniciarContagemTempo)
        {
            if (time_us_64() >= tempo_final)
            {
                acionarBuzzer(*sistema);
                iniciarContagemTempo = false;
            }
        }

        vTaskDelay(pdMS_TO_TICKS(1));
    }
}