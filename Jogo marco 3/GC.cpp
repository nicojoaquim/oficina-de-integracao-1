#include "GC.h"
#include <iostream>
#include "Ente.h"
#include "Entidades.h"
#include "Personagens.h"
#include "Inimigo.h"
#include "Jogador.h"
#include "Fases.h"

using namespace std;
using namespace Gerenciadores;

GerenciadorColisoes::GerenciadorColisoes() : pJog1(NULL) {

}
GerenciadorColisoes::~GerenciadorColisoes() {}
void GerenciadorColisoes::setJog1(Jogador* pJg1) {
	if (pJg1) {
		pJog1 = pJg1;
	}
}

bool GerenciadorColisoes::verificarColisao(Entidade* pe1, Entidade* pe2) const {
	if (pe1 == nullptr || pe2 == nullptr) return false;
    sf::FloatRect bounds1 = pe1->getHitbox()->getGlobalBounds();
    sf::FloatRect bounds2 = pe2->getHitbox()->getGlobalBounds();

    return bounds1.findIntersection(bounds2).has_value();
}

void GerenciadorColisoes::executar(){
    tratarColisoesJogsChao();
    //tratarColisoesJogsObstaculo();
    tratarColisoesJogsInimigos();
    /*tratarColisoesInimChao();
    tratarColisoesInimObstaculo();
    tratarColisoesObstaculoChao();
    tratarColisoesProjObstaculo();
    tratarColisoesJogsProjeteis();*/
    tratarColisoesProjPersonagem();
    tratarColisoesProjChao();
}

void GerenciadorColisoes::tratarColisoesJogsChao() {
    if(pJog1!=NULL){
        if (pJog1->getx() < 0) {
            pJog1->setPosition(0, pJog1->gety());
        }
        if (pJog1->getx() > 1280 - pJog1->getSizex()) {
            pJog1->setPosition(1280 - pJog1->getSizex(), pJog1->gety());
        }

        if (pJog1->gety() < 0) {
            pJog1->setPosition(pJog1->getx(), 0);
        }
        if (pJog1->gety() > 720 - pJog1->getSizey()) {
            pJog1->setPosition(pJog1->getx(),720 - pJog1->getSizey());
        }
    }
}

void GerenciadorColisoes::tratarColisoesJogsInimigos() {
	int i;

	for (i = 0; i < (int)LI.size(); i++) {
		if (LI[i] != NULL) {
            if(pJog1!=NULL){
                if (verificarColisao(static_cast<Entidade*>(pJog1), static_cast<Entidade*>(LI[i]))) {
                    pJog1->colidir(LI[i]);
                    if(LI[i]->getVidas()<1){ //se a colisão eliminou o inimigo, remove ele da lista
                        LI.erase(LI.begin() + i);
                        i--;
                    }
                }
            }
		}
	}
}
/*
void GerenciadorColisoes::tratarColisoesJogsObstaculo() {
    std::list<Obstaculo*>::iterator it;

	for (it = LO.begin(); it != LO.end(); it++) {
        Obstaculo* pObs = *it;

		if (pObs != NULL) {

            if(pJog1!=NULL){
                if (verificarColisao(static_cast<Entidade*>(pJog1), static_cast<Entidade*>(pObs))) {
                    pObs->obstaculizar(pJog1);
                }
            }
            if(pJog2!=NULL){
                if (verificarColisao(static_cast<Entidade*>(pJog2), static_cast<Entidade*>(pObs))) {
                    pObs->obstaculizar(pJog2);
                }
            }
		}
	}
}

void GerenciadorColisoes::tratarColisoesInimObstaculo() {
	std::list<Obstaculo*>::iterator it;

	for (it = LO.begin(); it != LO.end(); it++) {
		Obstaculo* pObs = *it;
		if (pObs != NULL) {
            if(typeid(*pObs) == typeid(Plataforma)){ //inimigos só interagem com plataformas (deve ter solução mais elegante que essa)
                int i;

                for (i = 0; i < (int)LI.size(); i++) {
                    if (LI[i] != NULL) {

                        if (verificarColisao(static_cast<Entidade*>(pObs), static_cast<Entidade*>(LI[i]))) {
                            pObs->obstaculizar(LI[i]);
                        }

                    }
                }
            }
		}
	}
}

void GerenciadorColisoes::tratarColisoesInimChao() {
    for (int i = 0; i < (int)LI.size(); i++) {
        if (LI[i] != NULL) {
            if (LI[i]->gety() < 200) {
                LI[i]->setvely(0);
                LI[i]->setPosition(LI[i]->getx(), 200);
            }
        }
    }
}

void GerenciadorColisoes::tratarColisoesObstaculoChao(){
    std::list<Obstaculo*>::iterator it;

    for (it = LO.begin(); it != LO.end(); it++) {
        Obstaculo* pObs = *it;
        if (pObs != NULL) {
            if (pObs->gety() < 200.0) {
                pObs->setvely(0);
                pObs->setPosition(pObs->getx() , 200);
            }
        }
    }
}

void GerenciadorColisoes::tratarColisoesProjObstaculo(){
    if (!LP.empty()) {
        std::list<Obstaculo*>::iterator it;
        std::set<Projetil*>::iterator it2;

        for (it = LO.begin(); it != LO.end(); it++) {
            Obstaculo* pObs = *it;
            if (pObs != NULL) {
                if(typeid(*pObs) == typeid(Plataforma)){
                    for (it2 = LP.begin(); it2 != LP.end(); ) {
                        Projetil* pProj = *it2;
                        if (verificarColisao(static_cast<Entidade*>(pObs), static_cast<Entidade*>(pProj))) {
                                pProj->desativar();
                                it2=LP.erase(it2);
                        } else {
                            it2++;
                        }
                    }
                }
            }
        }
    }
}

void GerenciadorColisoes::tratarColisoesJogsProjeteis(){
    if (!LP.empty()) {
        std::set<Projetil*>::iterator it2;

        for (it2 = LP.begin(); it2 != LP.end(); ) {
            Projetil* pProj = *it2;
            if (pProj != NULL) {

                if(pJog1!=NULL){
                    if (verificarColisao(static_cast<Entidade*>(pJog1), static_cast<Entidade*>(pProj))) {
                        pProj->getDragao()->danificar(pJog1);
                        pProj->desativar();
                        it2=LP.erase(it2);
                    } else if(pJog2!=NULL){
                        if (verificarColisao(static_cast<Entidade*>(pJog2), static_cast<Entidade*>(pProj))) {
                            pProj->getDragao()->danificar(pJog2);
                            pProj->desativar();
                            it2=LP.erase(it2);
                        } else {
                            it2++;
                        }
                    } else {
                        it2++;
                    }
                }
            }
        }
    }
}*/

