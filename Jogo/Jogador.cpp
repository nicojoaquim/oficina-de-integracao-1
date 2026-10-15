#include "Jogador.h"
#include <fstream>
#include <windows.h>
#include <winhttp.h>
#include <cstdio>
#include <string>
#include <atomic>
#include <thread>
#include <mutex>
#include <chrono>

#pragma comment(lib, "winhttp.lib")

// Valores compartilhados com a thread de leitura. A rede nunca bloqueia a thread do jogo.
static std::atomic<float> sensorAY(0.0f);
static std::atomic<float> sensorAZ(0.0f);
static std::atomic<int> sensorFlex1(-1);
static std::atomic<bool> sensoresDisponiveis(false);

static bool receberSensoresJogador(float& acelY, float& acelZ, int& flex1)
{
    HINTERNET sessao = WinHttpOpen(L"Jogo Jogador MPU6050",
        WINHTTP_ACCESS_TYPE_NO_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);
    if (!sessao) return false;

    // Limita esperas caso o ESP32 esteja desligado/desconectado.
    WinHttpSetTimeouts(sessao, 100, 100, 150, 150);

    HINTERNET conexao = WinHttpConnect(sessao, L"192.168.4.1", 80, 0);
    if (!conexao) {
        WinHttpCloseHandle(sessao);
        return false;
    }

    HINTERNET requisicao = WinHttpOpenRequest(conexao, L"GET", L"/sensores",
        NULL, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, 0);
    if (!requisicao) {
        WinHttpCloseHandle(conexao);
        WinHttpCloseHandle(sessao);
        return false;
    }

    bool sucesso = false;
    if (WinHttpSendRequest(requisicao, WINHTTP_NO_ADDITIONAL_HEADERS, 0,
        WINHTTP_NO_REQUEST_DATA, 0, 0, 0) &&
        WinHttpReceiveResponse(requisicao, NULL)) {
        char buffer[512];
        DWORD bytesLidos = 0;
        std::string resposta;
        do {
            bytesLidos = 0;
            if (!WinHttpReadData(requisicao, buffer, sizeof(buffer) - 1, &bytesLidos)) break;
            if (bytesLidos == 0) break;
            buffer[bytesLidos] = '\0';
            resposta += buffer;
        } while (bytesLidos > 0 && resposta.size() < 2048);

        float ax, ay, az, gx, gy, gz;
        int f1, f2, f3;
        if (sscanf_s(resposta.c_str(),
            "AX:%f AY:%f AZ:%f GX:%f GY:%f GZ:%f FLEX1:%d FLEX2:%d FLEX3:%d",
            &ax, &ay, &az, &gx, &gy, &gz, &f1, &f2, &f3) == 9) {
            acelY = ay;
            acelZ = az;
            flex1 = f1;
            sucesso = true;
        }
    }

    WinHttpCloseHandle(requisicao);
    WinHttpCloseHandle(conexao);
    WinHttpCloseHandle(sessao);
    return sucesso;
}

// Executa a comunicação Wi-Fi fora da thread gráfica do SFML.
static void iniciarLeituraSensoresEmSegundoPlano()
{
    static std::once_flag inicio;
    std::call_once(inicio, []() {
        std::thread([]() {
            while (true) {
                float ay = 0.0f, az = 0.0f;
                int flex1 = -1;
                if (receberSensoresJogador(ay, az, flex1)) {
                    sensorAY.store(ay);
                    sensorAZ.store(az);
                    sensorFlex1.store(flex1);
                    sensoresDisponiveis.store(true);
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(40));
            }
            }).detach();
        });
}

using namespace Personagens;

Jogador::Jogador() : Personagem(), pontos(0), forca_andar(10), max_vidas(30), invulneravel(false), tempoInvencivel(0.2f), salvo(false), tecla_pressionada(false) {
    num_vidas = max_vidas;
    sizex = 45;
    sizey = 18;
    atribuirBotoes();

    sf::RectangleShape* shape = new sf::RectangleShape(sf::Vector2f(sizex, sizey));
    shape->setFillColor(sf::Color::Green);

    pHitbox = shape;
    criarSprite();
}

Jogador::~Jogador() {}

void Jogador::atribuirBotoes() {
    botao_cima = sf::Keyboard::Key::W;
    botao_esquerda = sf::Keyboard::Key::A;
    botao_baixo = sf::Keyboard::Key::S;
    botao_direita = sf::Keyboard::Key::D;
    botao_atirar = sf::Keyboard::Key::Space;
}

