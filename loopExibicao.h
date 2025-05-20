#pragma once

#include "Sistema.h"
#include "loopContadorTempo.h"
#include "Oled.h"

#define TAMANHO_MAX_STRING 255
#define TAMANHO_PILHA_LOOP_EXIBICAO 8192
#define PRIORIDADE_LOOP_EXIBICAO 1

volatile char texto_exibido[TAMANHO_MAX_STRING + 1];

void loopExibicao(void *params)
{
    char buffer[TAMANHO_MAX_STRING + 1];

    Oled tela(i2c1, PINO_SDA_TELA, PINO_SCL_TELA);
    tela.definirFonte(fonte_customizada);
    tela.inicializar();
    tela.limpar();

    while (true)
    {

        if (tempo_restante != ultimo_tempo_restante || dispostivoBluetoothConectado != ultimo_dispostivoBluetoothConectado)
        {
            tela.imprimir(0, LINHA_TITULO, "%s", "    PicoRide    ");
            tela.imprimir(0, LINHA_1, "Tempo locacao   ");
            tela.limparLinha(LINHA_2);
            tela.imprimir(0, LINHA_2, "%ds", tempo_restante);
            tela.limparLinha(LINHA_3);
            tela.imprimir(0, LINHA_3, "BT: %s", dispostivoBluetoothConectado ? "Conec." : "Desconec.");
            tela.renderizarDisplay();
            printf("Tempo restante: %u ms\n", tempo_restante);
            ultimo_tempo_restante = tempo_restante;
            ultimo_dispostivoBluetoothConectado = dispostivoBluetoothConectado;
        }
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

void inicializarLoopExibicao()
{    
    xTaskCreate(loopExibicao, "loopExibicao", TAMANHO_PILHA_LOOP_EXIBICAO, nullptr, PRIORIDADE_LOOP_EXIBICAO, nullptr);
}
