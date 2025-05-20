#pragma once

#include "Sistema.h"

void inicializarSerial()
{
    uart_init(UART_SERIAL, BAUD_RATE_SERIAL);
    gpio_set_function(PINO_TX_SERIAL, GPIO_FUNC_UART);
    gpio_set_function(PINO_RX_SERIAL, GPIO_FUNC_UART);
    uart_set_format(UART_SERIAL, 8, 1, UART_PARITY_NONE);
    uart_set_fifo_enabled(UART_SERIAL, true);
}