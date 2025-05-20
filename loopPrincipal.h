#pragma once

#include "Sistema.h"
#include "auxiliarBuzzer.h"
#include "loopContadorTempo.h"
#include "loopBluetooth.h"

void processarComandoControle(char comando, bool &contador_pausado, volatile uint32_t &temporizador_ms, Sistema &sistema)
{
    const char *mensagem = "";

    switch (comando)
    {
    case 'a':
        temporizador_ms += TEMPO_5000_MS;
        mensagem = "[CMD a] +5000ms";
        break;
    case 'b':
        temporizador_ms += TEMPO_10000_MS;
        mensagem = "[CMD b] +10000ms";
        break;
    case 'c':
        temporizador_ms += TEMPO_20000_MS;
        mensagem = "[CMD c] +20000ms";
        break;
    case 'd':
        temporizador_ms += TEMPO_30000_MS;
        mensagem = "[CMD d] +30000ms";
        break;
    case 'e':
        temporizador_ms += TEMPO_40000_MS;
        mensagem = "[CMD e] +40000ms";
        break;
    case 'f':
        temporizador_ms += TEMPO_50000_MS;
        mensagem = "[CMD f] +50000ms";
        break;
    case 'g':
        temporizador_ms += TEMPO_60000_MS;
        mensagem = "[CMD g] +60000ms";
        break;
    case 'i':
        if (temporizador_ms == 0)
        {
            mensagem = "[CMD i] Nenhum tempo definido";
            break;
        }
        iniciarContador(temporizador_ms);
        mensagem = "[CMD i] Início da contagem";
        break;
    case 'p':
        contador_pausado = pausarContador();
        mensagem = contador_pausado ? "[CMD p] Contagem pausada" : "[CMD p] Contagem retomada";
        break;
    case 'r':
        temporizador_ms = 0;
        resetarContador();
        mensagem = "[CMD r] Temporizador resetado";
        break;
    default:
        mensagem = "[CMD ?] Comando desconhecido";
        break;
    }

    enviarComandoBluetooth(mensagem);
    printf("%s | Temporizador: %ums\n", mensagem, temporizador_ms);
    acionarBuzzer(sistema);
}

void loopPrincipal(void *parametro)
{
    Sistema *sistema = reinterpret_cast<Sistema *>(parametro);
    char comando = 0;
    bool contador_pausado = false;

    while (true)
    {

        if (xQueueReceive(fila_comandos, &comando, pdMS_TO_TICKS(10)))
        {
            processarComandoControle(comando, contador_pausado, tempo_restante, *sistema);
        }

        vTaskDelay(pdMS_TO_TICKS(10));
    }
}
