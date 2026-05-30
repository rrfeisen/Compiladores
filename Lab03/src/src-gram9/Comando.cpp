/*
 * Comando.cpp — gramática 9 (Icon)
 *
 * Regras relevantes:
 *  13  stmt_list -> stmt_list stmt
 *  14  stmt_list -> stmt
 *  15  stmt -> expr
 *  16  stmt -> if_stmt
 *  17  stmt -> while_stmt
 *  18  stmt -> return_stmt
 *  19  if_stmt -> KW_IF expr KW_THEN block
 *  20  if_stmt -> KW_IF expr KW_THEN block KW_ELSE block
 *  21  if_stmt -> KW_IF expr KW_THEN block KW_ELSE if_stmt
 *  22  while_stmt -> KW_WHILE expr KW_DO block
 *  23  return_stmt -> KW_RETURN expr
 *  24  block -> LBRACE stmt_list RBRACE
 *  25  block -> LBRACE RBRACE
 */

#include "Comando.hpp"
#include <iostream>
#include "../debug-util.hpp"
using namespace std;

// ----------------------------------------------------------------
// debug
// ----------------------------------------------------------------
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
  for (auto c : then_cmds) c->debug_com_tab(tab+1);
  if (!else_cmds.empty()) {
    tab3(tab); cerr << "ELSE:" << endl;
    for (auto c : else_cmds) c->debug_com_tab(tab+1);
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
  for (auto c : corpo) c->debug_com_tab(tab+1);
}
void ComandoRetorno::debug_com_tab(int tab) {
  tab3(tab); cerr << "RETURN:" << endl;
  if (expressao) expressao->debug_com_tab(tab+1);
}
void ComandoExpr::debug_com_tab(int tab) {
  tab3(tab); cerr << "EXPR_CMD:" << endl;
  if (expressao) expressao->debug_com_tab(tab+1);
}

// ----------------------------------------------------------------
// extrai_block — regras 24 e 25
// ----------------------------------------------------------------
vector<Comando*> Comando::extrai_block(No_arv_parse* no) {
  if (no->regra == 25) return vector<Comando*>(); // block vazio
  // regra 24: block -> LBRACE stmt_list RBRACE
  return extrai_stmt_list(no->filhos[1]);
}

// ----------------------------------------------------------------
// extrai_stmt_list — regras 13 e 14
// ----------------------------------------------------------------
vector<Comando*> Comando::extrai_stmt_list(No_arv_parse* no) {
  if (no->regra == 14) { // stmt_list -> stmt
    vector<Comando*> res;
    Comando* c = extrai_stmt(no->filhos[0]);
    if (c) res.push_back(c);
    return res;
  }
  // regra 13: stmt_list -> stmt_list stmt
  vector<Comando*> res = extrai_stmt_list(no->filhos[0]);
  Comando* c = extrai_stmt(no->filhos[1]);
  if (c) res.push_back(c);
  return res;
}

// ----------------------------------------------------------------
// extrai_stmt — regras 15-18
// ----------------------------------------------------------------
Comando* Comando::extrai_stmt(No_arv_parse* no) {
  switch(no->regra) {

  case 15: { // stmt -> expr
    Expressao* e = Expressao::extrai_expressao(no->filhos[0]);
    // Se for atribuição, encapsula em ComandoAtribuicao
    if (dynamic_cast<ExpressaoAtribuicao*>(e)) {
      ComandoAtribuicao* ca = new ComandoAtribuicao();
      ca->expressao = e;
      return ca;
    }
    ComandoExpr* ce = new ComandoExpr();
    ce->expressao = e;
    return ce;
  }

  case 16: { // stmt -> if_stmt
    return extrai_stmt(no->filhos[0]);
  }

  case 17: { // stmt -> while_stmt
    return extrai_stmt(no->filhos[0]);
  }

  case 18: { // stmt -> return_stmt
    return extrai_stmt(no->filhos[0]);
  }

  case 19: { // if_stmt -> KW_IF expr KW_THEN block
    ComandoIf* ci = new ComandoIf();
    ci->condicao  = Expressao::extrai_expressao(no->filhos[1]);
    ci->then_cmds = extrai_block(no->filhos[3]);
    return ci;
  }

  case 20: { // if_stmt -> KW_IF expr KW_THEN block KW_ELSE block
    ComandoIf* ci = new ComandoIf();
    ci->condicao  = Expressao::extrai_expressao(no->filhos[1]);
    ci->then_cmds = extrai_block(no->filhos[3]);
    ci->else_cmds = extrai_block(no->filhos[5]);
    return ci;
  }

  case 21: { // if_stmt -> KW_IF expr KW_THEN block KW_ELSE if_stmt
    ComandoIf* ci = new ComandoIf();
    ci->condicao  = Expressao::extrai_expressao(no->filhos[1]);
    ci->then_cmds = extrai_block(no->filhos[3]);
    ci->else_if   = extrai_stmt(no->filhos[5]);
    return ci;
  }

  case 22: { // while_stmt -> KW_WHILE expr KW_DO block
    ComandoWhile* cw = new ComandoWhile();
    cw->condicao = Expressao::extrai_expressao(no->filhos[1]);
    cw->corpo    = extrai_block(no->filhos[3]);
    return cw;
  }

  case 23: { // return_stmt -> KW_RETURN expr
    ComandoRetorno* cr = new ComandoRetorno();
    cr->expressao = Expressao::extrai_expressao(no->filhos[1]);
    return cr;
  }

  default:
    cerr << "extrai_stmt: regra nao tratada " << no->regra << endl;
    return NULL;
  }
}
