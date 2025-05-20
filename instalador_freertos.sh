#!/bin/bash

FREERTOS_DIR="freertos"
FREERTOS_REPO="https://github.com/FreeRTOS/FreeRTOS-Kernel.git"

function echo_info() {
    echo -e "\033[1;32m$1\033[0m"
}

function echo_error() {
    echo -e "\033[1;31m$1\033[0m"
}

if ! command -v git &> /dev/null; then
    echo_error "Erro: Git não está instalado. Instale o Git e tente novamente."
    exit 1
fi

if [ -d "$FREERTOS_DIR" ]; then
    echo_info "A pasta '$FREERTOS_DIR' já existe. Removendo..."
    rm -rf "$FREERTOS_DIR"
    if [ $? -ne 0 ]; then
        echo_error "Erro ao remover a pasta '$FREERTOS_DIR'."
        exit 1
    fi
    echo_info "Pasta '$FREERTOS_DIR' removida com sucesso."
fi

echo_info "Clonando o repositório do FreeRTOS..."
git clone "$FREERTOS_REPO" "$FREERTOS_DIR"
if [ $? -ne 0 ]; then
    echo_error "Erro ao clonar o repositório do FreeRTOS."
    exit 1
fi
echo_info "Repositório do FreeRTOS clonado com sucesso."
echo_info "Instalação do FreeRTOS concluída com sucesso!"