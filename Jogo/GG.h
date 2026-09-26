#pragma once

#include <SFML/Graphics.hpp>
#include <optional>

class Ente;
namespace Personagens {
	class Jogador;
}
using namespace Personagens;
/*namespace Entidades {
	class Entidade;
	class Projetil;
}
namespace Personagens {
	class Jogador;
	class Inimigo;
}
namespace Obstaculos {
	class Obstaculo;
}
using namespace Personagens;
using namespace Obstaculos;
using namespace Entidades;*/

namespace Gerenciadores {

	class GerenciadorGrafico {
	private:
		sf::RenderWindow obj;
		//para digitar o nome do jogador para ranking:
		std::string nome_jogador;
		bool digitando;

	public:
		GerenciadorGrafico();
		~GerenciadorGrafico();

		void desenharEnte(Ente* pE);
		void desenharFundo(sf::Sprite fundo);
		void desenharFundo(sf::Sprite fundo, sf::Sprite chao);
		void desenharVidas(Jogador* pJog1);
		bool isOpen();
		void atualizarEventos();
		void clear();
		void display();
		sf::RenderWindow& getJanela() const;

		std::string getNomeJogador();
		void limparNomeJogador();
		void setDigitando(bool digitando_in);
	};
}

