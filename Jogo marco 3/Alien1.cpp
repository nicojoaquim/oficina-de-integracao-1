#include "Jogador.h"
#include <fstream>
#include "Alien1.h"
using namespace Personagens;

int Personagens::Alien1::cont_aliens = 0;
int Personagens::Alien1::cont_aliens_cumulativo = 0;

Alien1::Alien1() : Inimigo(), rapidez(0), timer(0), y_base(0), x_base(0) {
    cont_aliens++;
    cont_aliens_cumulativo++;
    podeSofrerDano=true;
    direcao=false;

	sf::RectangleShape* shape = new sf::RectangleShape();
	shape->setFillColor(sf::Color(0x54270DFF));
	pHitbox = shape;
}

Alien1::~Alien1() {
    cont_aliens--;
}

void Alien1::criar() {
    sizex = 35;
	sizey = 70;

	num_vidas = nivel_maldade+2;
    x = 1280;
	y_base = 80 + (rand() % 530);
	y=(y_base-40)+(rand() % 40);
	x_base=980 + (rand() % 40);

	vely=10+5*nivel_maldade;
	//rapidez=3+2*(rand()%100 + 1)/100.0;

	//velx=rapidez;

	criarSprite();
}
void Alien1::criar(int nivel) {
    nivel_maldade=nivel;
    sizex = 35;
	sizey = 70;

	num_vidas = nivel_maldade+2;
    x = 1280;
	y_base = 80 + (rand() % 530);
	y=(y_base-50)+(rand() % 50);
	x_base=980 + (rand() % 40);


	vely=0.5+0.5*nivel_maldade;
	//rapidez=3+2*(rand()%100 + 1)/100.0;

	//velx=rapidez;

	criarSprite();
}
void Alien1::salvar() {
	buffer << getTipo() << " "
	       << rapidez << " ";
	Inimigo::salvarDataBuffer();
}

void Alien1::carregar(std::ifstream& arquivo) {
    arquivo >> rapidez;
    Inimigo::carregar(arquivo);
    criarSprite();
}

void Alien1::danificar(Jogador* pJog) {
    pJog->operator--(1);
}

void Alien1::operator--(int dano) {
    Personagem::operator--(dano);
	setTexture("ship_16_colour_cinza.png");
	pHitbox->setFillColor(sf::Color(0xFFADADFF));
	timer=5;
}

void Alien1::executar() {
    //timer para voltar para a cor normal, se recebeu dano recentemente
    if(timer==1){
        setTexture("ship_16_colour.png");
        pHitbox->setFillColor(sf::Color(0x54270DFF));
        timer=0;
    }
    if(timer>0){
        timer--;
    }

    if(rand()%200==0){ //chance de atirar
        atirar=true;
    } else {
        atirar=false;
    }

    if(x>x_base){
        velx=-5;
        mover(velx,0);
    } else {
        velx=0;
        if (y < y_base-50 && vely < 0) {
            vely = -vely;
        }
        if (y > y_base+50 && vely > 0) {
            vely = -vely;
        }
        mover(velx,vely);
    }


}

void Alien1::mover(float velx, float vely) {
    x += velx;
	y += vely;
	pHitbox->move({velx, vely});
}

void Alien1::criarSprite() {
    setTexture("ship_16_colour.png");
    static_cast<sf::RectangleShape*>(pHitbox)->setSize({ sizex, sizey });
    pHitbox->setPosition({ x, y });

    sf::FloatRect limitesSprite = pSprite->getLocalBounds();
    pSprite->setOrigin({ limitesSprite.size.x / 2.0f, limitesSprite.size.y / 2.0f });
    //pSprite->setOrigin({ limitesSprite.size.x / 2.0f, limitesSprite.size.y / 2.0f });
    pSprite->setRotation(sf::degrees(-90.f));
    pSprite->setScale({ 0.4f, 0.4f});
}

void Alien1::atualizarSprite() {
    pSprite->setPosition({ x - 3.f + sizex / 2.0f, y + sizey / 2.0f });
    /*float scala = 1.2f;
    if (direcao) {
        pSprite->setScale({ scala, scala });
        pSprite->setOrigin({ 24.f, 32.f });
    }
    else {
        pSprite->setScale({ -scala, scala });
        pSprite->setOrigin({ 8.f, 32.f });
    }*/
}

std::string Alien1::getTipo() {
	return "Alien1";
}

int Alien1::getContAliens() {
    return cont_aliens;
}

int Alien1::getContAliens_cumulativo() {
    return cont_aliens_cumulativo;
}
void Alien1::resetarContadores() {
    cont_aliens = 0;
    cont_aliens_cumulativo = 0;
}
