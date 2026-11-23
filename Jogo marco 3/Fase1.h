#pragma once

#include "Fases.h"

class Jogo;

namespace Fases {

	class FasePrimeira : public Fase {
	private:
        int maxAlien1;
        int proximo_marco;
        int nivel;
	public:
		FasePrimeira();
		~FasePrimeira();
		void executar();
		void limpar() override;

	protected:
	    void recuperar();
		void criarAlien1();
		void criarObstaculos();
		void criarSprite();
		void atualizarSprite();
	};
}

