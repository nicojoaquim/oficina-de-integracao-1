#pragma once

#include "Projetil.h"
#include <fstream>
#include "Personagens.h"
#include "Jogador.h"

using namespace Entidades;

Projetil::Projetil() : Entidade(), pAtirador(NULL), ativo(false) {

}

Projetil::Projetil(Personagem* pAtirador_in) : Entidade(), ativo(true), pAtirador(pAtirador_in) {

    if (dynamic_cast<Personagens::Jogador*>(pAtirador) != nullptr) {
        // Comportamento quando o atirador é um Jogador
        velx=(35+rand()%10);
        vely=0;
        setTexture("bola.png");
    }
    else if (dynamic_cast<Personagens::Inimigo*>(pAtirador) != nullptr) {
        // Comportamento quando o atirador é um Inimigo
        velx=(15+rand()%7);
        vely=(-5+rand()%10);
        setTexture("bola_verde.png");
    }

    x=pAtirador->getx()+pAtirador->getSizex();
    y=pAtirador->gety()+pAtirador->getSizey()/2;

    if(pAtirador->direcao==false){
        velx=-velx;
    }

    sizex = 2.5;
    sizey = 2.5;
    sf::CircleShape* shape = new sf::CircleShape(sizex+ sizey);

	shape->setFillColor(sf::Color(0x54270DFF));
	shape->setPosition({x, y});
	pHitbox = shape;
	criarSprite();
}

Projetil::~Projetil(){
    desativar();
}

void Projetil::criar(){

}

void Projetil::executar(){
    gravitar();
    x += velx;
    y += vely;
	pHitbox->move({velx, vely});
}
void Projetil::salvar(){
    if(pAtirador!=NULL){
        buffer  << getTipo() << " "
                << pAtirador->getId() << " "
                << ativo << " ";

        salvarDataBuffer();
    }
}

void Projetil::carregar(std::ifstream& arquivo) {
    arquivo >> ativo;
    Entidade::carregar(arquivo);
    pHitbox->setPosition({x, y});
}

const int Projetil::getVidas() {
    return (int)ativo;
}

void Projetil::desativar(){
    if(pAtirador!=NULL){
        //pAtirador->removeProj();
        pAtirador=NULL;
    }
    ativo=false;
}

Personagem* Projetil::getAtirador(){
    return pAtirador;
}


void Projetil::criarSprite() {

    sf::FloatRect limitesSprite = pSprite->getLocalBounds();
    pSprite->setOrigin({ limitesSprite.size.x / 2.0f, limitesSprite.size.y / 2.0f });
    pSprite->setScale({ 0.4f, 0.4f });
}

void Projetil::atualizarSprite() {
    pSprite->setPosition({ pHitbox->getPosition().x + sizex * 1.90f, pHitbox->getPosition().y + sizey * 1.8f });
}

std::string Projetil::getTipo() {
    return "Projetil";
}
