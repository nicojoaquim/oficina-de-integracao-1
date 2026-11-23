#pragma once
#include "Entidades.h"
#include "Projetil.h"
using namespace Entidades;

namespace Personagens {

	class Personagem : public Entidade {
	protected:
		int num_vidas;
		//Projetil* pProj;
		bool atirar;

    public:
        bool direcao;
	public:
		Personagem();
		~Personagem();

		void salvarDataBuffer();
		void carregar(std::ifstream& arquivo);

		virtual void executar() = 0;
		virtual void salvar() = 0;
		virtual void criar() = 0;
		virtual void mover(float velx, float vely) = 0;
		const int getVidas();
		virtual const int getPontos();
		void setVidas(int n);
		virtual void operator--(int dano);
		virtual std::string getTipo() = 0;
		//void setProj(Projetil* pProj);
		//void removeProj();
		bool getAtirar();

	};
}
