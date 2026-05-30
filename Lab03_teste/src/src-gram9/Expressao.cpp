/*
 * Expressao.cpp — gramática 9 (Icon)
 *
 * Mapeamento de regras (índice 0-based, ordem do .site):
 *   0  program -> decl_list
 *   1  decl_list -> decl_list decl
 *   2  decl_list -> decl
 *   3  decl -> record_decl
 *   4  decl -> proc_decl
 *   5  record_decl -> KW_RECORD ID LPAREN param_list RPAREN
 *   6  param_list -> id_list
 *   7  param_list -> ''
 *   8  id_list -> id_list COMMA ID
 *   9  id_list -> ID
 *  10  proc_decl -> KW_PROCEDURE ID LPAREN param_list RPAREN opt_local stmt_list KW_END
 *  11  opt_local -> KW_LOCAL id_list
 *  12  opt_local -> ''
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
 *  26  expr -> assign_expr
 *  27  assign_expr -> or_expr OP_ASGN assign_expr
 *  28  assign_expr -> or_expr OP_PLUS_ASGN assign_expr
 *  29  assign_expr -> or_expr OP_MINUS_ASGN assign_expr
 *  30  assign_expr -> or_expr
 *  31  or_expr -> or_expr OP_OR and_expr
 *  32  or_expr -> and_expr
 *  33  and_expr -> and_expr OP_AND cmp_expr
 *  34  and_expr -> cmp_expr
 *  35  cmp_expr -> add_expr OP_EQ add_expr
 *  36  cmp_expr -> add_expr OP_NEQ add_expr
 *  37  cmp_expr -> add_expr OP_LT add_expr
 *  38  cmp_expr -> add_expr OP_LE add_expr
 *  39  cmp_expr -> add_expr OP_GT add_expr
 *  40  cmp_expr -> add_expr OP_GE add_expr
 *  41  cmp_expr -> add_expr
 *  42  add_expr -> add_expr OP_PLUS mul_expr
 *  43  add_expr -> add_expr OP_MINUS mul_expr
 *  44  add_expr -> mul_expr
 *  45  mul_expr -> mul_expr OP_TIMES unary_expr
 *  46  mul_expr -> mul_expr OP_DIV unary_expr
 *  47  mul_expr -> mul_expr OP_MOD unary_expr
 *  48  mul_expr -> unary_expr
 *  49  unary_expr -> KW_NOT unary_expr
 *  50  unary_expr -> postfix_expr
 *  51  postfix_expr -> postfix_expr OP_DOT ID
 *  52  postfix_expr -> postfix_expr LBRACKET expr RBRACKET
 *  53  postfix_expr -> postfix_expr LPAREN arg_list RPAREN
 *  54  postfix_expr -> primary
 *  55  primary -> INT_LIT
 *  56  primary -> REAL_LIT
 *  57  primary -> STRING_LIT
 *  58  primary -> ID
 *  59  primary -> LPAREN expr RPAREN
 *  60  arg_list -> expr_list
 *  61  arg_list -> ''
 *  62  expr_list -> expr_list COMMA expr
 *  63  expr_list -> expr
 */

#include "Expressao.hpp"
#include <iostream>
#include <iomanip>
#include "../debug-util.hpp"
using namespace std;

