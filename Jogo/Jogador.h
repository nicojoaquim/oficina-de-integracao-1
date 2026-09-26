#pragma once
#include "Personagens.h"
using namespace Entidades;

namespace Personagens {
	class Jogador : public Personagem {
	protected:
	    sf::Keyboard::Key botao_cima;
	    sf::Keyboard::Key botao_esquerda;
	    sf::Keyboard::Key botao_baixo;
	    sf::Keyboard::Key botao_direita;
	    sf::Keyboard::Key botao_atirar;
		int pontos;
		sf::Clock relogioDano;
		float tempoInvencivel;
		bool invulneravel;
		int max_vidas;
		bool tecla_pressionada;

	public:
		bool salvo;
		float forca_andar;

	public:
		Jogador();
		~Jogador();

		void atribuirBotoes();
		void executar();
		void salvar();
		void carregar(std::ifstream& arquivo);
		void criar();
		void mover(float velx, float vely);
		void operator--(int dano);
		void operator++();
		const int getPontos();
		void setPontos(int p);
		void reset();
		void moverParaSpawn();
		void criarSprite();
		void atualizarSprite();
		std::string getTipo();
	};
}