void GerenciadorColisoes::tratarColisoesProjChao(){
    if (!LP.empty()) {
        std::set<Projetil*>::iterator it2;

        for (it2 = LP.begin(); it2 != LP.end(); ) {
            Projetil* pProj = *it2;
            if (pProj->getx() < 0 || pProj->getx() > 1280) {
                pProj->desativar();
                it2=LP.erase(it2);
            } else {
                it2++;
            }
        }
    }
}

void GerenciadorColisoes::tratarColisoesProjPersonagem() {
    for (auto it = LP.begin(); it != LP.end(); ) {
        Projetil* pProj = *it;

        if (pProj == nullptr) {
            it = LP.erase(it);
            continue;
        }

        bool colidiu = false;
        Personagens::Personagem* pAtirador = pProj->getAtirador();

        // Caso 1: O atirador é o jogador (verifica colisão contra todos os inimigos)
        if (pAtirador == static_cast<Personagens::Personagem*>(pJog1)) {
            for (size_t i = 0; i < LI.size(); i++) {
                if (LI[i] != nullptr && verificarColisao(static_cast<Entidades::Entidade*>(pProj), static_cast<Entidades::Entidade*>(LI[i]))) {
                    LI[i]->operator--(1);
                    pProj->desativar();

                    // Verifica se o inimigo morreu e remove da lista LI
                    if (LI[i]->getVidas() < 1) {
                        pJog1->operator++();
                        LI.erase(LI.begin() + i);
                    }

                    colidiu = true;
                    break;
                }
            }
        }
        // Caso 2: O atirador é um inimigo (verifica colisão apenas com o jogador)
        else if (pAtirador != nullptr) {
            if (pJog1 != nullptr && verificarColisao(static_cast<Entidades::Entidade*>(pProj), static_cast<Entidades::Entidade*>(pJog1))) {
                pJog1->operator--(1);
                pProj->desativar();
                colidiu = true;
            }
        }

        if (colidiu) {
            it = LP.erase(it);
        } else {
            ++it;
        }
    }
}





void GerenciadorColisoes::incluirInimigo(Inimigo *pI) {
	if (pI) {
		LI.push_back(pI);
	}
}

/*void GerenciadorColisoes::incluirObstaculo(Obstaculo *pO) {
	if (pO) {
		LO.push_back(pO);
	}
}
*/
void GerenciadorColisoes::incluirProjetil(Projetil* pj) {
    LP.insert(pj);
}

void GerenciadorColisoes::limparListas(){
    LI.clear();
    //LO.clear();
    LP.clear();
}

