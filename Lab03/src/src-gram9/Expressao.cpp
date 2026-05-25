#include "Expressao.hpp"
#include "ExpressaoDivisao.hpp"
#include "ExpressaoIgualdade.hpp"
#include "ExpressaoMenor.hpp"
#include "ExpressaoMod.hpp"
#include "ExpressaoMultiplicacao.hpp"
#include "ExpressaoNegacao.hpp"
#include "ExpressaoSoma.hpp"
#include "ExpressaoSubtracao.hpp"
#include "ExpressaoValor.hpp"
#include "ExpressaoVariavel.hpp"
#include <iostream>
#include "../debug-util.hpp"

using namespace std;

Expressao* Expressao::extrai_expressao(No_arv_parse* no) {
  if (no == NULL) return NULL;

  switch(no->regra) {
  case 50: { // 50) unary_expr -> KW_NOT unary_expr
    ExpressaoNegacao* res = new ExpressaoNegacao();
    res->expressao = extrai_expressao(no->filhos[1]);
    return res;
  }
  case 36: { // 36) cmp_expr -> add_expr OP_EQ add_expr
    ExpressaoIgualdade* res = new ExpressaoIgualdade();
    res->esquerda = extrai_expressao(no->filhos[0]);
    res->direita = extrai_expressao(no->filhos[2]);
    return res;
  }
  case 38: { // 38) cmp_expr -> add_expr OP_LT add_expr
    ExpressaoMenor* res = new ExpressaoMenor();
    res->esquerda = extrai_expressao(no->filhos[0]);
    res->direita = extrai_expressao(no->filhos[2]);
    return res;
  }
  case 43: { // 43) add_expr -> add_expr OP_PLUS mul_expr
    ExpressaoSoma* res = new ExpressaoSoma();
    res->esquerda = extrai_expressao(no->filhos[0]);
    res->direita = extrai_expressao(no->filhos[2]);
    return res;
  }
  case 44: { // 44) add_expr -> add_expr OP_MINUS mul_expr
    ExpressaoSubtracao* res = new ExpressaoSubtracao();
    res->esquerda = extrai_expressao(no->filhos[0]);
    res->direita = extrai_expressao(no->filhos[2]);
    return res;
  }
  case 46: { // 46) mul_expr -> mul_expr OP_TIMES unary_expr
    ExpressaoMultiplicacao* res = new ExpressaoMultiplicacao();
    res->esquerda = extrai_expressao(no->filhos[0]);
    res->direita = extrai_expressao(no->filhos[2]);
    return res;
  }
  case 47: { // 47) mul_expr -> mul_expr OP_DIV unary_expr
    ExpressaoDivisao* res = new ExpressaoDivisao();
    res->esquerda = extrai_expressao(no->filhos[0]);
    res->direita = extrai_expressao(no->filhos[2]);
    return res;
  }
  case 48: { // 48) mul_expr -> mul_expr OP_MOD unary_expr
    ExpressaoMod* res = new ExpressaoMod();
    res->esquerda = extrai_expressao(no->filhos[0]);
    res->direita = extrai_expressao(no->filhos[2]);
    return res;
  }
  case 60: { // 60) primary -> LPAREN expr RPAREN
    return extrai_expressao(no->filhos[1]);
  }
  case 59: { // 59) primary -> ID
    ExpressaoVariavel* res = new ExpressaoVariavel();
    res->nome = ID::extrai_ID(no->filhos[0]);
    return res;
  }
  case 56: // 56) primary -> INT_LIT
  case 57: // 57) primary -> REAL_LIT
  case 58: { // 58) primary -> STRING_LIT
    ExpressaoValor* res = new ExpressaoValor();
    res->valor = ValorLiteral::extrai_valor_literal(no->filhos[0]);
    return res;
  }
  
  // Regras de repasse (ex: add_expr -> mul_expr)
  // 27, 31, 33, 35, 42, 45, 49, 51, 55, 64
  case 27: case 31: case 33: case 35: case 42: case 45: case 49: case 51: case 55: case 64: {
    return extrai_expressao(no->filhos[0]);
  }
  
  default: {
    return NULL;
  }
  }
}

void Expressao::debug_com_tab(int tab) {
  tab3(tab);
  cerr << "Expressao generica" << endl;
}
