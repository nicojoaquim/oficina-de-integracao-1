#pragma once

#include "GG.h"
#include "stdio.h"
#include "Jogador.h"
#include "Fase1.h"
#include "Ente.h"
#include "menu.h"

using namespace Fases;
using namespace Personagens;

class Jogo {
private:
	Jogador* pJog1;
	Gerenciadores::GerenciadorGrafico GG;

public:
	static int proxFase;
	FasePrimeira fase1;
    //FaseSegunda fase2;
    Menu menu_principal;

public:
	Jogo();
	~Jogo();

	Jogador* getJogador1();
	void executar();
	void recuperar();
	void recuperaRank();
	void randomizar();
};

