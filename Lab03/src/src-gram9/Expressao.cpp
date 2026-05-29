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

  if (no->filhos.size() == 3) {
      string op = no->filhos[1]->simb;
      if (op == "OP_PLUS") {
          ExpressaoSoma* res = new ExpressaoSoma();
          res->esquerda = extrai_expressao(no->filhos[0]); res->direita = extrai_expressao(no->filhos[2]); return res;
      }
      if (op == "OP_MINUS") {
          ExpressaoSubtracao* res = new ExpressaoSubtracao();
          res->esquerda = extrai_expressao(no->filhos[0]); res->direita = extrai_expressao(no->filhos[2]); return res;
      }
      if (op == "OP_TIMES") {
          ExpressaoMultiplicacao* res = new ExpressaoMultiplicacao();
          res->esquerda = extrai_expressao(no->filhos[0]); res->direita = extrai_expressao(no->filhos[2]); return res;
      }
      if (op == "OP_DIV") {
          ExpressaoDivisao* res = new ExpressaoDivisao();
          res->esquerda = extrai_expressao(no->filhos[0]); res->direita = extrai_expressao(no->filhos[2]); return res;
      }
      if (op == "OP_MOD") {
          ExpressaoMod* res = new ExpressaoMod();
          res->esquerda = extrai_expressao(no->filhos[0]); res->direita = extrai_expressao(no->filhos[2]); return res;
      }
      if (op == "OP_EQ") {
          ExpressaoIgualdade* res = new ExpressaoIgualdade();
          res->esquerda = extrai_expressao(no->filhos[0]); res->direita = extrai_expressao(no->filhos[2]); return res;
      }
      if (op == "OP_LT") {
          ExpressaoMenor* res = new ExpressaoMenor();
          res->esquerda = extrai_expressao(no->filhos[0]); res->direita = extrai_expressao(no->filhos[2]); return res;
      }
      if (no->filhos[0]->simb == "LPAREN") return extrai_expressao(no->filhos[1]);
  }
  
  if (no->filhos.size() == 2 && no->filhos[0]->simb == "KW_NOT") {
      ExpressaoNegacao* res = new ExpressaoNegacao();
      res->expressao = extrai_expressao(no->filhos[1]); return res;
  }

  if (no->filhos.size() == 1) {
      if (no->filhos[0]->simb == "ID") {
          ExpressaoVariavel* res = new ExpressaoVariavel();
          res->nome = ID::extrai_ID(no->filhos[0]); return res;
      }
      if (no->filhos[0]->simb == "INT_LIT" || no->filhos[0]->simb == "REAL_LIT" || no->filhos[0]->simb == "STRING_LIT" || no->filhos[0]->simb == "KW_TRUE" || no->filhos[0]->simb == "KW_FALSE") {
          ExpressaoValor* res = new ExpressaoValor();
          res->valor = ValorLiteral::extrai_valor_literal(no->filhos[0]); return res;
      }
      // Repasse
      return extrai_expressao(no->filhos[0]);
  }

  return NULL;
}

void Expressao::debug_com_tab(int tab) {
  tab3(tab); cerr << "Expressao generica" << endl;
}
