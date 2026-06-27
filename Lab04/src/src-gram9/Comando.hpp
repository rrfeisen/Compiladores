#ifndef _COMANDO_HPP_
#define _COMANDO_HPP_
#include <vector>
#include "../Arvore.hpp"
#include "Expressao.hpp"
using namespace std;

class Comando {
public:
  virtual ~Comando() {}
  virtual void debug_com_tab(int tab);

  static vector<Comando*> extrai_stmt_list(No_arv_parse* no);
  static Comando*         extrai_stmt(No_arv_parse* no);
  static vector<Comando*> extrai_block(No_arv_parse* no);
};

class ComandoAtribuicao : public Comando {
public:
  Expressao* expressao;
  ComandoAtribuicao() : expressao(NULL) {}
  void debug_com_tab(int tab);
};

class ComandoIf : public Comando {
public:
  Expressao*        condicao;
  vector<Comando*>  then_cmds;
  vector<Comando*>  else_cmds;
  Comando*          else_if;

  ComandoIf() : condicao(NULL), else_if(NULL) {}
  void debug_com_tab(int tab);
};

class ComandoWhile : public Comando {
public:
  Expressao*       condicao;
  vector<Comando*> corpo;

  ComandoWhile() : condicao(NULL) {}
  void debug_com_tab(int tab);
};

class ComandoRetorno : public Comando {
public:
  Expressao* expressao;
  ComandoRetorno() : expressao(NULL) {}
  void debug_com_tab(int tab);
};

class ComandoExpr : public Comando {
public:
  Expressao* expressao;
  ComandoExpr() : expressao(NULL) {}
  void debug_com_tab(int tab);
};

#endif
