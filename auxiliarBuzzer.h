#pragma once
#include "Sistema.h"
#include "hardware/timer.h"
#include "hardware/irq.h"

absolute_time_t pegarTempoAbsolutoAtual(){
   return get_absolute_time();
}

void inicializarContadorDeTempoDoBuzzer( Sistema &sistema, repeating_timer &timer) {   
    add_repeating_timer_ms(10, desabilitarBuzzer, &sistema, &timer);   
}

bool desabilitarBuzzer(struct repeating_timer *t)
{
    Sistema *sistema = reinterpret_cast<Sistema *>(t->user_data);
    if (sistema->ativo && absolute_time_diff_us(sistema->inicioAtivacao, get_absolute_time()) >= TEMPO_BUZZER_MS * 1000)
    {        
        sistema->buzzer.modificarDuty(DUTY_DESLIGAR_BUZZER);
        sistema->ativo = false;
    }
    return true;
}

void acionarBuzzer(Sistema &sistema)
{
    if (!sistema.ativo)
    {
        sistema.buzzer.modificarDuty(DUTY_BUZZER);
        sistema.inicioAtivacao = get_absolute_time();
        sistema.ativo = true;
    }
}