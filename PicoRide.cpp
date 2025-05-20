#include "Sistema.h"
#include "auxiliarBuzzer.h"
#include "auxiliarSerial.h"
#include "loopPrincipal.h"
#include "loopBluetooth.h"
#include "loopContadorTempo.h"
#include "loopExibicao.h"

int main()
{
    stdio_init_all();
    inicializarSerial();

    printf("Iniciando...\n");

    PWM buzzer(PINO_BUZZER, WRAP_BUZZER, DIVISOR_CLOCK_BUZZER);
    buzzer.iniciar(DUTY_INICIAL_BUZZER);

    Sistema sistema = {
        .buzzer = buzzer,
        .inicioAtivacao = pegarTempoAbsolutoAtual(),
        .ativo = false};

    repeating_timer timer;

    inicializarContadorDeTempoDoBuzzer(sistema, timer);
    inicializarLoopPrincipal(sistema);
    inicializarLoopBluetooth();
    iniciarContadorDeTempo();
    inicializarLoopExibicao();
    vTaskStartScheduler();
    
    return 1;
}
