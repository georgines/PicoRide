#include "SerialBluetooth.h"
#include "btstack.h"
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdarg>

BluetoothSerial *BluetoothSerial::instancia = nullptr;

static btstack_packet_callback_registration_t registroCallbackHci;
static btstack_context_callback_registration_t callback_registration;

void BluetoothSerial::inicializarBluetoothStack()
{
    registroCallbackHci.callback = BluetoothSerial::manipuladorPacote;
    hci_add_event_handler(&registroCallbackHci);
    l2cap_init();
    rfcomm_init();
    rfcomm_register_service(BluetoothSerial::manipuladorPacote, CANAL_RFCOMM, 0xffff);
    sdp_init();
}

void BluetoothSerial::configurarServicoSpp(BluetoothSerial *instancia, const char *nomeDispositivo)
{
    std::memset(instancia->registroSpp, 0, sizeof(instancia->registroSpp));
    spp_create_sdp_record(instancia->registroSpp, 0x10001, CANAL_RFCOMM, nomeDispositivo);
    sdp_register_service(instancia->registroSpp);
}

void BluetoothSerial::configurarGap(const char *nomeDispositivo)
{
    gap_set_local_name(nomeDispositivo);
    gap_discoverable_control(1);
    gap_ssp_set_io_capability(SSP_IO_CAPABILITY_DISPLAY_YES_NO);
}

BluetoothSerial::BluetoothSerial(const char *nomeDispositivo)
{
    instancia = this;
    stdio_init_all();
    if (cyw43_arch_init())
    {
        std::printf("cyw43_arch_init falhou\n");
        return;
    }
    this->inicializarBluetoothStack();
    this->configurarServicoSpp(this, nomeDispositivo);
    this->configurarGap(nomeDispositivo);
    hci_power_control(HCI_POWER_ON);
}

BluetoothSerial::~BluetoothSerial()
{
    instancia = nullptr;
}

void BluetoothSerial::enviar(const uint8_t *dados, uint16_t tamanho)
{
    if (!this->canalRfcomm || tamanho > sizeof(this->bufferEnvio))
        return;
    std::memcpy(this->bufferEnvio, dados, tamanho);
    this->tamanhoEnvio = tamanho;
    rfcomm_request_can_send_now_event(this->canalRfcomm);
}

uint16_t BluetoothSerial::receber(uint8_t *buffer, uint16_t tamanhoMaximo)
{
    if (!this->tamanhoRecebido || !buffer)
        return 0;
    uint16_t n = (this->tamanhoRecebido < tamanhoMaximo) ? this->tamanhoRecebido : tamanhoMaximo;
    std::memcpy(buffer, this->bufferRecebimento, n);
    this->tamanhoRecebido = 0;
    return n;
}

void BluetoothSerial::definirCallbackRecebimento(CallbackRecebimentoBluetooth cb)
{
    this->callbackRecebimento = std::move(cb);
}

void BluetoothSerial::definirCallbackConexao(CallbackConexaoBluetooth cb) {
    this->callbackConexao = std::move(cb);
}

void BluetoothSerial::definirCallbackDesconexao(CallbackConexaoBluetooth cb) {
    this->callbackDesconexao = std::move(cb);
}

void BluetoothSerial::executar()
{
    btstack_run_loop_execute();
}

void BluetoothSerial::manipuladorPacote(uint8_t tipoPacote,
                                        uint16_t canal,
                                        uint8_t *pacote,
                                        uint16_t tamanho)
{
    if (BluetoothSerial::instancia)
    {
        BluetoothSerial::instancia->tratarPacote(tipoPacote, canal, pacote, tamanho);
    }
}

void BluetoothSerial::tratarPacote(uint8_t tipoPacote,
                                   uint16_t canal,
                                   uint8_t *pacote,
                                   uint16_t tamanho)
{
    UNUSED(canal);
    if (tipoPacote == HCI_EVENT_PACKET)
    {
        this->tratarEventoHci(pacote);
    }
    else if (tipoPacote == RFCOMM_DATA_PACKET)
    {
        this->tratarPacoteRfcomm(pacote, tamanho);
    }
}

void BluetoothSerial::tratarEventoHci(uint8_t *pacote)
{
    bd_addr_t endereco;
    auto tipoEvento = hci_event_packet_get_type(pacote);
    if (tipoEvento == HCI_EVENT_PIN_CODE_REQUEST)
    {
        hci_event_pin_code_request_get_bd_addr(pacote, endereco);
        gap_pin_code_response(endereco, "0000");
    }
    else if (tipoEvento == RFCOMM_EVENT_INCOMING_CONNECTION)
    {
        auto cid = rfcomm_event_incoming_connection_get_rfcomm_cid(pacote);
        rfcomm_accept_connection(cid);
    }
    else if (tipoEvento == RFCOMM_EVENT_CHANNEL_OPENED)
    {
        if (!rfcomm_event_channel_opened_get_status(pacote))
        {
            this->canalRfcomm = rfcomm_event_channel_opened_get_rfcomm_cid(pacote);
            if (this->callbackConexao)
            {
                this->callbackConexao();
            }
        }
    }
    else if (tipoEvento == RFCOMM_EVENT_CAN_SEND_NOW)
    {
        this->enviarBufferSePronto();
    }
    else if (tipoEvento == RFCOMM_EVENT_CHANNEL_CLOSED)
    {
        this->canalRfcomm = 0;
        if (this->callbackDesconexao)
        {
            this->callbackDesconexao();
        }
    }
}

void BluetoothSerial::tratarPacoteRfcomm(uint8_t *pacote, uint16_t tamanho)
{
    if (tamanho > sizeof(this->bufferRecebimento))
        return;
    this->tamanhoRecebido = tamanho;
    std::memcpy(this->bufferRecebimento, pacote, tamanho);
    if (this->callbackRecebimento)
        this->callbackRecebimento(this->bufferRecebimento, tamanho);
}

void BluetoothSerial::enviarBufferSePronto()
{
    if (this->tamanhoEnvio)
    {
        rfcomm_send(this->canalRfcomm, this->bufferEnvio, this->tamanhoEnvio);
        this->tamanhoEnvio = 0;
    }
}

void BluetoothSerial::enviarString(const char *texto)
{
    if (!texto)
        return;
    this->enviar(reinterpret_cast<const uint8_t *>(texto), strlen(texto));
}

void BluetoothSerial::enviarStringFormatada(const char *formato, ...)
{
    char buffer[256];
    va_list args;
    va_start(args, formato);
    int len = vsnprintf(buffer, sizeof(buffer), formato, args);
    va_end(args);
    if (len <= 0 || (size_t)len > sizeof(buffer))
        return;
    this->enviar(reinterpret_cast<const uint8_t *>(buffer), len);
}

void BluetoothSerial::iniciarLoopTimer(std::function<void()> callbackExterno, uint32_t intervalo_ms) {
    this->callbackExternoLoop = std::move(callbackExterno);

    btstack_run_loop_set_timer_handler(&timerLoop, [](btstack_timer_source_t *ts) {
        BluetoothSerial* instancia = BluetoothSerial::instancia;
        if (instancia && instancia->callbackExternoLoop) {
            instancia->callbackExternoLoop();
        }
        btstack_run_loop_set_timer(ts, instancia->intervaloLoop);
        btstack_run_loop_add_timer(ts);
    });

    this->intervaloLoop = intervalo_ms;
    btstack_run_loop_set_timer(&timerLoop, intervalo_ms);
    btstack_run_loop_add_timer(&timerLoop);
}