// ---------- helpers de debug ----------
void Expressao::debug_com_tab(int tab) {
  tab3(tab); cerr << "Expressao generica" << endl;
}
void ExpressaoValor::debug_com_tab(int tab) {
  tab3(tab);
  cerr << (valor ? valor->como_string() : "nil")
       << " [" << (valor && valor->tipo ? valor->tipo->nome() : "?") << "]" << endl;
}
void ExpressaoVariavel::debug_com_tab(int tab) {
  tab3(tab); cerr << "VAR[" << (nome ? nome->nome : "?") << "]" << endl;
}
void ExpressaoAtribuicao::debug_com_tab(int tab) {
  tab3(tab); cerr << "ATRIB(" << op << ")" << endl;
  if (esquerda) esquerda->debug_com_tab(tab+1);
  if (direita)  direita->debug_com_tab(tab+1);
}
void ExpressaoOr::debug_com_tab(int tab) {
  tab3(tab); cerr << "OR" << endl;
  if (esquerda) esquerda->debug_com_tab(tab+1);
  if (direita)  direita->debug_com_tab(tab+1);
}
void ExpressaoAnd::debug_com_tab(int tab) {
  tab3(tab); cerr << "AND" << endl;
  if (esquerda) esquerda->debug_com_tab(tab+1);
  if (direita)  direita->debug_com_tab(tab+1);
}
void ExpressaoCmp::debug_com_tab(int tab) {
  tab3(tab); cerr << "CMP(" << op << ")" << endl;
  if (esquerda) esquerda->debug_com_tab(tab+1);
  if (direita)  direita->debug_com_tab(tab+1);
}
void ExpressaoSoma::debug_com_tab(int tab) {
  tab3(tab); cerr << "SOMA" << endl;
  if (esquerda) esquerda->debug_com_tab(tab+1);
  if (direita)  direita->debug_com_tab(tab+1);
}
void ExpressaoSubtracao::debug_com_tab(int tab) {
  tab3(tab); cerr << "SUB" << endl;
  if (esquerda) esquerda->debug_com_tab(tab+1);
  if (direita)  direita->debug_com_tab(tab+1);
}
void ExpressaoMultiplicacao::debug_com_tab(int tab) {
  tab3(tab); cerr << "MUL" << endl;
  if (esquerda) esquerda->debug_com_tab(tab+1);
  if (direita)  direita->debug_com_tab(tab+1);
}
void ExpressaoDivisao::debug_com_tab(int tab) {
  tab3(tab); cerr << "DIV" << endl;
  if (esquerda) esquerda->debug_com_tab(tab+1);
  if (direita)  direita->debug_com_tab(tab+1);
}
void ExpressaoMod::debug_com_tab(int tab) {
  tab3(tab); cerr << "MOD" << endl;
  if (esquerda) esquerda->debug_com_tab(tab+1);
  if (direita)  direita->debug_com_tab(tab+1);
}
void ExpressaoNot::debug_com_tab(int tab) {
  tab3(tab); cerr << "NOT" << endl;
  if (operando) operando->debug_com_tab(tab+1);
}

