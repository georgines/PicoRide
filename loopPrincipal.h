#pragma once

#include "Sistema.h"
#include "auxiliarBuzzer.h"
#include "auxiliarTempo.h"
#include <array>

// Loop principal do sistema: gerencia comandos recebidos e controla o temporizador
inline void loopPrincipal(void *parametro)
{
    auto *sistema = static_cast<Sistema *>(parametro);

    char comando = 0;
    uint32_t temporizador_ms = 0;
    bool contador_pausado = false;

    // Tabela de incremento de tempo para comandos 'a' a 'g'
    constexpr std::array<uint32_t, 7> incrementos = {
        TEMPO_5000_MS, TEMPO_10000_MS, TEMPO_20000_MS, TEMPO_30000_MS,
        TEMPO_40000_MS, TEMPO_50000_MS, TEMPO_60000_MS
    };
    constexpr char comandos_tempo[] = {'a','b','c','d','e','f','g'};

    while (true)
    {
        if (xQueueReceive(fila_comandos, &comando, 0))
        {
            // Verifica se o comando é de incremento de tempo
            bool comando_reconhecido = false;
            for (size_t i = 0; i < incrementos.size(); ++i) {
                if (comando == comandos_tempo[i]) {
                    temporizador_ms += incrementos[i];
                    printf("[CMD %c] +%ums | Temporizador: %ums\n", comando, incrementos[i], temporizador_ms);
                    comando_reconhecido = true;
                    break;
                }
            }
            if (!comando_reconhecido) {
                switch (comando)
                {
                case 'i':
                    iniciarContador(temporizador_ms);
                    printf("[CMD i] Início da contagem! Tempo total: %ums\n", temporizador_ms);
                    break;
                case 'p':
                    contador_pausado = pausarContador();
                    printf(contador_pausado ? "[CMD p] Contagem pausada.\n" : "[CMD p] Contagem retomada.\n");
                    break;
                case 'r':
                    temporizador_ms = 0;
                    resetarContador();
                    printf("[CMD r] Temporizador resetado.\n");
                    break;
                default:
                    printf("[CMD ?] Comando desconhecido: %c\n", comando);
                    break;
                }
            }
            acionarBuzzer(*sistema);
        }
        vTaskDelay(pdMS_TO_TICKS(1));
    }
}
