#include "Oled.h"

Oled::Oled(i2c_inst_t *i2c, uint8_t pinoSDA, uint8_t pinoSCL) {
  i2c_init(i2c1, SSD1306_CLOCK_I2C * 1000);
  gpio_set_function(pinoSDA, GPIO_FUNC_I2C);
  gpio_set_function(pinoSCL, GPIO_FUNC_I2C);
  gpio_pull_up(pinoSDA);
  gpio_pull_up(pinoSCL);

  this->colunaInicio = 0;
  this->colunaFim = SSD1306_LARGURA - 1;
  this->paginaInicio = 0;
  this->paginaFim = SSD1306_N_PAGINAS - 1;
}

void Oled::inicializar() {
  uint8_t comandos[] = {
    SSD1306_DISPLAY_ON_OFF,
    SSD1306_MODO_MEMORIA,
    0x00,
    SSD1306_LINHA_INICIAL_DISPLAY,
    SSD1306_SEGMENTO_REMAP | 0x01,
    SSD1306_TAXA_MUX,
    SSD1306_ALTURA - 1,
    SSD1306_SAIDA_COMUM | 0x08,
    SSD1306_OFFSET_DISPLAY,
    0x00,
    SSD1306_CONFIG_PINOS_COMUNS,
#if ((SSD1306_LARGURA == 128) && (SSD1306_ALTURA == 32))
    0x02,
#elif ((SSD1306_LARGURA == 128) && (SSD1306_ALTURA == 64))
    0x12,
#else
    0x02,
#endif
    SSD1306_DIVISAO_CLOCK,
    0x80,
    SSD1306_PRECHARGE,
    0xF1,
    SSD1306_NIVEL_VCOMH,
    0x30,
    SSD1306_CONTRASTE,
    0xFF,
    SSD1306_DISPLAY_ENTIRE_ON,
    SSD1306_DISPLAY_NORMAL,
    SSD1306_BOMBA_CARGA,
    0x14,
    SSD1306_SCROLL | 0x00,
    SSD1306_DISPLAY_ON_OFF | 0x01,
  };

  this->enviarListaComandos(comandos, sizeof(comandos) / sizeof(comandos[0]));
  this->calcularTamanhoBufferRenderizacao();
}

void Oled::limpar() {
  memset(this->bufferDisplay, 0, SSD1306_TAMANHO_BUFFER);
  this->renderizarDisplay();
}

void Oled::limparAreaLinha(int x, int y) {
  uint8_t larguraCaractere = 8;
  int num_espacos = (SSD1306_LARGURA - x) / larguraCaractere;
  char buffer[num_espacos + 1];
  memset(buffer, ' ', num_espacos);
  buffer[num_espacos] = '\0';
  this->imprimir(x, y, buffer);
  //this->renderizarDisplay();
}

void Oled::limparLinha(int linha) {
  this->limparAreaLinha(0, linha);
}

void Oled::limparAreaTexto(int16_t x, int16_t y, const char *texto) {
  uint8_t larguraCaractere = 8;
  uint8_t alturaCaractere = 8;

  int larguraTexto = strlen(texto) * larguraCaractere;
  int alturaTexto = alturaCaractere;

  for (int linha = 0; linha < alturaTexto; linha++) {
    for (int coluna = 0; coluna < larguraTexto; coluna++) {
      int indiceBuffer = ((y + linha) / 8) * SSD1306_LARGURA + (x + coluna);
      if (indiceBuffer >= 0 && indiceBuffer < SSD1306_TAMANHO_BUFFER) {
        this->bufferDisplay[indiceBuffer] = 0;
      }
    }
  }
}

void Oled::definirFonte(uint16_t *fonte) {
  this->dadosFonte = fonte;
}

void Oled::renderizarDisplay() {
  uint8_t comandos[] = {
    SSD1306_ENDERECO_COLUNA, this->colunaInicio, this->colunaFim,
    SSD1306_ENDERECO_PAGINA, this->paginaInicio, this->paginaFim
  };

  this->enviarListaComandos(comandos, sizeof(comandos) / sizeof(comandos[0]));
  this->enviarBuffer(this->bufferDisplay, this->tamanhoBuffer);
}

void Oled::imprimir(int16_t x, int16_t y, const char *formato, ...) {
  char buffer[128];

  va_list args;
  va_start(args, formato);
  vsnprintf(buffer, sizeof(buffer), formato, args);
  va_end(args);

  this->limparAreaTexto(x, y, buffer);
  this->desenharTexto(x, y, buffer);
}

void Oled::desenharTexto(int16_t x, int16_t y, const char *texto) {
  uint8_t larguraCaractere = 8;

  while (*texto) {
    this->desenharCaractere(this->bufferDisplay, x, y, *texto++);
    x += larguraCaractere;

    if (x > SSD1306_LARGURA - larguraCaractere) {
      x = 0;
      y += 8;
    }

    if (y >= SSD1306_ALTURA) {
      break;
    }
  }
}

void Oled::enviarComando(uint8_t comando) {
  uint8_t buffer[2] = {0x80, comando};
  i2c_write_blocking(i2c1, SSD1306_ENDERECO_I2C, buffer, 2, false);
}

void Oled::enviarListaComandos(uint8_t *comandos, int quantidade) {
  for (int i = 0; i < quantidade; i++) {
    this->enviarComando(comandos[i]);
  }
}

void Oled::calcularTamanhoBufferRenderizacao() {
  this->tamanhoBuffer = (this->colunaFim - this->colunaInicio + 1) *
                        (this->paginaFim - this->paginaInicio + 1);
}

void Oled::enviarBuffer(uint8_t *dados, int tamanho) {
  uint8_t *bufferTemp = reinterpret_cast<uint8_t *>(malloc(tamanho + 1));

  if (bufferTemp == nullptr) {
    return;
  }

  bufferTemp[0] = 0x40;
  memcpy(bufferTemp + 1, dados, tamanho);

  i2c_write_blocking(i2c1, SSD1306_ENDERECO_I2C, bufferTemp, tamanho + 1, false);

  free(bufferTemp);
}

void Oled::desenharCaractere(uint8_t *buffer, int16_t x, int16_t y, char caractere) {
  if (!this->dadosFonte) return;

  if (x >= SSD1306_LARGURA || y >= SSD1306_ALTURA || x < 0 || y < 0) {
    return;
  }

  uint8_t alturaCaractere = 8;
  uint8_t larguraCaractere = 8;
  uint16_t deslocamentoFonte = (caractere - ' ') * alturaCaractere;

  for (uint8_t linha = 0; linha < alturaCaractere; linha++) {
    uint16_t dadosLinha = this->dadosFonte[deslocamentoFonte + linha];

    for (uint8_t coluna = 0; coluna < larguraCaractere; coluna++) {
      if (dadosLinha & (0x8000 >> coluna)) {
        int indiceBuffer = ((y + linha) / 8) * SSD1306_LARGURA + (x + coluna);

        if (indiceBuffer >= 0 && indiceBuffer < SSD1306_TAMANHO_BUFFER) {
          buffer[indiceBuffer] |= (1 << ((y + linha) % 8));
        }
      }
    }
  }
}
