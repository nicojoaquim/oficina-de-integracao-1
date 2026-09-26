
#include "Fases.h"
#include "Principal.h"
#include <chrono>

using namespace Fases;

//FASE PRIMEIRA
FasePrimeira::FasePrimeira() {
    criarSprite();
}
FasePrimeira::~FasePrimeira() {

}
void FasePrimeira::executar() {
    if (!recuperado) {
        pJog1->moverParaSpawn();

        criarObstaculos();
        criarInimigos();
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
        //código para passar para a fase 2: (ambos os jogadores, ou apenas o que estiver vivo, devem ir para o canto direito do cenário)
        if (pJogo->proxFase != 0) { //não executar quando o fase::executar deu gameover
            if (pJog1->getx() > 1250) {
                pJogo->proxFase = 2;
                limpar();
                return;
            }
        }
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

void FasePrimeira::criarInimigos() {
}
void FasePrimeira::criarObstaculos() {
}

void FasePrimeira::criarSprite(){
    setTexture("fundo4.png");

    pSprite->setRotation(180.f);
    //pSprite->setScale(1.4f, 1.6f);
    pSprite->setPosition(1280, 720);
    pSprite->setColor(sf::Color(255, 255, 255));
}
