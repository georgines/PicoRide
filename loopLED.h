#pragma once

#include "Sistema.h"

#define TAMANHO_PILHA_LOOP_LED 128
#define PRIORIDADE_LOOP_LED 1
#define TEMPO_DELAY_MS 1000

enum EstadoLED
{
    DESLIGADO,
    LIGADO
};

void ledVerde(bool estado)
{
    gpio_put(PINO_LED_VERDE, estado);
}

void ledVermelho(bool estado)
{
    gpio_put(PINO_LED_VERMELHO, estado);
}

// Função auxiliar para alternar o estado de um LED
void alternarLED(void (*ledFunc)(bool), EstadoLED estadoInicial, EstadoLED estadoFinal)
{
    ledFunc(estadoInicial);
    vTaskDelay(pdMS_TO_TICKS(TEMPO_DELAY_MS));
    ledFunc(estadoFinal);
    vTaskDelay(pdMS_TO_TICKS(TEMPO_DELAY_MS));
}

void loopLED(void *params)
{
    gpio_init(PINO_LED_VERMELHO);
    gpio_init(PINO_LED_VERDE);

    gpio_set_dir(PINO_LED_VERMELHO, GPIO_OUT);
    gpio_set_dir(PINO_LED_VERDE, GPIO_OUT);

    ledVermelho(EstadoLED::DESLIGADO);
    ledVerde(EstadoLED::DESLIGADO);

    while (true)
    {
        if (dispostivoBluetoothConectado)
        {
            alternarLED(ledVerde, EstadoLED::LIGADO, EstadoLED::DESLIGADO);
            ledVermelho(EstadoLED::DESLIGADO);
        }
        else
        {
            alternarLED(ledVermelho, EstadoLED::LIGADO, EstadoLED::DESLIGADO);
            ledVerde(EstadoLED::DESLIGADO);
        }
    }
}

void inicializarLoopLED()
{
    xTaskCreate(loopLED, "loopLED", TAMANHO_PILHA_LOOP_LED, nullptr, PRIORIDADE_LOOP_LED, nullptr);
}
