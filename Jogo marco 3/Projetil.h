
#pragma once

#include "Entidades.h"
#include <iostream>

namespace Personagens {
	class Personagem;
}

namespace Entidades {

    class Projetil : public Entidade{
    protected:
        bool ativo;
        Personagem* pAtirador;

    public:
        Projetil();
        Projetil(Personagem* pAtirador_in);
        ~Projetil();

        void executar();
        void salvar();
        void carregar(std::ifstream& arquivo);
        void criar();
        const int getVidas();
        void desativar();
		float arrasto();
		Personagem* getAtirador();
		void criarSprite();
		void atualizarSprite();
		std::string getTipo();
    };
}
