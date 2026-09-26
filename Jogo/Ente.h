#pragma once
#include "GG.h"
using namespace Gerenciadores;

#include <iostream>
using namespace std;

class Ente {
private:
	static int cont_id;

protected:
	int id;
	static GerenciadorGrafico* pGG;
	sf::Texture* pTexture;
	sf::Sprite* pSprite;

public:
	Ente();
	virtual ~Ente();

	virtual void executar() = 0;
	void desenhar();
	static void setGG(GerenciadorGrafico* pG);
    sf::Sprite& getSprite();
	void setTexture(const std::string& arquivo);
	virtual void criarSprite();
	virtual void atualizarSprite();
	int getId() const;
};
