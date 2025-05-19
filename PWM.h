#pragma once
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"

class PWM {
public:
    PWM(uint8_t pino, uint16_t wrap, float divisor_clock);
    void iniciar(uint16_t duty_inicial);
    void habilitar();
    void desabilitar();
    void habilitarInterrupcao();
    void modificarDuty(uint16_t duty);
    static void modificarDuty(uint8_t pino, uint16_t wrap, uint16_t duty);
    static void limparInterrupcao(uint16_t slice);
    static uint16_t pegarSlice(uint8_t pino);
    static uint16_t mapearDuty(uint16_t x, uint16_t in_min, uint16_t in_max, uint16_t out_min, uint16_t out_max);

private:
    uint16_t slice;
    uint8_t pino;
    uint16_t wrap;
    float divisor_clock;
};