void Jogador::executar() {

    if (relogioDano.getElapsedTime().asSeconds() > tempoInvencivel && invulneravel) {
        invulneravel = false;
        setTexture("jogador1.png");
        pSprite->setTexture(*pTexture);
    }

    if (sf::Keyboard::isKeyPressed(botao_direita) && !sf::Keyboard::isKeyPressed(botao_esquerda)) {
        x += forca_andar;
    }
    else if (sf::Keyboard::isKeyPressed(botao_esquerda) && !sf::Keyboard::isKeyPressed(botao_direita)) {
        x -= forca_andar;
    }

    if (sf::Keyboard::isKeyPressed(botao_cima) && !sf::Keyboard::isKeyPressed(botao_baixo)) {
        y += forca_andar;
    }
    else if (sf::Keyboard::isKeyPressed(botao_baixo) && !sf::Keyboard::isKeyPressed(botao_cima)) {
        y -= forca_andar;
    }

    // Inicia a leitura Wi-Fi em segundo plano: a chamada HTTP nao trava o jogo.
    iniciarLeituraSensoresEmSegundoPlano();

    if (sensoresDisponiveis.load()) {
        const float acelY = sensorAY.load();
        const float acelZ = sensorAZ.load();

  
        if (acelY > 3.0f) {
            x += forca_andar;
        }
        else if (acelY < -3.0f) {
            x -= forca_andar;
        }

        if (acelZ < -3.0f) {
            y += forca_andar;
        }
        else if (acelZ > 3.0f) {
            y -= forca_andar;

        }
    }

    if (sf::Keyboard::isKeyPressed(botao_atirar)) {
        if (tecla_pressionada == false) {
            atirar = true;
            tecla_pressionada = true;
        }
        else {
            atirar = false;
        }
    }
    else {
        atirar = false;
        tecla_pressionada = false;
    }

    // Disparo pelo FLEX1: dispara uma vez ao dobrar e rearma ao estender.
    // Limites baseados nas leituras informadas: estendido 1350-1450, dobrado ~1500.
    static bool flexPressionado = false;
    const int flex1 = sensorFlex1.load();
    if (flex1 >= 2600 && !flexPressionado) {
        atirar = true;
        flexPressionado = true;
    }
    else if (flex1 < 2600) {
        flexPressionado = false;
    }

    gravitar();
    //mover(velx, vely);
    //forca_andar = 5;

    pHitbox->setPosition(x, y);
}
void Jogador::salvar() {
    if (!salvo) {
        salvo = true;

        buffer << getTipo() << " "
            << pontos << " "
            //<< relogioDano << " "
            << tempoInvencivel << " "
            << invulneravel << " "
            << max_vidas << " "
            << forca_andar << " "
            << tecla_pressionada << " ";

        Personagem::salvarDataBuffer();
    }
}
void Jogador::carregar(std::ifstream& arquivo) {
    arquivo >> pontos >> /*relogioDano >>*/ tempoInvencivel >> invulneravel >> max_vidas >> forca_andar >> tecla_pressionada;
    Personagem::carregar(arquivo);
    pHitbox->setPosition(x, y);
}

void Jogador::criar() {
}

void Jogador::mover(float velx, float vely) {
    x += velx;
    y += vely;
    pHitbox->move(velx, vely);
}

void Jogador::operator--(int dano) {
    if (!invulneravel) {
        if (num_vidas > 0) {
            Personagem::operator--(dano);
            invulneravel = true;
            setTexture("jogador1dano.png");
            pSprite->setTexture(*pTexture);
        }
        relogioDano.restart();
    }
    else {
        std::cout << "Dano ignorado! Jogador invencivel por mais: " << (tempoInvencivel - relogioDano.getElapsedTime().asSeconds()) << "s" << std::endl;
    }
}
void Jogador::moverParaSpawn() {
    setPosition(100, 200);
}

void Jogador::operator++() {
    pontos++;
}

const int Jogador::getPontos() {
    return pontos;
}
void Jogador::setPontos(int p) { pontos = p; }
void Jogador::reset() {
    vely = 0;
    num_vidas = max_vidas;
    pontos = 0;
}

void Jogador::criarSprite() {
    setTexture("jogador.png");

    //pSprite->setTextureRect(sf::IntRect({ 0, 0 }, { 16, 16 }));

    sf::FloatRect limitesSprite = pSprite->getLocalBounds();
    pSprite->setOrigin(limitesSprite.width / 2.0f, limitesSprite.height / 2.0f);
    pSprite->setScale(-1, 1);

    pSprite->setRotation(180.0f);
}
void Jogador::atualizarSprite() {
    pSprite->setPosition(x + 0.6 * sizex, y + sizey);

    if (direcao) {
        //pSprite->setOrigin(12.5f, 16.f);
        //pSprite->setScale(3, 4);
    }
    else {
        //pSprite->setOrigin(4.f, 16.f);
        //pSprite->setScale(-3, 4);
    }
}

std::string Jogador::getTipo() {
    return "Jogador";
}

