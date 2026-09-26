#include "menu.h"
#include <optional>
#include <iostream>
#include "Principal.h"

Menu::Menu() : Ente(), pJog(NULL), cor_botao(41, 85, 156), cor_botao_selecionado(56, 134, 193), cor_botao_borda(217,193,252), submenu_ativo(0), nova_pontuacao(0), mouse_pressionado(false) {
    try
    {
        if (!fonte.loadFromFile("BAUHS93.TTF"))
        {
            throw std::runtime_error("Nao conseguiu carregar a fonte BAUHS93.TTF.");
        }

        criarTitulo("Space Shooter");
    }
    catch (const std::exception& e)
    {
        std::cerr << "Erro no menu: " << e.what() << std::endl;
    }
    criarTitulo("Space Shooter");

    criarSubMenu(0); //menu principal
    criarSubMenu(1); //novo jogo
    criarSubMenu(2); //rankings
    criarSubMenu(3); //game over
    criarSubMenu(4); //vitoria (sem high score)
    criarSubMenu(5); //vitoria (com high score)
    criarSubTitulo(1,"NOVO JOGO");
    criarSubTitulo(2,"RANKINGS");
    criarSubTitulo(3,"GAME OVER");

    //return true quando o botão faz sair do menu, return false quando continua no menu
    criarBotao(0,"NOVO JOGO", [this]() { pJog->proxFase = 1; Fase::recuperado = false;  return true;} );

    criarBotao(0, "CONTINUAR", [this]() { pJog->recuperar();  return true; });

    criarBotao(0,"RANKINGS",[this]() {submenu_ativo = 2; return false;} );
        criarRanking(2,ranking[0]=0,ranking_nome[0]='...');
        criarRanking(2,ranking[1]=0,ranking_nome[1]='...');
        criarRanking(2,ranking[2]=0,ranking_nome[2]='...');
        criarBotao(2,"VOLTAR",[this]() {submenu_ativo=0; return false;} );

    criarBotao(0,"FECHAR",[this]() {pJog->proxFase = -1; return true;} );


    criarBotao(3,"MENU",[this]() {submenu_ativo=0; return false;} );
    criarBotao(4,"MENU",[this]() {submenu_ativo=0; return false;} );
    criarBotao(5,"OK",[this]() { aceitar_nome(); submenu_ativo=0; return false;},1);

    criarSprite();
    cout << "Menu de Id: " << getId() << " criado no Jogo" << endl;
}

Menu::~Menu() {
    pJog = NULL;
    for(int i=0;i<(int)lista_submenus.size(); i++) {
        for(int j=0;j<lista_submenus[i]->lista_botoes.size();j++){
            delete lista_submenus[i]->lista_botoes[j];
        }
        lista_submenus[i]->lista_botoes.clear();
        delete lista_submenus[i];
    }
    lista_submenus.clear();
}

void Menu::criarSprite(){
    setTexture("fundo3.png");

    pSprite->setRotation(180.f);
    //pSprite->setScale(1.4f, 1.6f);
    pSprite->setPosition(1280, 720);
    //pSprite->setColor(sf::Color(112, 146, 190));
}
void Menu::setJogo(Jogo* j) {
    pJog = j;
}
void Menu::gameOver(){
    submenu_ativo=3;
}

void Menu::vitoria(int pontuacao){
    if(pontuacao>ranking[2]){
        criarSubTitulo(5,("VOCE VENCEU! PONTOS: " + std::to_string(pontuacao)).c_str());
        nova_pontuacao=pontuacao;
        submenu_ativo=5;

        novo_nome_texto.setFont(fonte);
        novo_nome_texto.setCharacterSize(50);
        novo_nome_texto.setScale({ 1.f, -1.f });
        novo_nome_texto.setFillColor(sf::Color::Black);
        pGG->setDigitando(true);
    } else {
        criarSubTitulo(4,("VOCE VENCEU! PONTOS: " + std::to_string(pontuacao)).c_str());
        submenu_ativo=4;
    }
}

