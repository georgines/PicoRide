#pragma once

#include "Sistema.h"

SemaphoreHandle_t mutex_tempo;
volatile uint32_t tempo_restante_ms = 0;
volatile uint32_t temporizador_ms = 0;
volatile bool contador_ativo = false;

void iniciarContador(uint32_t tempo_ms)
{
  if (xSemaphoreTake(mutex_tempo, portMAX_DELAY) == pdTRUE)
  {
    tempo_restante_ms = tempo_ms;
    contador_ativo = true;
    xSemaphoreGive(mutex_tempo);
  }
}

void resetarContador()
{
  if (xSemaphoreTake(mutex_tempo, portMAX_DELAY) == pdTRUE)
  {
    contador_ativo = false;
    tempo_restante_ms = 0;
    xSemaphoreGive(mutex_tempo);
  }
}

bool pausarContador()
{
  bool pausou = false;
  if (xSemaphoreTake(mutex_tempo, portMAX_DELAY) == pdTRUE)
  {
    if (contador_ativo)
    {
      contador_ativo = false;
      pausou = true;
    }
    else if (tempo_restante_ms > 0)
    {
      contador_ativo = true;
      pausou = false;
    }
    xSemaphoreGive(mutex_tempo);
  }
  return pausou;
}

bool timer_callback(repeating_timer_t *rt)
{
  BaseType_t xHigherPriorityTaskWoken = pdFALSE;
  if (xSemaphoreTakeFromISR(mutex_tempo, &xHigherPriorityTaskWoken) == pdTRUE)
  {
    if (contador_ativo && tempo_restante_ms > 0)
    {
      tempo_restante_ms--;
      printf("Tempo restante: %ums\n", tempo_restante_ms);
    }
    xSemaphoreGiveFromISR(mutex_tempo, &xHigherPriorityTaskWoken);
  }
  return true;
}

void iniciarContagemTempoDoCronometro(repeating_timer_t tempo)
{
  add_repeating_timer_ms(-1, timer_callback, NULL, &tempo);
}
