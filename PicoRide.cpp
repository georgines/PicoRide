#include "Sistema.h"
#include "auxiliarBuzzer.h"

int main()
{
    stdio_init_all();

    Oled tela(i2c1, PINO_SDA_TELA, PINO_SCL_TELA);
    tela.definirFonte(fonte_customizada);
    tela.inicializar();
    tela.limpar();

    PWM buzzer(PINO_BUZZER, WRAP_BUZZER, DIVISOR_CLOCK_BUZZER);
    buzzer.iniciar(DUTY_INICIAL_BUZZER);

    Sistema sistema = {
        .tela = tela,
        .buzzer = buzzer,
        .inicioAtivacao = pegarTempoAbsolutoAtual(),
        .ativo = false};

        
}