void Menu::digitando_pontuacao(){
    novo_nome_texto.setString("Digite seu nome: " + pGG->getNomeJogador());
    sf::FloatRect limitesTexto = novo_nome_texto.getLocalBounds();
    novo_nome_texto.setOrigin(limitesTexto.left + limitesTexto.width / 2.0f, limitesTexto.top + limitesTexto.height / 2.0f);
    novo_nome_texto.setPosition({ 640, 395 });

    pGG->getJanela().draw(novo_nome_texto);
}
void Menu::aceitar_nome(){
    if(nova_pontuacao>ranking[1]){
        if(nova_pontuacao>ranking[0]){
            cout<<"Novo top score! Primeiro lugar."<<endl;
            editarRanking(2,2,ranking[1],ranking_nome[1]);
            editarRanking(2,1,ranking[0],ranking_nome[0]);
            editarRanking(2,0,nova_pontuacao, pGG->getNomeJogador());
        } else {
            cout<<"Novo top score! Segundo lugar."<<endl;
            editarRanking(2,2,ranking[1],ranking_nome[1]);
            editarRanking(2,1,nova_pontuacao, pGG->getNomeJogador());
        }
    } else {
        cout<<"Novo top score! Terceiro lugar."<<endl;
        editarRanking(2,2,nova_pontuacao, pGG->getNomeJogador());
    }
    pGG->limparNomeJogador();
    pGG->setDigitando(false);
}

void Menu::criarTitulo(const char* nome_titulo){
    titulo.setString(nome_titulo);
    titulo.setFont(fonte);
    titulo.setCharacterSize(110);
    titulo.setScale({ 1.f, -1.f });

    sf::FloatRect limitesTexto = titulo.getLocalBounds();
    titulo.setOrigin(limitesTexto.left + limitesTexto.width / 2.0f, limitesTexto.top + limitesTexto.height / 2.0f);
    titulo.setPosition({ 640, 600 });
}

void Menu::criarSubTitulo(int id_submenu,const char* nome_subtitulo){
    if (id_submenu >= 0 && id_submenu < static_cast<int>(lista_submenus.size()) && lista_submenus[id_submenu] != nullptr) {
        lista_submenus[id_submenu]->subtitulo.setString(nome_subtitulo);
        lista_submenus[id_submenu]->subtitulo.setFont(fonte);
        lista_submenus[id_submenu]->subtitulo.setCharacterSize(60);
        lista_submenus[id_submenu]->subtitulo.setScale({ 1.f, -1.f });
        lista_submenus[id_submenu]->subtitulo.setFillColor(sf::Color::Black);

        sf::FloatRect limitesTexto = lista_submenus[id_submenu]->subtitulo.getLocalBounds();
        lista_submenus[id_submenu]->subtitulo.setOrigin(limitesTexto.left + limitesTexto.width / 2.0f, limitesTexto.top + limitesTexto.height / 2.0f);
        lista_submenus[id_submenu]->subtitulo.setPosition({ 640, 500 });
    } else {
        std::cout << "Erro ao criar o subtitulo no submenu " << id_submenu << std::endl;
    }
}

void Menu::criarRanking(int id_submenu,int pontos,std::string nome){
    Botao *novo_botao = new Botao();
    lista_submenus[id_submenu]->lista_botoes.push_back(novo_botao);
    novo_botao->setId((int)lista_submenus[id_submenu]->lista_botoes.size());

    int altura=75;

    novo_botao->texto.setString((nome+"     " + std::to_string(pontos)).c_str());
    novo_botao->texto.setFont(fonte);
    novo_botao->texto.setCharacterSize(50);
    novo_botao->texto.setScale({ 1.f, -1.f });
    novo_botao->texto.setFillColor(sf::Color::Black);
    sf::FloatRect limitesTexto = novo_botao->texto.getLocalBounds();
    novo_botao->texto.setOrigin(limitesTexto.left + limitesTexto.width / 2.0f, limitesTexto.top + limitesTexto.height / 2.0f);
    novo_botao->texto.setPosition({ 640.0f, 470.0f - altura*novo_botao->getId()});
}
void Menu::editarRanking(int id_submenu, int id_ranking, int pontos,std::string nome){
    ranking[id_ranking]=pontos;
    ranking_nome[id_ranking]=nome;
    lista_submenus[id_submenu]->lista_botoes[id_ranking]->texto.setString((nome+"     " + std::to_string(pontos)).c_str());

    //centralizar o texto:
    sf::FloatRect limitesTexto = lista_submenus[id_submenu]->lista_botoes[id_ranking]->texto.getLocalBounds();
    lista_submenus[id_submenu]->lista_botoes[id_ranking]->texto.setOrigin(limitesTexto.left + limitesTexto.width / 2.0f, limitesTexto.top + limitesTexto.height / 2.0f);
    lista_submenus[id_submenu]->lista_botoes[id_ranking]->texto.setPosition({ 640.0f, 470.0f - 75*lista_submenus[id_submenu]->lista_botoes[id_ranking]->getId()});

}

