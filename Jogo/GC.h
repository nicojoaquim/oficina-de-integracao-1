#pragma once

#include <optional>
#include <vector>
#include <list>
#include <set>

class Ente;
namespace Entidades {
	class Entidade;
	class Projetil;
}
namespace Personagens {
	class Jogador;
	//class Inimigo;
}
using namespace Personagens;
using namespace Entidades;

namespace Gerenciadores {

	class GerenciadorColisoes {
	private:
		Jogador* pJog1;
		//std::vector<Inimigo*> LI;
		//std::list<Obstaculo*> LO;
		std::set<Projetil*> LP;
    private:
		bool verificarColisao(Entidade* pe1, Entidade* pe2) const;
		void tratarColisoesJogsChao();
		/*void tratarColisoesJogsInimigos();
		void tratarColisoesJogsObstaculo();
		void tratarColisoesInimObstaculo();
		void tratarColisoesInimChao();
		void tratarColisoesObstaculoChao();
		void tratarColisoesProjObstaculo();
		void tratarColisoesJogsProjeteis();*/
		void tratarColisoesProjChao();
	public:
		GerenciadorColisoes();
		~GerenciadorColisoes();
		void setJog1(Jogador* pJg1);

		void executar();
		//void incluirInimigo(Inimigo* pI);
		//void incluirObstaculo(Obstaculo* pO);
		void incluirProjetil(Projetil* pj);
		void limparListas();
	};

}
