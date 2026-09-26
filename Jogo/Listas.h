#pragma once
#include <fstream>;
#include <vector>

namespace Entidades {
    class Entidade;
}
using namespace Entidades;

namespace Listas {

    template <typename TL>
    class Lista {

    public:
        template <typename TE>
        class Elemento {
        private:
            Elemento<TE>* pProx;
            TE* pInfo;

        public:
            Elemento() : pProx(NULL), pInfo(NULL) {}
            ~Elemento() { pProx = NULL; pInfo = NULL; }
            void incluir(TE* p) { pInfo = p; }
            void setProx(Elemento<TE>* pE) { pProx = pE;  }
            const Elemento<TE>* getProximo() const { return pProx;  }
            TE* getInfo() const { return pInfo; }

        };

    private:
        Elemento<TL>* pPrimeiro;
        Elemento<TL>* pUltimo;

    public:
        Lista() : pPrimeiro(NULL), pUltimo(NULL) {}
        ~Lista() {
            pPrimeiro = NULL;
            pUltimo = NULL;
        }
        void incluir(TL* p) {
            if (!p) return;
            Elemento<TL>* novoElemento = new Elemento<TL>();
            novoElemento->incluir(p);

            if (pPrimeiro == NULL) {
                pPrimeiro = novoElemento;
                pUltimo = novoElemento;
            }
            else {
                pUltimo->setProx(novoElemento);
                pUltimo = novoElemento;
            }
        }
        void limpar() {
            Elemento<TL>* atual = pPrimeiro;
            while (atual != NULL) {
                Elemento<TL>* proximo = const_cast<Elemento<TL>*>(atual->getProximo());

                TL* pInfo = atual->getInfo();

                if (pInfo != NULL) {
                    if(typeid(*pInfo) != typeid(Jogador)){ //se não for jogador, tira da lista e apaga
                        Entidade* pEntidade = const_cast<Lista<Entidade>::Elemento<Entidade>*>(atual)->getInfo();

                        cout << "Deletando elemento de Id: " << pEntidade->getId() << " Tipo: " << typeid(*pEntidade).name() << "... ";
                        delete pInfo;
                        delete atual;
                        cout << "Deletado" << endl;
                    } else { //se for jogador, apenas tira da lista (jogador é criado e deletado pela classe principal)
                        delete atual;
                    }
                }
                atual = proximo;
            }
            pPrimeiro = NULL;
            pUltimo = NULL;
        }

        void remover(TL* p) {
            if (p == NULL || pPrimeiro == NULL) return;

            Elemento<TL>* atual = pPrimeiro;
            Elemento<TL>* anterior = NULL;

            while (atual != NULL && atual->getInfo() != p) {
                anterior = atual;
                atual = const_cast<Elemento<TL>*>(atual->getProximo());
            }

            if (atual != NULL) {
                if (atual == pPrimeiro) {
                    pPrimeiro = const_cast<Elemento<TL>*>(atual->getProximo());
                }
                else {
                    anterior->setProx(const_cast<Elemento<TL>*>(atual->getProximo()));
                }

                if (atual == pUltimo) {
                    pUltimo = anterior;
                }

                if(typeid(*p) != typeid(Jogador)){
                    delete p; //se for jogador, não deleta (jogador é criado e deletado pela classe principal)
                }
                delete atual;

                cout << "Deletado" << endl;
            }
        }

        Elemento<TL>* getPrimeiro() { return pPrimeiro; }
    };

    class ListaEntidades {
    private:
        Lista<Entidade> LEs;
        Jogador* pJog1;

    public:
        ListaEntidades() : LEs(), pJog1(NULL) {}
        ~ListaEntidades() { pJog1 = NULL; }
        void incluir(Entidade* pE) {
            LEs.incluir(pE);
            cout << typeid(*pE).name() << " e Id: " << pE->getId() << " incluido" << endl;
        }
        void setJogador(Jogador* pJog) { pJog1 = pJog; }
        std::vector<Personagem*> percorrer() {

            std::vector<Personagem*> atiradores;

            const Lista<Entidade>::Elemento<Entidade>* pAux = LEs.getPrimeiro();

            while (pAux != NULL) {
                const Lista<Entidade>::Elemento<Entidade>* pProximo = pAux->getProximo();

                Entidade* pEntidade = const_cast<Lista<Entidade>::Elemento<Entidade>*>(pAux)->getInfo();

                if (pEntidade != NULL) {
                    if (pEntidade->getVidas() > 0 ) {
                        pEntidade->executar();
                        if (Personagem* pPersonagem = dynamic_cast<Personagem*>(pEntidade)) {
                            if(pPersonagem->getAtirar()==true){
                                atiradores.push_back(pPersonagem);
                            }
                        }
                    }
                    else {
                        cout << "Entidade de Id: " << pEntidade->getId() << " morto e ignorado no desenho...";
                        LEs.remover(pEntidade);
                    }

                }
                pAux = pProximo;
            }
            return atiradores;
        }

        void desenhar(){
            const Lista<Entidade>::Elemento<Entidade>* pAux = LEs.getPrimeiro();
            while (pAux != NULL) {
                const Lista<Entidade>::Elemento<Entidade>* pProximo = pAux->getProximo();

                Entidade* pEntidade = const_cast<Lista<Entidade>::Elemento<Entidade>*>(pAux)->getInfo();
                if (pEntidade != NULL) {
                    if (pEntidade->getVidas() > 0){
                        pEntidade->desenhar();
                    }
                }
                pAux = pProximo;
            }
        }

        void limpar(){
            LEs.limpar();
        }
        void salvar(std::ofstream& arquivo) {
            const Lista<Entidade>::Elemento<Entidade>* pAux = LEs.getPrimeiro();

            while (pAux != NULL) {
                const Lista<Entidade>::Elemento<Entidade>* pProximo = pAux->getProximo();

                Entidade* pEntidade = const_cast<Lista<Entidade>::Elemento<Entidade>*>(pAux)->getInfo();

                if (pEntidade != NULL) {
                    std::cout << "DEBUG: Encontrou uma entidade para salvar!" << std::endl;
                    std::streambuf* antigo = pEntidade->getBuffer().rdbuf(arquivo.rdbuf());
                    pEntidade->salvar();
                    pEntidade->getBuffer().rdbuf(antigo);
                }
                pAux = pProximo;
            }
        }

        Personagem* buscarId(int idProcurado) {
            const Lista<Entidade>::Elemento<Entidade>* pAux = LEs.getPrimeiro();

            while (pAux != NULL) {
                const Lista<Entidade>::Elemento<Entidade>* pProximo = pAux->getProximo();

                Entidade* pEntidade = const_cast<Lista<Entidade>::Elemento<Entidade>*>(pAux)->getInfo();

                if (pEntidade != NULL) {
                    if (pEntidade->getId() == idProcurado) {
                        cout << "Carregamento: Encontrou entidade de id:" << pEntidade->getId() << endl;
                        return dynamic_cast<Personagem*>(pEntidade);
                    }
                }
                pAux = pProximo;
            }

            return nullptr;
        }

    };
}

