#include "Inimigo.h"
#include <fstream>
using namespace Entidades;

Inimigo::Inimigo() : Personagem(), podeSofrerDano(false), nivel_maldade(1) {

}
Inimigo::~Inimigo() {
	nivel_maldade = 0;
}

void Inimigo::salvarDataBuffer() {
    buffer	<< nivel_maldade << " ";

	Personagem::salvarDataBuffer();
}
void Inimigo::carregar(std::ifstream& arquivo) {
    arquivo >> nivel_maldade;
    Personagem::carregar(arquivo);
}
