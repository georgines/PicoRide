#pragma once

#include "Sistema.h"
#include "auxiliarBuzzer.h"
#include "auxiliarTempo.h"

// Função para iniciar o contador
inline void iniciarContador(uint32_t &tempo_ms)
{
    tempo_restante_ms = tempo_ms;
    contador_ativo = true;
    printf("[TIMER] Contador iniciado com %ums\n", tempo_ms);
}

// Função para pausar o contador
inline bool pausarContador()
{
    if (contador_ativo)
    {
        contador_ativo = false;
        printf("[TIMER] Contador pausado\n");
        return true;
    }
    else
    {
        contador_ativo = true;
        printf("[TIMER] Contador retomado\n");
        return false;
    }
}

// Função para resetar o contador
inline void resetarContador()
{
    contador_ativo = false;
    tempo_restante_ms = 0;
    printf("[TIMER] Contador resetado\n");
}


 void processarComandoControle(char comando, bool &contador_pausado, uint32_t &temporizador_ms)
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

    printf("%s | Temporizador: %ums\n", mensagem, temporizador_ms);
}


void loopPrincipal(void *parametro)
{
    auto *sistema = static_cast<Sistema *>(parametro);

    char comando = 0;
    bool contador_pausado = false;
    static uint32_t temporizador_ms = 0;
    while (true)
    {
        // Receber comandos da fila
        if (xQueueReceive(fila_comandos, &comando, pdMS_TO_TICKS(10)))
        {
            processarComandoControle(comando, contador_pausado, temporizador_ms);
        }

        // Atualizar temporizador se não estiver pausado
        if (!contador_pausado && temporizador_ms > 0)
        {
            temporizador_ms -= 10; // Decrementa 10ms por iteração

            if (temporizador_ms == 0)
            {
                printf("[TIMER] Temporizador chegou a zero!\n");
                resetarContador();
                contador_pausado = true; // Pausa automaticamente ao atingir zero
            }
        }

        // Delay para evitar polling excessivo
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}
