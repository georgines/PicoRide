#pragma once

#include "Sistema.h"

#define TAMANHO_PILHA_LOOP_CONTADOR_TEMPO 128
#define PRIORIDADE_LOOP_CONTADOR_TEMPO 3
#define TAMANHO_FILA_TEMPO 4

typedef enum
{
    INICIAR,
    PAUSAR,
    CONTINUAR,
    RESETAR
} StatusTemporizador_t;

typedef struct
{
    uint32_t valor_ms;
    StatusTemporizador_t cmd;
} MsgTemporizador_t;

volatile uint32_t tempo_restante = 0;
volatile uint32_t ultomo_tempo_restante = -1;
volatile StatusTemporizador_t estado = RESETAR;

QueueHandle_t filaTemporizador;
SemaphoreHandle_t mutexTemporizador;

void iniciarContador(uint32_t valor_ms)
{
    MsgTemporizador_t m = {.valor_ms = valor_ms, .cmd = INICIAR};
    xQueueSend(filaTemporizador, &m, portMAX_DELAY);
}

bool pausarContador()
{
    MsgTemporizador_t m = {.cmd = (estado == INICIAR || estado == CONTINUAR) ? PAUSAR : CONTINUAR};
    xQueueSend(filaTemporizador, &m, portMAX_DELAY);
    return (m.cmd == PAUSAR);
}

void resetarContador()
{
    MsgTemporizador_t m = {.cmd = RESETAR};
    xQueueSend(filaTemporizador, &m, portMAX_DELAY);
}

void iniciarContadorDeTempo(uint32_t valor_ms)
{
    MsgTemporizador_t m = {.valor_ms = valor_ms, .cmd = INICIAR};
    xQueueSend(filaTemporizador, &m, portMAX_DELAY);
}

static void loopContadorDeTempo(void *pv)
{
    MsgTemporizador_t msg;
    TickType_t tick_ant = xTaskGetTickCount();
    const TickType_t intervalo = pdMS_TO_TICKS(1);

    while (true)
    {
        if (xQueueReceive(filaTemporizador, &msg, 0) == pdPASS)
        {
            if (xSemaphoreTake(mutexTemporizador, portMAX_DELAY) == pdTRUE)
            {
                switch (msg.cmd)
                {
                case INICIAR:
                    tempo_restante = msg.valor_ms;
                    estado = INICIAR;
                    break;
                case PAUSAR:
                    estado = PAUSAR;
                    break;
                case CONTINUAR:
                    estado = CONTINUAR;
                    break;
                case RESETAR:
                    tempo_restante = 0;
                    estado = RESETAR;
                    break;
                }
                xSemaphoreGive(mutexTemporizador);
            }
        }
     
        if (estado == INICIAR || estado == CONTINUAR)
        {
            if (xSemaphoreTake(mutexTemporizador, portMAX_DELAY) == pdTRUE)
            {
                if (tempo_restante > 0)
                {
                    tempo_restante--;
                }
                xSemaphoreGive(mutexTemporizador);
            }
        }

        vTaskDelayUntil(&tick_ant, intervalo);
    }
}

void iniciarContadorDeTempo()
{   
    filaTemporizador = xQueueCreate(TAMANHO_FILA_TEMPO, sizeof(MsgTemporizador_t));
    mutexTemporizador = xSemaphoreCreateMutex();  
    xTaskCreate(loopContadorDeTempo, "LoopContadorDeTempo", TAMANHO_PILHA_LOOP_CONTADOR_TEMPO, NULL, PRIORIDADE_LOOP_CONTADOR_TEMPO, NULL);
}
