#pragma once

#include "Personagens.h"

namespace Personagens {

class Inimigo : public Personagem {
	protected:
		int nivel_maldade;

    public:
        bool podeSofrerDano;
	public:
		Inimigo();
		~Inimigo();

		void salvarDataBuffer();
		void carregar(std::ifstream& arquivo);
		virtual void executar() = 0;
		virtual void danificar(Jogador* pJog) = 0;
		virtual void salvar() = 0;
		virtual void criar() = 0;
		virtual std::string getTipo() = 0;
};

}
