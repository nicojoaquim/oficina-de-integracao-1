#include "Fases.h"
#include "Principal.h"
#include <chrono>

using namespace Fases;

bool Fase::recuperado = false;
sf::Sprite Fases::Fase::ceu;

Fase::Fase() : Ente(), pJog1(NULL), pJogo(NULL), lista_ents(), GC() {
    cout << "Fase de Id: " << getId() << " criado no Jogo" << endl;
}
Fase::~Fase() {
}
void Fase::salvarEntidades(std::ofstream& arquivo) {
    lista_ents.salvar(arquivo);
}
void Fase::setJogador(Jogador* pJog1_in) {
    if (pJog1_in != NULL) {
        pJog1 = pJog1_in;
        GC.setJog1(pJog1);
        lista_ents.incluir(static_cast<Entidade*>(pJog1));
    }
}
void Fase::setJogo(Jogo* j) {
    pJogo = j;
}

bool Fase::gameOver() {
    if (pJog1->getVidas() < 1) {
        return true;
    }
    return false;
}

void Fase::executar() {

    //comandos comuns a ambas as fases:

    std::vector<Personagem*> atiradores = lista_ents.percorrer();
    for (int i = 0; i < atiradores.size(); i++) {
        criarProjetil(atiradores[i]);
    }

    GC.executar();
    lista_ents.desenhar();
    Ente::pGG->display();

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) {
        ofstream arquivo("save.txt");
        pJog1->salvo = false;

        arquivo << "Fase:" << pJogo->proxFase << "\n";
        arquivo << "Pontos:" << pJog1->getPontos() << "\n";
        lista_ents.salvar(arquivo);
    }
    //se morreu ou apertou esc, volta para o menu:
    if (gameOver() || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) {
        recuperado = false;
        if (gameOver()) {
            cout << "Jogador perdeu todas as vidas! Voltando para o menu..." << endl;
            pJogo->menu_principal.gameOver();
        }
        else {
            cout << "Jogador saiu do jogo! Voltando para o menu..." << endl;
        }
        pJog1->reset();
        pJogo->proxFase = 0;
        limpar();
    }
}

void Fase::criarCenario() {
    texCeu.loadFromFile("fundo.png");
    ceu.setTexture(texCeu);
    ceu.setRotation(180.f);
    ceu.setScale(1.4f, 1.2f);
    ceu.setPosition(1300.f, 740.f);
}
void Fase::limpar() {
    pJog1 = NULL;
    lista_ents.limpar();
    GC.limparListas();
}

void Fase::criarProjetil(Personagem* pAtirador) {
    Projetil* pj = new Projetil(pAtirador);
    //pAtirador->setProj(pj);
    GC.incluirProjetil(pj);
    lista_ents.incluir(static_cast<Entidade*>(pj));
}
