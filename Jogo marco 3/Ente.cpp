#include "Ente.h"
#include <iostream>

using namespace Gerenciadores;
using namespace std;

int Ente::cont_id = 0;
GerenciadorGrafico* Ente::pGG;

Ente::Ente() : id(cont_id++), pTexture(NULL), pSprite(NULL) {
	pTexture = new sf::Texture();
	pSprite = new sf::Sprite(*pTexture);
}
Ente::~Ente() {
	delete pTexture;
	delete pSprite;
};

void Ente::desenhar() {
	pGG->desenharEnte(this);
}

void Ente::setGG(GerenciadorGrafico* pG) {
	if (!pG) { cout << "Gerenciador Gráfico não criado [Ente]" << endl; }
	if (!pGG){ pGG = pG; }
}

sf::Sprite& Ente::getSprite() {
	return *pSprite;
}

void Ente::setTexture(const std::string& arquivo) {
    if (!pTexture->loadFromFile(arquivo)) {
        std::cout << "Erro ao carregar textura: " << arquivo << std::endl;
    }
    std::cout << "pTexture = " << pTexture << std::endl;
    pSprite->setTexture(*pTexture, true);
}

void Ente::criarSprite() {
}

void Ente::atualizarSprite() {
}

int Ente::getId() const {
    return id;
}

