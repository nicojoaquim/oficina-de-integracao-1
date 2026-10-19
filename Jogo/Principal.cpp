
#include "Principal.h"

#include <ctime>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <filesystem>

using namespace std;

void Jogo::randomizar() {
	srand(static_cast<unsigned int>(time(NULL)));
}

int Jogo::proxFase = 0;
Jogo::Jogo() : pJog1(NULL), fase1(), /*fase2(),*/ menu_principal() {
	randomizar();
	pJog1 = new Jogador();

	recuperaRank();
}

Jogo::~Jogo() {
	if (proxFase != -1) {
		ofstream arquivoFisico("save.txt");

		if (arquivoFisico.is_open()) {
			cout << "Salvando... " << endl;
			arquivoFisico << "Fase:" << proxFase << "\n";
			arquivoFisico << "Pontos:" << pJog1->getPontos() << "\n";
			fase1.salvarEntidades(arquivoFisico);
			//fase2.salvarEntidades(arquivoFisico);

			arquivoFisico.close();

			cout << "Jogo salvo! " << endl;
		}
		else {
			cout << "Erro ao abrir o arquivo de save para escrita." << endl;
		}
	}

	ofstream arquivoRank("rank.txt");

	if (arquivoRank.is_open()) {
		for (int i = 0; i < 3; i++) {
			arquivoRank << "Ranking " << menu_principal.getRankNome(i) << " "
				<< menu_principal.getRanking(i) << "\n";
		}

		arquivoRank.close();
	}
	else {
		cout << "Erro ao abrir o arquivo de save Rank para escrita." << endl;
	}

	cout << "Limpando jogo... " << endl;

	fase1.limpar();
	//fase2.limpar();

	cout << "Deletando Jogador de Id: " << pJog1->getId() << " Tipo: " << typeid(*pJog1).name() << "... ";
	delete pJog1;
	cout << "Deletado" << endl;
	pJog1 = NULL;
}

Jogador* Jogo::getJogador1() {
	return pJog1;
}

void Jogo::executar() {

	Ente::setGG(&GG);

	menu_principal.setJogo(this);

	fase1.setJogo(this);

	//fase2.setJogo(this);

	while (GG.isOpen() && proxFase >= 0) {
		switch (proxFase) {
		case 0:
			menu_principal.executar();
			cout<<"Saindo do menu, para a fase "<<proxFase<<endl;
			break;
		case 1:
		    fase1.setJogador(pJog1);
		    fase1.executar();
			break;

		case 2:
            //fase2.setJogador(pJog1);
			//fase2.executar();
			break;
		}
	}
}

void Jogo::recuperar() {
	Fase::recuperado = true;

	printf("AAA\n");

	ifstream arquivo("save.txt");

	std::string linha;
	std::getline(arquivo, linha);
	int fase;
	if (linha.length() > 5) {
        fase = std::stoi(linha.substr(5));
	} else {
        std::cout << "Erro ao carregar a fase. Linha muito curta. " << std::endl;
	}

	if (fase == -1) { fase = 0; }
	std::getline(arquivo, linha);
	if(pJog1!=NULL){
        if (linha.length() > 7) {
            pJog1->setPontos(std::stoi(linha.substr(7)));
        } else {
            std::cout << "Erro ao carregar os pontos. Linha muito curta. " << std::endl;
            pJog1->setPontos(0);
        }
	}

	std::string tipo;
	while (arquivo >> tipo)
	{
		if (tipo == "Jogador")
		{
		    delete pJog1;

            pJog1 = new Jogador();
            pJog1->carregar(arquivo);
		}
	}

	proxFase = fase;
}

void Jogo::recuperaRank() {
	ifstream arquivo("rank.txt");
	int rank_pos = 0;

	std::string tipo;
	while (arquivo >> tipo)
	{
		if (tipo == "Ranking")
		{
			int pontuacao;
			std::string nome;

			arquivo >> nome >> pontuacao;

			menu_principal.editarRanking(2, rank_pos, pontuacao, nome);
			rank_pos++;
		}
	}
}
