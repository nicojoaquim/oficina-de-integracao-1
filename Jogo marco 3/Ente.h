#pragma once
#include "GG.h"
#include <string>

class Ente {
private:
    static int cont_id;

protected:
    int id;
    static Gerenciadores::GerenciadorGrafico* pGG;
    sf::Texture* pTexture;
    sf::Sprite* pSprite;

public:
    Ente();
    virtual ~Ente();

    virtual void executar() = 0;
    void desenhar();
    static void setGG(Gerenciadores::GerenciadorGrafico* pG);
    sf::Sprite& getSprite();
    void setTexture(const std::string& arquivo);
    virtual void criarSprite();
    virtual void atualizarSprite();
    int getId() const;
};
