#ifndef _EXPRESSAO_HPP_
#define _EXPRESSAO_HPP_
#include "../Arvore.hpp"
#include "ValorLiteral.hpp"
#include "ID.hpp"
#include "../../Frame/FrameAcesso.hpp"
#include <vector>
using namespace std;

// ----------------------------------------------------------------
// Classe base
// ----------------------------------------------------------------
class Expressao {
public:
  virtual ~Expressao() {}
  virtual void debug_com_tab(int tab);

  // Fábrica: constrói a expressão correta a partir de um nó da árvore
  static Expressao* extrai_expressao(No_arv_parse* no);
};

// ----------------------------------------------------------------
// Literal  (INT_LIT ou REAL_LIT)
// ----------------------------------------------------------------
class ExpressaoValor : public Expressao {
public:
  ValorLiteral* valor;
  ExpressaoValor() : valor(NULL) {}
  void debug_com_tab(int tab);
};

// ----------------------------------------------------------------
// Variável  (ID)
// ----------------------------------------------------------------
class ExpressaoVariavel : public Expressao {
public:
  ID* nome;
  FrameAcesso* acesso_frame; // preenchido na fase de alocacao de frame (Lab04)
  ExpressaoVariavel() : nome(NULL), acesso_frame(NULL) {}
  void debug_com_tab(int tab);
};

// ----------------------------------------------------------------
// Chamada de função:  ID ( arg_list )   (regra 53)
// ----------------------------------------------------------------
class ExpressaoChamada : public Expressao {
public:
  string nome_funcao;
  vector<Expressao*> argumentos;
  ExpressaoChamada() {}
  void debug_com_tab(int tab);
};

// ----------------------------------------------------------------
// Operações binárias
// ----------------------------------------------------------------
class ExpressaoBinaria : public Expressao {
public:
  Expressao* esquerda;
  Expressao* direita;
  ExpressaoBinaria() : esquerda(NULL), direita(NULL) {}
};

class ExpressaoAtribuicao : public ExpressaoBinaria {
public:
  // OP_ASGN := | OP_PLUS_ASGN +:= | OP_MINUS_ASGN -:=
  string op;
  void debug_com_tab(int tab);
};

class ExpressaoOr  : public ExpressaoBinaria { public: void debug_com_tab(int tab); };
class ExpressaoAnd : public ExpressaoBinaria { public: void debug_com_tab(int tab); };

class ExpressaoCmp : public ExpressaoBinaria {
public:
  string op; // "==","!=","<","<=",">",">="
  void debug_com_tab(int tab);
};

class ExpressaoSoma        : public ExpressaoBinaria { public: void debug_com_tab(int tab); };
class ExpressaoSubtracao   : public ExpressaoBinaria { public: void debug_com_tab(int tab); };
class ExpressaoMultiplicacao: public ExpressaoBinaria { public: void debug_com_tab(int tab); };
class ExpressaoDivisao     : public ExpressaoBinaria { public: void debug_com_tab(int tab); };
class ExpressaoMod         : public ExpressaoBinaria { public: void debug_com_tab(int tab); };

// ----------------------------------------------------------------
// Operação unária
// ----------------------------------------------------------------
class ExpressaoNot : public Expressao {
public:
  Expressao* operando;
  ExpressaoNot() : operando(NULL) {}
  void debug_com_tab(int tab);
};

#endif
