#pragma once

#include "Sistema.h"
#include "auxiliarBuzzer.h"
#include "auxiliarTempo.h"
#include <array>

// Função para processar comandos de incremento de tempo
inline bool processarComandoTempo(char comando, uint32_t &temporizador_ms) {
    constexpr std::array<uint32_t, 7> incrementos = {
        TEMPO_5000_MS, TEMPO_10000_MS, TEMPO_20000_MS, TEMPO_30000_MS,
        TEMPO_40000_MS, TEMPO_50000_MS, TEMPO_60000_MS
    };
    constexpr char comandos_tempo[] = {'a','b','c','d','e','f','g'};
    for (size_t i = 0; i < incrementos.size(); ++i) {
        if (comando == comandos_tempo[i]) {
            temporizador_ms += incrementos[i];
            printf("[CMD %c] +%ums | Temporizador: %ums\n", comando, incrementos[i], temporizador_ms);
            return true;
        }
    }
    return false;
}

// Função para processar comandos de controle do temporizador
inline void processarComandoControle(char comando, uint32_t &temporizador_ms, bool &contador_pausado) {
    switch (comando)
    {
    case 'i':
        iniciarContador(temporizador_ms);
        printf("[CMD i] Início da contagem! Tempo total: %ums\n", temporizador_ms);
        return;
    case 'p':
        contador_pausado = pausarContador();
        printf(contador_pausado ? "[CMD p] Contagem pausada.\n" : "[CMD p] Contagem retomada.\n");
        return;
    case 'r':
        temporizador_ms = 0;
        resetarContador();
        printf("[CMD r] Temporizador resetado.\n");
        return;
    default:
        printf("[CMD ?] Comando desconhecido: %c\n", comando);
        return;
    }
}

// Loop principal do sistema: gerencia comandos recebidos e controla o temporizador
inline void loopPrincipal(void *parametro)
{
    auto *sistema = static_cast<Sistema *>(parametro);

    char comando = 0;    
    bool contador_pausado = false;

    while (true)
    {
        if (xQueueReceive(fila_comandos, &comando, 0))
        {
            if (!processarComandoTempo(comando, temporizador_ms)) {
                processarComandoControle(comando, temporizador_ms, contador_pausado);
            }
            acionarBuzzer(*sistema);
        }
        vTaskDelay(pdMS_TO_TICKS(1));
    }
}
