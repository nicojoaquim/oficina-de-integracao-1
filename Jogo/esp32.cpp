#include "esp32.h"

// Função usada pelo libcurl para receber os dados HTTP
static size_t escreverResposta(void* conteudo, size_t tamanho,
                               size_t quantidade, void* usuario)
{
    size_t total = tamanho * quantidade;

    std::string* resposta = static_cast<std::string*>(usuario);
    resposta->append(static_cast<char*>(conteudo), total);

    return total;
}

// Recebe os valores dos sensores do ESP32-C3 pela página /sensores.
// Nesta versão, os eixos usados no jogo são AY, AZ e GX.
bool receberSensores(float& acelY, float& acelZ, float& giroX)
{
    CURL* curl = curl_easy_init();

    if (!curl)
        return false;

    std::string resposta;

    std::string url = std::string("http://") + IP + "/sensores";

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

    // Não usar proxy
    curl_easy_setopt(curl, CURLOPT_PROXY, "");

    // Receber a resposta
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, escreverResposta);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &resposta);

    // Timeout para não travar o jogo caso o ESP32 não responda
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT_MS, 500L);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, 1000L);

    // User-Agent
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "MPU6050");

    CURLcode resultado = curl_easy_perform(curl);

    bool sucesso = false;

    if (resultado == CURLE_OK)
    {
        float acelX, giroY, giroZ;

        // A página do ESP32 continua enviando os seis valores.
        // Aqui extraímos AY, AZ e GX para os controles do jogo.
        if (sscanf(
                resposta.c_str(),
                "AX:%f AY:%f AZ:%f GX:%f GY:%f GZ:%f",
                &acelX,
                &acelY,
                &acelZ,
                &giroX,
                &giroY,
                &giroZ
            ) == 6)
        {
            sucesso = true;
        }
    }

    curl_easy_cleanup(curl);

    return sucesso;
}
