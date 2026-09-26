#pragma once

#include "Ente.h"
#include "GC.h"
#include "Personagens.h"
#include "Listas.h"

class Jogo;

namespace Fases {

	class Fase : public Ente {
	public:
		Jogador* pJog1;
		Jogo* pJogo;
		static bool recuperado;
		static sf::Sprite ceu;
	protected:
		Listas::ListaEntidades lista_ents;
		Gerenciadores::GerenciadorColisoes GC;
		sf::Texture texCeu;

	public:
		Fase();
		~Fase();

		void setJogo(Jogo* j);
		virtual void executar();
		virtual void limpar();
		void setJogador(Jogador* pJog1_in);
		void salvarEntidades(std::ofstream& arquivo);
		bool gameOver();
		virtual void recuperar()=0;

	protected:
		void criarCenario();
		virtual void criarInimigos() = 0;
		virtual void criarObstaculos() = 0;
		void criarProjetil(Personagem* pAtirador);

	};
}
