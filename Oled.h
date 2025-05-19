#pragma once

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/i2c.h"
#include "fonte.h"

#define SSD1306_ALTURA 64 // Define a altura do display (32 pixels)
#define SSD1306_LARGURA 128 // Define a largura do display (128 pixels)

#define SSD1306_ENDERECO_I2C _u(0x3C) // Define o endereço do i2c do display
#define SSD1306_CLOCK_I2C 400 // Define o tempo do clock (pode ser aumentado)

// Comandos de configuração (endereços)
#define SSD1306_MODO_MEMORIA _u(0x20)
#define SSD1306_ENDERECO_COLUNA _u(0x21)
#define SSD1306_ENDERECO_PAGINA _u(0x22)
#define SSD1306_SCROLL_HORIZONTAL _u(0x26)
#define SSD1306_SCROLL _u(0x2E)
#define SSD1306_LINHA_INICIAL_DISPLAY _u(0x40)
#define SSD1306_CONTRASTE _u(0x81)
#define SSD1306_BOMBA_CARGA _u(0x8D)
#define SSD1306_SEGMENTO_REMAP _u(0xA0)
#define SSD1306_DISPLAY_ENTIRE_ON _u(0xA4)
#define SSD1306_DISPLAY_ALL_ON _u(0xA5)
#define SSD1306_DISPLAY_NORMAL _u(0xA6)
#define SSD1306_DISPLAY_INVERSO _u(0xA7)
#define SSD1306_TAXA_MUX _u(0xA8)
#define SSD1306_DISPLAY_ON_OFF _u(0xAE)
#define SSD1306_SAIDA_COMUM _u(0xC0)
#define SSD1306_OFFSET_DISPLAY _u(0xD3)
#define SSD1306_DIVISAO_CLOCK _u(0xD5)
#define SSD1306_PRECHARGE _u(0xD9)
#define SSD1306_CONFIG_PINOS_COMUNS _u(0xDA)
#define SSD1306_NIVEL_VCOMH _u(0xDB)

#define SSD1306_ALTURA_PAGINA _u(8)
#define SSD1306_N_PAGINAS (SSD1306_ALTURA / SSD1306_ALTURA_PAGINA)
#define SSD1306_TAMANHO_BUFFER (SSD1306_N_PAGINAS * SSD1306_LARGURA)

class Oled {
  private:
    uint8_t colunaInicio;
    uint8_t colunaFim;
    uint8_t paginaInicio;
    uint8_t paginaFim;
    int tamanhoBuffer;
    uint16_t *dadosFonte = nullptr;
    uint8_t bufferDisplay[SSD1306_TAMANHO_BUFFER];
  public:
    Oled(i2c_inst_t *i2c, uint8_t pinoSDA, uint8_t pinoSCL);
    void inicializar();
    void limpar();
    void limparAreaLinha(int x, int y);
    void limparLinha(int linha);
    void limparAreaTexto(int16_t x, int16_t y, const char *texto);
    void definirFonte(uint16_t *fonte);
    void renderizarDisplay();
    void imprimir(int16_t x, int16_t y, const char *formato, ...);
    void desenharTexto(int16_t x, int16_t y, const char *texto);

    

  private:
    void enviarComando(uint8_t comando);
    void enviarListaComandos(uint8_t *comandos, int quantidade);
    void calcularTamanhoBufferRenderizacao();
    void enviarBuffer(uint8_t *dados, int tamanho);
    void desenharCaractere(uint8_t *buffer, int16_t x, int16_t y, char caractere);
};
