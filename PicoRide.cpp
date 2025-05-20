#include "Sistema.h"
#include "auxiliarBuzzer.h"
#include "loopPrincipal.h"
#include "loopBluetooth.h"
#include "loopContadorTempo.h"
#include "loopExibicao.h"


int main()
{
    stdio_init_all();
    uart_init(uart0, 115200);

    // Configurar GPIO 0 e 1 para UART0
    gpio_set_function(0, GPIO_FUNC_UART); // TX
    gpio_set_function(1, GPIO_FUNC_UART); // RX

    uart_set_format(uart0, 8, 1, UART_PARITY_NONE);

    // Habilitar FIFO da UART
    uart_set_fifo_enabled(uart0, true);

    printf("Iniciando...\n");

    PWM buzzer(PINO_BUZZER, WRAP_BUZZER, DIVISOR_CLOCK_BUZZER);
    buzzer.iniciar(DUTY_INICIAL_BUZZER);

    Sistema sistema = {
        .buzzer = buzzer,
        .inicioAtivacao = pegarTempoAbsolutoAtual(),
        .ativo = false};

   
   

    repeating_timer timer;
    inicializarContadorDeTempoDoBuzzer(sistema, timer);

    fila_comandos = xQueueCreate(8, sizeof(char));
    xTaskCreate(loopPrincipal, "loopPricipal", 8192, (void *)&sistema, 2, NULL);

    xTaskCreate(loopBluetooth, "loopBluetooth", 8192, NULL, 2, NULL);

    iniciarContadorDeTempo();
    inicializarLoopExibicao();
    vTaskStartScheduler();
    return 1;
}
