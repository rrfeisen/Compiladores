/*
 * Expressao.cpp — gramática 9 (Icon) — Lab04
 *
 * Mapeamento de regras (índice 0-based, ordem do .site):
 *   [... ver Lab03 para a lista completa ...]
 *  53  postfix_expr -> postfix_expr LPAREN arg_list RPAREN   <- CHAMADA DE FUNÇÃO
 *  58  primary -> ID
 *  60  arg_list -> expr_list
 *  61  arg_list -> ''
 *  62  expr_list -> expr_list COMMA expr
 *  63  expr_list -> expr
 *
 * Mudança em relação ao Lab03: a regra 53 (chamada de função) agora é
 * tratada de fato, gerando um nó ExpressaoChamada com a lista de
 * argumentos avaliados. Isso é necessário para o Lab04 poder identificar
 * quais variáveis aparecem como argumento de chamada de função (tais
 * variáveis devem ser alocadas no Frame, e não em pseudo-registrador).
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
  tab3(tab); cerr << "VAR[" << (nome ? nome->nome : "?") << "]";
  if (acesso_frame) cerr << " (frame atribuido)";
  cerr << endl;
}
void ExpressaoChamada::debug_com_tab(int tab) {
  tab3(tab); cerr << "CHAMADA[" << nome_funcao << "] (" << argumentos.size() << " args)" << endl;
  for (size_t i = 0; i < argumentos.size(); ++i) {
    argumentos[i]->debug_com_tab(tab+1);
  }
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

// ----------------------------------------------------------------
// Helpers para listas de argumentos (regras 60-63)
// ----------------------------------------------------------------

// expr_list -> expr_list COMMA expr (62) | expr (63)
static void extrai_expr_list(No_arv_parse* no, vector<Expressao*>& out) {
  if (no->regra == 63) { // expr_list -> expr
    out.push_back(Expressao::extrai_expressao(no->filhos[0]));
    return;
  }
  // regra 62: expr_list -> expr_list COMMA expr
  extrai_expr_list(no->filhos[0], out);
  out.push_back(Expressao::extrai_expressao(no->filhos[2]));
}

// arg_list -> expr_list (60) | '' (61)
static vector<Expressao*> extrai_arg_list(No_arv_parse* no) {
  vector<Expressao*> res;
  if (no->regra == 61) return res; // vazio
  extrai_expr_list(no->filhos[0], res); // regra 60
  return res;
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

  // --- postfix: dot e bracket simplificados (fora do escopo do Lab04) ---
  case 51: // postfix_expr -> postfix_expr OP_DOT ID
  case 52: // postfix_expr -> postfix_expr LBRACKET expr RBRACKET
    return extrai_expressao(no->filhos[0]);

  // --- CHAMADA DE FUNÇÃO: postfix_expr -> postfix_expr LPAREN arg_list RPAREN ---
  case 53: {
    ExpressaoChamada* r = new ExpressaoChamada();
    // postfix_expr (filhos[0]) deve resolver para um nome de função.
    // Na fatia da gramática usada, isso só acontece quando postfix_expr
    // é, em última instância, um ID (primary -> ID, regra 58).
    No_arv_parse* alvo = no->filhos[0];
    while (alvo->regra == 54 || alvo->regra == 50) { // postfix_expr->primary, unary_expr->postfix_expr
      alvo = alvo->filhos[0];
    }
    if (alvo->regra == 58) { // primary -> ID
      r->nome_funcao = alvo->filhos[0]->dado_extra;
    } else {
      r->nome_funcao = "<expr>"; // chamada sobre expressao complexa; nome generico
    }
    r->argumentos = extrai_arg_list(no->filhos[2]);
    return r;
  }

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
