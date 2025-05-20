#pragma once

#include "pico/stdlib.h"
#include "hardware/uart.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "Oled.h"
#include "PWM.h"
#include "SerialBluetooth.h"
#include <stdio.h>
#include <cstring>

// pinos tela
#define PINO_SDA_TELA 14
#define PINO_SCL_TELA 15

// pinos serial
#define PINO_TX_SERIAL 0
#define PINO_RX_SERIAL 1
#define UART_SERIAL uart0
#define BAUD_RATE_SERIAL 115200

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

#define TEMPO_5_MIN 300000
#define TEMPO_10_MIN 600000
#define TEMPO_30_MIN 1800000
#define TEMPO_60_MIN 3600000

struct Sistema
{
    PWM &buzzer;
    volatile absolute_time_t inicioAtivacao;
    volatile bool ativo;
};

QueueHandle_t fila_comandos;

BluetoothSerial bluetooth("PicoRide");

bool dispostivoBluetoothConectado = false;
bool ultimo_dispostivoBluetoothConectado = false;

absolute_time_t pegarTempoAbsolutoAtual();
void inicializarContadorDeTempoDoBuzzer(Sistema &sistema, repeating_timer &timer);
bool desabilitarBuzzer(struct repeating_timer *t);
void acionarBuzzer(Sistema &sistema);