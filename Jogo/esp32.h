#pragma once

#include <curl/curl.h>
#include <string>
#include <cstdio>

#define IP "192.168.4.1"

static size_t escreverResposta(void* conteudo, size_t tamanho,
                               size_t quantidade, void* usuario);

bool receberSensores(float& acelY, float& acelZ, float& giroX);
