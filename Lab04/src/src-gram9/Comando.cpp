/*
 * Comando.cpp — gramática 9 (Icon) — inalterado em relação ao Lab03
 */
#include "Comando.hpp"
#include <iostream>
#include "../debug-util.hpp"
using namespace std;

void Comando::debug_com_tab(int tab) {
  tab3(tab); cerr << "Comando generico" << endl;
}
void ComandoAtribuicao::debug_com_tab(int tab) {
  tab3(tab); cerr << "ATRIBUICAO:" << endl;
  if (expressao) expressao->debug_com_tab(tab+1);
}
void ComandoIf::debug_com_tab(int tab) {
  tab3(tab); cerr << "IF:" << endl;
  if (condicao) condicao->debug_com_tab(tab+1);
  tab3(tab); cerr << "THEN:" << endl;
  for (size_t i = 0; i < then_cmds.size(); ++i) then_cmds[i]->debug_com_tab(tab+1);
  if (!else_cmds.empty()) {
    tab3(tab); cerr << "ELSE:" << endl;
    for (size_t i = 0; i < else_cmds.size(); ++i) else_cmds[i]->debug_com_tab(tab+1);
  }
  if (else_if) {
    tab3(tab); cerr << "ELSE_IF:" << endl;
    else_if->debug_com_tab(tab+1);
  }
}
void ComandoWhile::debug_com_tab(int tab) {
  tab3(tab); cerr << "WHILE:" << endl;
  if (condicao) condicao->debug_com_tab(tab+1);
  tab3(tab); cerr << "DO:" << endl;
  for (size_t i = 0; i < corpo.size(); ++i) corpo[i]->debug_com_tab(tab+1);
}
void ComandoRetorno::debug_com_tab(int tab) {
  tab3(tab); cerr << "RETURN:" << endl;
  if (expressao) expressao->debug_com_tab(tab+1);
}
void ComandoExpr::debug_com_tab(int tab) {
  tab3(tab); cerr << "EXPR_CMD:" << endl;
  if (expressao) expressao->debug_com_tab(tab+1);
}

vector<Comando*> Comando::extrai_block(No_arv_parse* no) {
  if (no->regra == 25) return vector<Comando*>();
  return extrai_stmt_list(no->filhos[1]);
}

vector<Comando*> Comando::extrai_stmt_list(No_arv_parse* no) {
  if (no->regra == 14) {
    vector<Comando*> res;
    Comando* c = extrai_stmt(no->filhos[0]);
    if (c) res.push_back(c);
    return res;
  }
  vector<Comando*> res = extrai_stmt_list(no->filhos[0]);
  Comando* c = extrai_stmt(no->filhos[1]);
  if (c) res.push_back(c);
  return res;
}

Comando* Comando::extrai_stmt(No_arv_parse* no) {
  switch(no->regra) {
  case 15: {
    Expressao* e = Expressao::extrai_expressao(no->filhos[0]);
    if (dynamic_cast<ExpressaoAtribuicao*>(e)) {
      ComandoAtribuicao* ca = new ComandoAtribuicao();
      ca->expressao = e;
      return ca;
    }
    ComandoExpr* ce = new ComandoExpr();
    ce->expressao = e;
    return ce;
  }
  case 16: return extrai_stmt(no->filhos[0]);
  case 17: return extrai_stmt(no->filhos[0]);
  case 18: return extrai_stmt(no->filhos[0]);
  case 19: {
    ComandoIf* ci = new ComandoIf();
    ci->condicao  = Expressao::extrai_expressao(no->filhos[1]);
    ci->then_cmds = extrai_block(no->filhos[3]);
    return ci;
  }
  case 20: {
    ComandoIf* ci = new ComandoIf();
    ci->condicao  = Expressao::extrai_expressao(no->filhos[1]);
    ci->then_cmds = extrai_block(no->filhos[3]);
    ci->else_cmds = extrai_block(no->filhos[5]);
    return ci;
  }
  case 21: {
    ComandoIf* ci = new ComandoIf();
    ci->condicao  = Expressao::extrai_expressao(no->filhos[1]);
    ci->then_cmds = extrai_block(no->filhos[3]);
    ci->else_if   = extrai_stmt(no->filhos[5]);
    return ci;
  }
  case 22: {
    ComandoWhile* cw = new ComandoWhile();
    cw->condicao = Expressao::extrai_expressao(no->filhos[1]);
    cw->corpo    = extrai_block(no->filhos[3]);
    return cw;
  }
  case 23: {
    ComandoRetorno* cr = new ComandoRetorno();
    cr->expressao = Expressao::extrai_expressao(no->filhos[1]);
    return cr;
  }
  default:
    cerr << "extrai_stmt: regra nao tratada " << no->regra << endl;
    return NULL;
  }
}
