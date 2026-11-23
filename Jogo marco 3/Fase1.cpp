#include "Fases.h"
#include "Principal.h"
#include <chrono>
#include <fstream>
using namespace std;
#include "Alien1.h"

using namespace Fases;

//FASE PRIMEIRA
FasePrimeira::FasePrimeira() : maxAlien1(1), proximo_marco(4), nivel(1) {
    criarSprite();
}
FasePrimeira::~FasePrimeira() {
    FasePrimeira::limpar();
}
void FasePrimeira::executar() {

    if (!recuperado) {
        pJog1->moverParaSpawn();

        criarObstaculos();
    }
    else {
        recuperar();
        recuperado = false;
    }

    while (Ente::pGG->isOpen() && pJogo->proxFase == 1) {

        Ente::pGG->atualizarEventos();
        Ente::pGG->clear();
        Ente::pGG->desenharEnte(this);
        Fase::pGG->desenharVidas(pJog1);

        Fase::executar();

        if(Personagens::Alien1::getContAliens() < maxAlien1){
            if(rand()%50==0){ //chance de criar um inimigo alien1
                criarAlien1();
            }
        }
        if(Personagens::Alien1::getContAliens_cumulativo() > proximo_marco ){
            maxAlien1++;
            proximo_marco=proximo_marco+nivel+3;
            nivel++;
        }

        //código para passar para a fase 2:
        /*if (pJogo->proxFase != 0) { //não executar quando o fase::executar deu gameover
            if (pJog1->getx() > 1250) {
                pJogo->proxFase = 2;
                limpar();
                return;
            }
        }*/
    }
}

void FasePrimeira::recuperar() {
    ifstream arquivo("save.txt");

    std::string tipo;
    while (arquivo >> tipo)
    {
        if (tipo == "Projetil")
        {
            int idAtirador;
            arquivo >> idAtirador;  //salva o id do atirador

            Personagem* pAtirador = lista_ents.buscarId(idAtirador);
            if(pAtirador!=NULL){
                Projetil* proj = new Projetil(pAtirador);
                //pAtirador->setProj(proj);
                proj->carregar(arquivo);
                lista_ents.incluir(static_cast<Entidade*>(proj));
                GC.incluirProjetil(proj);
            }
        }
    }
}

void FasePrimeira::criarAlien1() {
        Alien1* inim = new Alien1();
        inim->criar(nivel);
        lista_ents.incluir(static_cast<Entidade*>(inim));
        GC.incluirInimigo(static_cast<Inimigo*>(inim));
}
void FasePrimeira::criarObstaculos() {
}

void FasePrimeira::criarSprite(){
    setTexture("fundo.png");
    pTexture->setRepeated(true);

    // Começa exatamente na ponta esquerda da imagem (pixel 0)
    scrollX = static_cast<float>(pTexture->getSize().x) - 1280.f;

    pSprite->setTextureRect(sf::IntRect({ 0, 0 }, { 1280, 720 }));
    pSprite->setRotation(sf::degrees(180.f));
    pSprite->setPosition({ 1280.f, 720.f });
    pSprite->setColor(sf::Color(255, 255, 255));
}

void FasePrimeira::atualizarSprite() {
    scrollX -= 0.25f;
    pSprite->setTextureRect(sf::IntRect({ static_cast<int>(scrollX), 0 }, { 1280, 720 }));
}

void FasePrimeira::limpar() {
    Fase::limpar();
    Personagens::Alien1::resetarContadores();
    maxAlien1=1;
    proximo_marco=4;
    nivel=1;
}
