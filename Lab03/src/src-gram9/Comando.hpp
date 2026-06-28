#ifndef _COMANDO_HPP_
#define _COMANDO_HPP_
#include <vector>
#include "../Arvore.hpp"
#include "Expressao.hpp"
using namespace std;

// ----------------------------------------------------------------
// Classe base
// ----------------------------------------------------------------
class Comando {
public:
  virtual ~Comando() {}
  virtual void debug_com_tab(int tab);

  static vector<Comando*> extrai_stmt_list(No_arv_parse* no);
  static Comando*         extrai_stmt(No_arv_parse* no);
  static vector<Comando*> extrai_block(No_arv_parse* no);
};

// ----------------------------------------------------------------
// Atribuição:  lvalue := expr
// ----------------------------------------------------------------
class ComandoAtribuicao : public Comando {
public:
  Expressao* expressao; // já é uma ExpressaoAtribuicao
  ComandoAtribuicao() : expressao(NULL) {}
  void debug_com_tab(int tab);
};

// ----------------------------------------------------------------
// if-then / if-then-else / if-then-else-if
// ----------------------------------------------------------------
class ComandoIf : public Comando {
public:
  Expressao*        condicao;
  vector<Comando*>  then_cmds;
  vector<Comando*>  else_cmds;  // vazio se não houver else
  Comando*          else_if;    // != NULL se KW_ELSE if_stmt

  ComandoIf() : condicao(NULL), else_if(NULL) {}
  void debug_com_tab(int tab);
};

// ----------------------------------------------------------------
// while expr do block
// ----------------------------------------------------------------
class ComandoWhile : public Comando {
public:
  Expressao*       condicao;
  vector<Comando*> corpo;

  ComandoWhile() : condicao(NULL) {}
  void debug_com_tab(int tab);
};

// ----------------------------------------------------------------
// return expr
// ----------------------------------------------------------------
class ComandoRetorno : public Comando {
public:
  Expressao* expressao;
  ComandoRetorno() : expressao(NULL) {}
  void debug_com_tab(int tab);
};

// ----------------------------------------------------------------
// Expressão usada como comando (ex: atribuição := no topo)
// ----------------------------------------------------------------
class ComandoExpr : public Comando {
public:
  Expressao* expressao;
  ComandoExpr() : expressao(NULL) {}
  void debug_com_tab(int tab);
};

#endif
