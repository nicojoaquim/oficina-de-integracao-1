#pragma once

#include "Fases.h"

class Jogo;

namespace Fases {

	class FasePrimeira : public Fase {
	private:

	public:
		FasePrimeira();
		~FasePrimeira();
		void executar();

	protected:
	    void recuperar();
		void criarInimigos();
		void criarObstaculos();
		void criarSprite();
	};
}

