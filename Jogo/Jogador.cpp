#include "Jogador.h"
#include <fstream>

using namespace Personagens;

Jogador::Jogador() : Personagem(), pontos(0), forca_andar(10), max_vidas(30), invulneravel(false), tempoInvencivel(0.2f), salvo(false), tecla_pressionada(false) {
	num_vidas=max_vidas;
	sizex = 45;
	sizey = 18;
	atribuirBotoes();

	sf::RectangleShape* shape = new sf::RectangleShape(sf::Vector2f(sizex, sizey));
	shape->setFillColor(sf::Color::Green);

	pHitbox = shape;
	criarSprite();
}

Jogador::~Jogador() {}

void Jogador::atribuirBotoes(){
    botao_cima=sf::Keyboard::Key::W;
    botao_esquerda=sf::Keyboard::Key::A;
    botao_baixo=sf::Keyboard::Key::S;
    botao_direita=sf::Keyboard::Key::D;
    botao_atirar=sf::Keyboard::Key::Space;
}

void Jogador::executar() {

    if(relogioDano.getElapsedTime().asSeconds()>tempoInvencivel && invulneravel){
        invulneravel=false;
        setTexture("jogador1.png");
        pSprite->setTexture(*pTexture);
    }

    if (sf::Keyboard::isKeyPressed(botao_direita) && !sf::Keyboard::isKeyPressed(botao_esquerda)) {
        x += forca_andar;
    }
    else if (sf::Keyboard::isKeyPressed(botao_esquerda) && !sf::Keyboard::isKeyPressed(botao_direita)) {
        x -= forca_andar;
    }

    if (sf::Keyboard::isKeyPressed(botao_cima) && !sf::Keyboard::isKeyPressed(botao_baixo)) {
        y += forca_andar;
    }
    else if (sf::Keyboard::isKeyPressed(botao_baixo) && !sf::Keyboard::isKeyPressed(botao_cima)) {
        y -= forca_andar;
    }

    if (sf::Keyboard::isKeyPressed(botao_atirar)) {
        if(tecla_pressionada==false){
            atirar=true;
            tecla_pressionada=true;
        } else {
            atirar=false;
        }
    } else {
        atirar=false;
        tecla_pressionada=false;
    }

	gravitar();
	//mover(velx, vely);
	//forca_andar = 5;

	pHitbox->setPosition(x, y);
}
void Jogador::salvar() {
	if (!salvo) {
		salvo = true;

		buffer	<< getTipo() << " "
				<< pontos << " "
				//<< relogioDano << " "
				<< tempoInvencivel << " "
				<< invulneravel << " "
				<< max_vidas << " "
				<< forca_andar << " "
				<< tecla_pressionada << " ";

		Personagem::salvarDataBuffer();
	}
}
void Jogador::carregar(std::ifstream& arquivo) {
    arquivo >> pontos >> /*relogioDano >>*/ tempoInvencivel >> invulneravel >> max_vidas >> forca_andar >> tecla_pressionada;
    Personagem::carregar(arquivo);
    pHitbox->setPosition(x, y);
}

void Jogador::criar(){
}

void Jogador::mover(float velx, float vely) {
	x += velx;
	y += vely;
	pHitbox->move(velx, vely);
}

void Jogador::operator--(int dano) {
	if (!invulneravel) {
		if (num_vidas > 0) {
			Personagem::operator--(dano);
			invulneravel=true;
            setTexture("jogador1dano.png");
            pSprite->setTexture(*pTexture);
		}
		relogioDano.restart();
	}
	else {
		std::cout << "Dano ignorado! Jogador invencivel por mais: " << (tempoInvencivel - relogioDano.getElapsedTime().asSeconds()) << "s" << std::endl;
	}
}
void Jogador::moverParaSpawn(){
    setPosition(100, 200);
}

void Jogador::operator++() {
	pontos++;
}

const int Jogador::getPontos() {
	return pontos;
}
void Jogador::setPontos(int p) { pontos = p; }
void Jogador::reset(){
    vely=0;
    num_vidas=max_vidas;
    pontos=0;
}

void Jogador::criarSprite(){
    setTexture("jogador.png");

	//pSprite->setTextureRect(sf::IntRect({ 0, 0 }, { 16, 16 }));

    sf::FloatRect limitesSprite = pSprite->getLocalBounds();
    pSprite->setOrigin(limitesSprite.width / 2.0f, limitesSprite.height / 2.0f);
    pSprite->setScale(-1,1);

	pSprite->setRotation(180.0f);
}
void Jogador::atualizarSprite() {
	pSprite->setPosition(x+0.6*sizex,y+sizey);

	if (direcao) {
		//pSprite->setOrigin(12.5f, 16.f);
		//pSprite->setScale(3, 4);
	}
	else {
		//pSprite->setOrigin(4.f, 16.f);
		//pSprite->setScale(-3, 4);
	}
}

std::string Jogador::getTipo() {
	return "Jogador";
}
