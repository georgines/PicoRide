#include "Sistema.h"
#include "auxiliarBuzzer.h"
#include "loopPrincipal.h"
#include "loopBluetooth.h"
#include "loopDisplay.h"

int main()
{
    stdio_init_all();

 

    PWM buzzer(PINO_BUZZER, WRAP_BUZZER, DIVISOR_CLOCK_BUZZER);
    buzzer.iniciar(DUTY_INICIAL_BUZZER);

    Sistema sistema = {       
        .buzzer = buzzer,
        .inicioAtivacao = pegarTempoAbsolutoAtual(),
        .ativo = false};

    fila_comandos = xQueueCreate(1, sizeof(char));

    mutex_tempo = xSemaphoreCreateMutex();

    repeating_timer timer;
    inicializarContadorDeTempoDoBuzzer(sistema, timer);

    repeating_timer_t tempo;
    iniciarContagemTempoDoCronometro(tempo);

    xTaskCreate(loopPrincipal, "loopPricipal", 8096, (void *)&sistema, 2, NULL);
    xTaskCreate(loopBluetooth, "loopBluetooth", 8096, (void *)&sistema, 2, NULL);
    // xTaskCreate(loopDisplay, "loopDisplay", 8096, (void *)&sistema, 2, NULL);

    vTaskStartScheduler();
    return 1;
}
