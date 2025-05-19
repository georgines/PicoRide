#include "PWM.h"

PWM::PWM(uint8_t pino, uint16_t wrap, float divisor_clock)
    : pino(pino), wrap(wrap), divisor_clock(divisor_clock), slice(0) {}

void PWM::iniciar(uint16_t duty_inicial) {
    gpio_set_function(this->pino, GPIO_FUNC_PWM);
    this->slice = pwm_gpio_to_slice_num(this->pino);
    pwm_set_clkdiv(this->slice, this->divisor_clock);
    pwm_set_wrap(this->slice, this->wrap);
    this->modificarDuty(duty_inicial);
    this->habilitar();
}

void PWM::habilitar() {
    pwm_set_enabled(this->slice, true);
}

void PWM::desabilitar() {
    pwm_set_enabled(this->slice, false);
}

void PWM::habilitarInterrupcao() {
    pwm_set_irq_enabled(this->slice, true);
}

void PWM::modificarDuty(uint16_t duty) {
    uint16_t duty_convertido = PWM::mapearDuty(duty, 0, 100, 0, this->wrap);
    pwm_set_gpio_level(this->pino, duty_convertido);
}

void PWM::modificarDuty(uint8_t pino, uint16_t wrap, uint16_t duty) {
    uint16_t duty_convertido = PWM::mapearDuty(duty, 0, 100, 0, wrap);
    pwm_set_gpio_level(pino, duty_convertido);
}

void PWM::limparInterrupcao(uint16_t slice) {
    pwm_clear_irq(slice);
}

uint16_t PWM::pegarSlice(uint8_t pino) {
    return pwm_gpio_to_slice_num(pino);
}

uint16_t PWM::mapearDuty(uint16_t x, uint16_t in_min, uint16_t in_max, uint16_t out_min, uint16_t out_max) {
    if (in_max == in_min) {
        return 0;
    }
    uint16_t result = (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
    if (result > out_max) {
        return out_max;
    }
    return result;
}
