#pragma once

#include "Sistema.h"
#include "auxiliarBuzzer.h"
#include "loopContadorTempo.h"
#include "loopBluetooth.h"

void exibirMensagem(const char *mensagem, uint32_t temporizador_ms)
{
    char mensagem_formatada[64];
    snprintf(mensagem_formatada, sizeof(mensagem_formatada), "%s | Temporizador: %ums\n", mensagem, temporizador_ms);
    enviarComandoBluetooth(mensagem_formatada);
    printf(mensagem_formatada);
}

void processarComandoControle(char comando, bool &contador_pausado, volatile uint32_t &temporizador_ms, Sistema &sistema)
{
    const char *mensagem = "";

    switch (comando)
    {
    case 'a':
        temporizador_ms += TEMPO_5000_MS;
        mensagem = "+5s no temporizador.";
        break;
    case 'b':
        temporizador_ms += TEMPO_10000_MS;
        mensagem = "+10s no temporizador.";
        break;
    case 'c':
        temporizador_ms += TEMPO_20000_MS;
        mensagem = "+20s no temporizador.";
        break;
    case 'd':
        temporizador_ms += TEMPO_30000_MS;
        mensagem = "+30s no temporizador.";
        break;   
    case 'i':
        if (temporizador_ms == 0)
        {
            mensagem = "Defina o tempo antes de iniciar.";
            break;
        }
        iniciarContador(temporizador_ms);
        mensagem = "Contagem iniciada.";
        break;
    case 'p':
        contador_pausado = pausarContador();
        mensagem = contador_pausado ? "Contagem pausada." : "Contagem retomada.";
        break;
    case 'r':
        temporizador_ms = 0;
        resetarContador();
        mensagem = "Temporizador resetado.";
        break;
    default:
        mensagem = "Comando desconhecido.";
        break;
    }

    exibirMensagem(mensagem, temporizador_ms);
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