// ---------- fábrica principal ----------
Expressao* Expressao::extrai_expressao(No_arv_parse* no) {
  if (no == NULL) return NULL;

  switch(no->regra) {

  // --- passagens diretas ---
  case 26: // expr -> assign_expr
  case 30: // assign_expr -> or_expr
  case 32: // or_expr -> and_expr
  case 34: // and_expr -> cmp_expr
  case 41: // cmp_expr -> add_expr
  case 44: // add_expr -> mul_expr
  case 48: // mul_expr -> unary_expr
  case 50: // unary_expr -> postfix_expr
  case 54: // postfix_expr -> primary
    return extrai_expressao(no->filhos[0]);

  // --- atribuições ---
  case 27: { // assign_expr -> or_expr OP_ASGN assign_expr
    ExpressaoAtribuicao* r = new ExpressaoAtribuicao();
    r->op       = ":=";
    r->esquerda = extrai_expressao(no->filhos[0]);
    r->direita  = extrai_expressao(no->filhos[2]);
    return r;
  }
  case 28: { // assign_expr -> or_expr OP_PLUS_ASGN assign_expr
    ExpressaoAtribuicao* r = new ExpressaoAtribuicao();
    r->op       = "+:=";
    r->esquerda = extrai_expressao(no->filhos[0]);
    r->direita  = extrai_expressao(no->filhos[2]);
    return r;
  }
  case 29: { // assign_expr -> or_expr OP_MINUS_ASGN assign_expr
    ExpressaoAtribuicao* r = new ExpressaoAtribuicao();
    r->op       = "-:=";
    r->esquerda = extrai_expressao(no->filhos[0]);
    r->direita  = extrai_expressao(no->filhos[2]);
    return r;
  }

  // --- lógicos ---
  case 31: { // or_expr -> or_expr OP_OR and_expr
    ExpressaoOr* r = new ExpressaoOr();
    r->esquerda = extrai_expressao(no->filhos[0]);
    r->direita  = extrai_expressao(no->filhos[2]);
    return r;
  }
  case 33: { // and_expr -> and_expr OP_AND cmp_expr
    ExpressaoAnd* r = new ExpressaoAnd();
    r->esquerda = extrai_expressao(no->filhos[0]);
    r->direita  = extrai_expressao(no->filhos[2]);
    return r;
  }

  // --- comparações ---
  case 35: case 36: case 37: case 38: case 39: case 40: {
    ExpressaoCmp* r = new ExpressaoCmp();
    r->esquerda = extrai_expressao(no->filhos[0]);
    r->direita  = extrai_expressao(no->filhos[2]);
    // filho[1] é o operador terminal
    string simb = no->filhos[1]->simb;
    if      (simb == "OP_EQ")  r->op = "==";
    else if (simb == "OP_NEQ") r->op = "!=";
    else if (simb == "OP_LT")  r->op = "<";
    else if (simb == "OP_LE")  r->op = "<=";
    else if (simb == "OP_GT")  r->op = ">";
    else if (simb == "OP_GE")  r->op = ">=";
    return r;
  }

  // --- aritméticos ---
  case 42: { // add_expr -> add_expr OP_PLUS mul_expr
    ExpressaoSoma* r = new ExpressaoSoma();
    r->esquerda = extrai_expressao(no->filhos[0]);
    r->direita  = extrai_expressao(no->filhos[2]);
    return r;
  }
  case 43: { // add_expr -> add_expr OP_MINUS mul_expr
    ExpressaoSubtracao* r = new ExpressaoSubtracao();
    r->esquerda = extrai_expressao(no->filhos[0]);
    r->direita  = extrai_expressao(no->filhos[2]);
    return r;
  }
  case 45: { // mul_expr -> mul_expr OP_TIMES unary_expr
    ExpressaoMultiplicacao* r = new ExpressaoMultiplicacao();
    r->esquerda = extrai_expressao(no->filhos[0]);
    r->direita  = extrai_expressao(no->filhos[2]);
    return r;
  }
  case 46: { // mul_expr -> mul_expr OP_DIV unary_expr
    ExpressaoDivisao* r = new ExpressaoDivisao();
    r->esquerda = extrai_expressao(no->filhos[0]);
    r->direita  = extrai_expressao(no->filhos[2]);
    return r;
  }
  case 47: { // mul_expr -> mul_expr OP_MOD unary_expr
    ExpressaoMod* r = new ExpressaoMod();
    r->esquerda = extrai_expressao(no->filhos[0]);
    r->direita  = extrai_expressao(no->filhos[2]);
    return r;
  }

  // --- unário ---
  case 49: { // unary_expr -> KW_NOT unary_expr
    ExpressaoNot* r = new ExpressaoNot();
    r->operando = extrai_expressao(no->filhos[1]);
    return r;
  }

  // --- postfix (ignoramos dot/bracket/call por simplicidade) ---
  case 51: // postfix_expr -> postfix_expr OP_DOT ID  (não avaliamos)
  case 52: // postfix_expr -> postfix_expr LBRACKET expr RBRACKET
  case 53: // postfix_expr -> postfix_expr LPAREN arg_list RPAREN
    // Retorna apenas a parte esquerda (simplificação aceitável para o lab)
    return extrai_expressao(no->filhos[0]);

  // --- literais ---
  case 55: // primary -> INT_LIT
  case 56: { // primary -> REAL_LIT
    ExpressaoValor* r = new ExpressaoValor();
    r->valor = ValorLiteral::extrai_valor_literal(no->filhos[0]);
    return r;
  }
  case 57: { // primary -> STRING_LIT  (tratamos como INT 0 — não usado no lab)
    ExpressaoValor* r = new ExpressaoValor();
    r->valor = new ValorLiteral(0);
    return r;
  }
  case 58: { // primary -> ID
    ExpressaoVariavel* r = new ExpressaoVariavel();
    r->nome = ID::extrai_ID(no->filhos[0]);
    return r;
  }
  case 59: { // primary -> LPAREN expr RPAREN
    return extrai_expressao(no->filhos[1]);
  }

  default:
    cerr << "extrai_expressao: regra nao tratada " << no->regra << endl;
    return NULL;
  }
}
