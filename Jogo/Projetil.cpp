#pragma once

#include "Projetil.h"
#include <fstream>
#include "Personagens.h"
#include "Jogador.h"

using namespace Entidades;

Projetil::Projetil() : Entidade(), pAtirador(NULL), ativo(false) {

}

Projetil::Projetil(Personagem* pAtirador_in) : Entidade(), ativo(true), pAtirador(pAtirador_in) {
    x=pAtirador->getx()+pAtirador->getSizex();
    //y=pAtirador->gety();
    y=pAtirador->gety()+pAtirador->getSizey()/2;

    velx=(25+rand()%20);
    if(pAtirador->direcao==false){
        velx=-velx;
    }

    vely=0;
    sizex = 2.5;
    sizey = 2.5;
    sf::CircleShape* shape = new sf::CircleShape(sizex+ sizey);

	shape->setFillColor(sf::Color(0x54270DFF));
	shape->setPosition(x, y);
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
	pHitbox->move(velx, vely);
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
    pHitbox->setPosition(x, y);
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
    setTexture("bola.png");
    sf::FloatRect limitesSprite = pSprite->getLocalBounds();
    pSprite->setOrigin(limitesSprite.width / 2.0f, limitesSprite.height / 2.0f);
    pSprite->setScale(0.4, 0.4);
}

void Projetil::atualizarSprite() {
    pSprite->setPosition(pHitbox->getPosition().x - sizex*0.20, pHitbox->getPosition().y - sizey*0.1);
}

std::string Projetil::getTipo() {
    return "Projetil";
}
