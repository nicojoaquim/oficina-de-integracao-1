#pragma once

#include "Inimigo.h"

namespace Personagens {
    class Alien1 : public Inimigo {
	private:
		float rapidez;
		int timer;
		static int cont_aliens;
		static int cont_aliens_cumulativo;
		int y_base;
		int x_base;

	public:
		Alien1();
		~Alien1();

		void danificar(Jogador* p);
		void executar();
		void salvar();
		void carregar(std::ifstream& arquivo);
		void criar();
		void criar(int nivel);
		void mover(float velx, float vely);
		void criarSprite();
		void atualizarSprite();
		void operator--(int dano);
		std::string getTipo();
		static int getContAliens();
		static int getContAliens_cumulativo();
		static void resetarContadores();
    };

}
