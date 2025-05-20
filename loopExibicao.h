#pragma once

#include "Sistema.h"
#include "loopContadorTempo.h"
#include "Oled.h"

#define TAMANHO_MAX_STRING 255

volatile char texto_exibido[TAMANHO_MAX_STRING + 1];

QueueHandle_t fila_exibicao;

SemaphoreHandle_t semaforo_exibicao;

void loopExibicao(void *params)
{
    char buffer[TAMANHO_MAX_STRING + 1];

    Oled tela(i2c1, PINO_SDA_TELA, PINO_SCL_TELA);
    tela.definirFonte(fonte_customizada);
    tela.inicializar();
    tela.limpar();

    while (true)
    {
        // if (xQueueReceive(fila_exibicao, &buffer, pdMS_TO_TICKS(10)) == pdTRUE)
        // {
        //     buffer[TAMANHO_MAX_STRING] = '\0';
        // }

        // if (xSemaphoreTake(semaforo_exibicao, portMAX_DELAY) == pdTRUE)
        // {

        //     xSemaphoreGive(semaforo_exibicao);
        // }

        if (tempo_restante != ultomo_tempo_restante)
        { 
            tela.imprimir(0, LINHA_TITULO, "%s", "    PicoRide    ");
            tela.imprimir(0, LINHA_1, "Tempo locacao   ");
            tela.limparLinha(LINHA_2);
            tela.imprimir(0, LINHA_2, "%ds", tempo_restante);
            tela.renderizarDisplay();
            printf("Tempo restante: %u ms\n", tempo_restante);
            ultomo_tempo_restante = tempo_restante;     
        }
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

void inicializarLoopExibicao()
{
    semaforo_exibicao = xSemaphoreCreateMutex();
    fila_exibicao = xQueueCreate(5, sizeof(char[TAMANHO_MAX_STRING + 1]));
    xTaskCreate(loopExibicao, "loopExibicao", 8192, nullptr, 1, nullptr);
}
