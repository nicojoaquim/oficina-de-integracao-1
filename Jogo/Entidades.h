#pragma once

#include "Ente.h"
#include <iostream>

/*namespace Personagens {
	class Dragao;
}*/

namespace Entidades {

	class Entidade : public Ente {
	protected:
	    sf::Shape* pHitbox;
		float x;
		float y;

		float sizex;
		float sizey;

        float velx;
        float vely;

		ostream buffer;

		static const float gravidade;

    protected:
		void salvarDataBuffer();
		void carregar(std::ifstream& arquivo);
		void setBuffer(std::streambuf* sb);

	public:
		Entidade();
		virtual ~Entidade();

		virtual void executar() = 0;
		virtual void salvar() = 0;
		virtual void criar() = 0;
		//virtual void carregar() = 0;
		float getx();
		float gety();
		float getSizex();
		float getSizey();
		float getvelx();
		float getvely();
		void gravitar();
        sf::Shape* getHitbox();
		void setPosition(float x, float y);
		void setvelx(float x);
		void setvely(float y);
		virtual const int getPontos();
		virtual const int getVidas() = 0;
		//const bool getObs();
		ostream& getBuffer();
		std::ostream& getConteudoBuffer();

		virtual std::string getTipo() = 0;

	};
}

