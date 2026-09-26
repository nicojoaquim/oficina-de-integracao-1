#include "Personagens.h"
#include <fstream>

using namespace Personagens;

Personagem::Personagem() : Entidade(), num_vidas(1), direcao(true), /*pProj(NULL),*/ atirar(false) {
}
Personagem::~Personagem() {
	num_vidas = 0;
}
void Personagem::operator--(int dano) {
	num_vidas -= dano;
	if(num_vidas<0){
        num_vidas=0;
	}
}
void Personagem::salvarDataBuffer() {
    buffer	//<< id << " "
            << num_vidas << " "
            << direcao << " "
            << atirar << " ";

	Entidade::salvarDataBuffer();
}
void Personagem::carregar(std::ifstream& arquivo) {
    arquivo >> num_vidas >> direcao >> atirar;
    Entidade::carregar(arquivo);
}

const int Personagem::getVidas() {
	return num_vidas;
}
const int Personagem::getPontos() { return 0; }
void Personagem::setVidas(int n){
    num_vidas=n;
}

/*void Personagem::setProj(Projetil* pPj) {
	pProj = pPj;
}
void Personagem::removeProj() {
	pProj = NULL;
}*/
bool Personagem::getAtirar(){
    return atirar;
}
