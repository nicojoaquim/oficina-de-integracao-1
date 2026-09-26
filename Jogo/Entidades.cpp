#include "Entidades.h"
#include <fstream>
//#include "Personagens.h"

using namespace Entidades;

//força de gravidade que se aplica a todas as entidades:
const float Entidade::gravidade = 0;
Entidade::Entidade() : Ente(), pHitbox(NULL), x(-1), y(-1), sizex(0), sizey(0), vely(0), velx(0), buffer(NULL) {

}

Entidade::~Entidade() {
    pHitbox=NULL;
	x = -1;
	y = -1;
	sizex = 0;
	sizey = 0;
}

void Entidade::setBuffer(std::streambuf* sb) {
    buffer.rdbuf(sb);
}

void Entidade::salvarDataBuffer() {
    buffer  << id << " "
            << x << " "
            << y << " "
            << velx << " "
            << vely << " "
            << sizex << " "
            << sizey << "\n";
}
void Entidade::carregar(std::ifstream& arquivo) {
    arquivo >> id >> x >> y >> velx >> vely >> sizex >> sizey;
}

std::ostream& Entidade::getConteudoBuffer() {
    return buffer;
}

ostream& Entidade::getBuffer() { return buffer; }
float Entidade::getx() { return x; }
float Entidade::gety() { return y; }
float Entidade::getSizex() { return sizex; }
float Entidade::getSizey() { return sizey; }
float Entidade::getvelx() { return velx; }
float Entidade::getvely() { return vely; }
void Entidade::gravitar() {
    vely -= gravidade;
}

sf::Shape* Entidade::getHitbox() {
	return pHitbox;
}

void Entidade::setPosition(float xNew, float yNew) {
	x = xNew;
	y = yNew;
	pHitbox->setPosition(xNew, yNew);
}
void Entidade::setvelx(float x){
    velx=x;
}
void Entidade::setvely(float y){
    vely=y;
}
const int Entidade::getPontos() { return 0; }
