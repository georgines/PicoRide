#pragma once

#include "FreeRTOS.h"
#include "pico/stdlib.h"
#include <stdio.h>
#include "Oled.h"
#include "PWM.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

// pinos tela
#define PINO_SDA_TELA 14
#define PINO_SCL_TELA 15

// menu
#define MODO_LIVRE 0
#define MODO_MEMORIA 1
#define MODO_ZERAR_X 2
#define MODO_ZERAR_Y 3

// linhas menu
#define LINHA_TITULO 0
#define LINHA_1 20
#define LINHA_2 32
#define LINHA_3 44
#define LINHA_MENU 56

// buzzer
#define PINO_BUZZER 10
#define WRAP_BUZZER 4095            // Configurado para 1 kHz
#define DIVISOR_CLOCK_BUZZER 30.52f // Configurado para 1 kHz
#define DUTY_INICIAL_BUZZER 0
#define DUTY_BUZZER 50
#define DUTY_DESLIGAR_BUZZER DUTY_INICIAL_BUZZER
#define TEMPO_BUZZER_MS 100

#define TEMPO_5000_MS  5000
#define TEMPO_10000_MS 10000
#define TEMPO_20000_MS 20000
#define TEMPO_30000_MS 30000 
#define TEMPO_40000_MS 40000
#define TEMPO_50000_MS 50000
#define TEMPO_60000_MS 60000

struct Sistema
{   
    Oled &tela;
    PWM &buzzer;
    volatile absolute_time_t inicioAtivacao;
    volatile bool ativo;
};

QueueHandle_t fila_comandos; 

// Protótipos das funções organizados por responsabilidades

// funcoes buzzer
absolute_time_t pegarTempoAbsolutoAtual();
void inicializarContadorDeTempoDoBuzzer(Sistema &sistema, repeating_timer &timer);
bool desabilitarBuzzer(struct repeating_timer *t);
void acionarBuzzer(Sistema &sistema);

//função de tempo
uint32_t timer_callback(uint64_t tempo_final);

// Funções de exibição

// Funções de loop
