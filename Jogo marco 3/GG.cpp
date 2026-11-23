
#include "GG.h"
#include <iostream>
#include "Ente.h"
#include "Entidades.h"
#include "Personagens.h"
#include "Jogador.h"
#include "Fases.h"

using namespace std;
using namespace Gerenciadores;

//GRAFICO
GerenciadorGrafico::GerenciadorGrafico() : obj(sf::VideoMode({ 1280, 720 }), "Jogo para Luva"), digitando(false), nome_jogador("") {
	sf::View view;
	view.setCenter(sf::Vector2f(640.f, 360.f));
	view.setSize(sf::Vector2f(1280.f, -720.f));
	obj.setView(view);

	obj.setFramerateLimit(60);
}
GerenciadorGrafico::~GerenciadorGrafico() { obj.close(); }
void GerenciadorGrafico::desenharEnte(Ente* pE) {
    if(pE==NULL) {
        return;
    }
	pE->atualizarSprite();

    if (Entidade* pEntidade = dynamic_cast<Entidade*>(pE)) {
        obj.draw(*pEntidade->getHitbox()); //Tirar comentário para desenhar hitboxes nas entidades
    }
	obj.draw(pE->getSprite());
}
void GerenciadorGrafico::desenharFundo(sf::Sprite fundo) {
    obj.draw(fundo);
}
void GerenciadorGrafico::desenharFundo(sf::Sprite fundo, sf::Sprite chao) {
    obj.draw(fundo);
    obj.draw(chao);
}
void GerenciadorGrafico::desenharVidas(Jogador* pJog1) {
    sf::Font font;
    if (!font.openFromFile("BAUHS93.TTF"))
        return;

    std::string textoVidas = "Vidas: " + std::to_string(pJog1->getVidas());

    sf::Text vidas(font, textoVidas, 45);
    vidas.setScale({ 1.f, -1.f });
    vidas.setPosition({ 1000.f, 720.f });

    std::string textoPontos = "Pontos: " + std::to_string(pJog1->getPontos());

    sf::Text pontos(font, textoPontos, 45);
    pontos.setScale({ 1.f, -1.f });
    pontos.setPosition({ 690.f, 720.f });

    obj.draw(vidas);
    obj.draw(pontos);
}
bool GerenciadorGrafico::isOpen() {
	return obj.isOpen();
}
void GerenciadorGrafico::atualizarEventos() {
    while (const std::optional evento = obj.pollEvent()) {
        if (evento->is<sf::Event::Closed>()) {
            obj.close();
        }
        else if (const auto* textoDigitado = evento->getIf<sf::Event::TextEntered>()) {
            if (digitando) {
                // backspace para apagar a última letra
                if (textoDigitado->unicode == 8) {
                    if (!nome_jogador.empty()) {
                        nome_jogador.pop_back();
                    }
                } else if (textoDigitado->unicode < 128) {
                    if (nome_jogador.size() < 15) { // tamanho máximo de 15 caracteres
                        nome_jogador += static_cast<char>(textoDigitado->unicode);
                    }
                }
            }
        }
    }
}

void GerenciadorGrafico::clear() {
	obj.clear();
}
void GerenciadorGrafico::display() {
	obj.display();
}
sf::RenderWindow& GerenciadorGrafico::getJanela() const {
	return const_cast<sf::RenderWindow&>(obj);
}
std::string GerenciadorGrafico::getNomeJogador(){
    return nome_jogador;
}
void GerenciadorGrafico::limparNomeJogador(){
    nome_jogador = "";
}
void GerenciadorGrafico::setDigitando(bool digitando_in){
    digitando=digitando_in;
}
