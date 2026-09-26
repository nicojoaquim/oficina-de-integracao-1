#pragma once

#include "Ente.h"

#include <string.h>
#include <functional>

class Jogo;

class Menu : public Ente {

private:
    Jogo* pJog;
    sf::Font fonte;
    sf::Text titulo;

    class Botao {
        private:
            char nome[30];
            int id;

        public:
            sf::RectangleShape retangulo;
            sf::Text texto;
            sf::FloatRect borda;
            std::function<bool()> acao;

            Botao() : id(-1), nome("") {
            }
            ~Botao(){
            }
            void setNome(const char* nome_botao){
                strcpy_s(nome,nome_botao); // Dependendo da versão do compilador e das bibliotecas pode ser necessário trocar para strcpy
            }
            void setId(int id_botao){
                id=id_botao;
            }
            char* getNome(){
                return nome;
            }
            int getId(){
                return id;
            }
            bool executar_acao() {
                if (acao)
                    return acao();
                else
                    return false;
            }
    };

    class SubMenu{
        private:
            int id;

        public:
            vector<Botao*> lista_botoes;
            sf::Text subtitulo;
            SubMenu() : id(-1 ){
            }
            ~SubMenu(){
            }
            void setId(int id_submenu){
                id=id_submenu;
            }
            int getId(){
                return id;
            }
            void executar();
    };
    vector<SubMenu*> lista_submenus;
    int submenu_ativo;
    bool mouse_pressionado;
    int ranking[3];
    std::string ranking_nome[3];
    int nova_pontuacao;
    sf::Text novo_nome_texto;
    sf::Color cor_botao;
    sf::Color cor_botao_selecionado;
    sf::Color cor_botao_borda;

public:
    Menu();
    ~Menu();

    void setJogo(Jogo* j);
    void executar();

    void criarTitulo(const char* nome_titulo);
    void criarSubTitulo(int id_submenu,const char* nome_subtitulo);
    void criarBotao(int id_submenu,const char* nome_botao,std::function<bool()> acao_botao, int offset=0);

    void criarSubMenu(int id_submenu);
    void gameOver();
    void vitoria(int pontuacao);
    void criarRanking(int id_submenu,int pontos,std::string nome);
    void editarRanking(int id_submenu, int id_ranking, int pontos,std::string nome);
    void digitando_pontuacao();
    void aceitar_nome();
    int getRanking(int i);
    std::string getRankNome(int i);
    void criarSprite();
};