void Menu::criarBotao(int id_submenu,const char* nome_botao,std::function<bool()> acao_botao,int offset){
    Botao *novo_botao = new Botao();
    lista_submenus[id_submenu]->lista_botoes.push_back(novo_botao);
    novo_botao->setNome(nome_botao);
    novo_botao->setId((int)lista_submenus[id_submenu]->lista_botoes.size());
    novo_botao->acao=acao_botao;

    int altura=70;
    novo_botao->retangulo.setSize(sf::Vector2f(390, (float)altura));

    sf::Vector2f tamanho = novo_botao->retangulo.getSize();
    novo_botao->retangulo.setOrigin(tamanho.x / 2.0f, tamanho.y / 2.0f);

    novo_botao->retangulo.setScale({ 1.f, -1.f });
    novo_botao->retangulo.setPosition({ 640.0f, 470.0f - (altura+5.0f)*(novo_botao->getId() + offset)});//altura+5 porque tem 5 pixels de borda
    novo_botao->retangulo.setFillColor(sf::Color(cor_botao));
    novo_botao->retangulo.setOutlineThickness(5);
    novo_botao->retangulo.setOutlineColor(sf::Color(cor_botao_borda));

    novo_botao->borda = novo_botao->retangulo.getGlobalBounds();

    novo_botao->texto.setString(nome_botao);
    novo_botao->texto.setFont(fonte);
    novo_botao->texto.setCharacterSize(50);
    novo_botao->texto.setScale({ 1.f, -1.f });
    sf::FloatRect limitesTexto = novo_botao->texto.getLocalBounds();
    novo_botao->texto.setOrigin(limitesTexto.left + limitesTexto.width / 2.0f, limitesTexto.top + limitesTexto.height / 2.0f);
    novo_botao->texto.setPosition(novo_botao->retangulo.getPosition());
}

void Menu::criarSubMenu(int id_submenu){
    SubMenu *novo_submenu = new SubMenu();
    novo_submenu->setId(id_submenu);
    lista_submenus.push_back(novo_submenu);
}

void Menu::executar() {
    cout<<"Menu executando..."<<endl;
    while (Ente::pGG->isOpen()) {

        Ente::pGG->atualizarEventos();
        Ente::pGG->clear();
        //Ente::pGG->desenharFundo(fundo);
        Ente::pGG->desenharEnte(this);

        sf::Vector2i mousePos = sf::Mouse::getPosition(Ente::pGG->getJanela());
        sf::Vector2f mousePosF = Ente::pGG->getJanela().mapPixelToCoords(mousePos);

        for(int i=0;i<lista_submenus[submenu_ativo]->lista_botoes.size();i++){
            lista_submenus[submenu_ativo]->lista_botoes[i]->retangulo.setFillColor(sf::Color(cor_botao));//primeiro pinta todos os botões na cor padrão
        }
        for(int i=0;i<lista_submenus[submenu_ativo]->lista_botoes.size();i++){
            if(lista_submenus[submenu_ativo]->lista_botoes[i]->borda.contains(mousePosF)){
                lista_submenus[submenu_ativo]->lista_botoes[i]->retangulo.setFillColor(sf::Color(cor_botao_selecionado));//se detecta que o mouse está em um botão, pinta ele de outra cor
                if (mouse_pressionado && !sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)){//executa ao soltar o botão do mouse

                    if (lista_submenus[submenu_ativo]->lista_botoes[i]->executar_acao() == true) {
                        mouse_pressionado=sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
                        submenu_ativo=0;
                        return;
                    }
                }
                break;//break para não selecionar dois botões ao mesmo tempo (se o mouse ficar bem na borda entre eles)
            }
        }
        mouse_pressionado=sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
        for(int i=0;i<lista_submenus[submenu_ativo]->lista_botoes.size();i++){
            pGG->getJanela().draw(lista_submenus[submenu_ativo]->lista_botoes[i]->retangulo);
            pGG->getJanela().draw(lista_submenus[submenu_ativo]->lista_botoes[i]->texto);
        }
        pGG->getJanela().draw(titulo);
        pGG->getJanela().draw(lista_submenus[submenu_ativo]->subtitulo);

        if(submenu_ativo==5){
            digitando_pontuacao();
        }

        Ente::pGG->display();
    }
}

int Menu::getRanking(int i) { return ranking[i]; }
std::string Menu::getRankNome(int i) { return ranking_nome[i];}

