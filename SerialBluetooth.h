#pragma once

#include <cstdint>
#include <functional>
#include "btstack_run_loop.h"

using CallbackRecebimentoBluetooth = std::function<void(const uint8_t *dados, uint16_t tamanho)>;

class BluetoothSerial
{
public:
    explicit BluetoothSerial(const char *nomeDispositivo);
    ~BluetoothSerial();

    void enviar(const uint8_t *dados, uint16_t tamanho);
    uint16_t receber(uint8_t *buffer, uint16_t tamanhoMaximo);
    void definirCallbackRecebimento(CallbackRecebimentoBluetooth callbackRecebimento);
    void executar();
    void enviarString(const char *texto);
    void enviarStringFormatada(const char *formato, ...);
    void iniciarLoopTimer(std::function<void()> callbackExterno, uint32_t intervalo_ms);

private:
    void tratarEventoHci(uint8_t *pacote);
    void tratarPacoteRfcomm(uint8_t *pacote, uint16_t tamanho);
    void enviarBufferSePronto();
    static void inicializarBluetoothStack();
    static void configurarServicoSpp(BluetoothSerial *instancia, const char *nomeDispositivo);
    static void configurarGap(const char *nomeDispositivo);
    static void liberarBuffer(uint8_t *&buffer);
    void tratarPacote(uint8_t tipoPacote,
                      uint16_t canal,
                      uint8_t *pacote,
                      uint16_t tamanho);

    static void manipuladorPacote(uint8_t tipoPacote,
                                  uint16_t canal,
                                  uint8_t *pacote,
                                  uint16_t tamanho);

    static BluetoothSerial *instancia;

    static constexpr uint8_t CANAL_RFCOMM = 1;
    uint16_t canalRfcomm = 0;
    uint8_t registroSpp[150] = {};

    uint8_t *bufferRecebimento = nullptr;
    uint16_t tamanhoRecebido = 0;
    CallbackRecebimentoBluetooth callbackRecebimento = nullptr;
    uint32_t intervaloLoop = 1000;
    std::function<void()> callbackExternoLoop = nullptr;

    uint8_t *bufferEnvio = nullptr;
    uint16_t tamanhoEnvio = 0;

    btstack_timer_source_t timerLoop;
};
