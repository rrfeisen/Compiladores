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

// Tratei Ea, Eb, Ec, E, T e F na mesma funcao.
Expressao* Expressao::extrai_expressao(No_arv_parse* no) {
  // Ea,Eb,Ec,E,T,F. 16) a 33)
  switch(no->regra) {
  case 16: { // 16) Ea -> ! Ea
    ExpressaoNegacao* res16 = new ExpressaoNegacao();
    res16->expressao = extrai_expressao(no->filhos[1]);
    return res16;
  }
  case 17: { // 17) Ea -> Eb
    return extrai_expressao(no->filhos[0]);
  }
  case 18: { // 18) Eb -> Ec == Ec
    ExpressaoIgualdade* res18 = new ExpressaoIgualdade();
    res18->esquerda = extrai_expressao(no->filhos[0]);
    res18->direita = extrai_expressao(no->filhos[2]);
    return res18;
  }
  case 19: { // 19) Eb -> Ec
    return extrai_expressao(no->filhos[0]);
  }
  case 20: { // 20) Ec -> E < E
    ExpressaoMenor* res20 = new ExpressaoMenor();
    res20->esquerda = extrai_expressao(no->filhos[0]);
    res20->direita = extrai_expressao(no->filhos[2]);
    return res20;
  }
  case 21: { // 21) Ec -> E
    return extrai_expressao(no->filhos[0]);
  }
  case 22: { // 22) E -> E + T
    ExpressaoSoma* res22 = new ExpressaoSoma();
    res22->esquerda = extrai_expressao(no->filhos[0]);
    res22->direita = extrai_expressao(no->filhos[2]);
    return res22;
  }
  case 23: { // 23) E -> E - T
    ExpressaoSubtracao* res23 = new ExpressaoSubtracao();
    res23->esquerda = extrai_expressao(no->filhos[0]);
    res23->direita = extrai_expressao(no->filhos[2]);
    return res23;
  }
  case 24: { // 24) E -> T
    return extrai_expressao(no->filhos[0]);
  }
  case 25: { // 25) T -> T * F
    ExpressaoMultiplicacao* res25 = new ExpressaoMultiplicacao();
    res25->esquerda = extrai_expressao(no->filhos[0]);
    res25->direita = extrai_expressao(no->filhos[2]);
    return res25;
  }
  case 26: {// 26) T -> T / F
    ExpressaoDivisao* res26 = new ExpressaoDivisao();
    res26->esquerda = extrai_expressao(no->filhos[0]);
    res26->direita = extrai_expressao(no->filhos[2]);
    return res26;
  }
  case 27: { // 27) T -> T % F
    ExpressaoMod* res27 = new ExpressaoMod();
    res27->esquerda = extrai_expressao(no->filhos[0]);
    res27->direita = extrai_expressao(no->filhos[2]);
    return res27;
  }
  case 28: { // 28) T -> F
    return extrai_expressao(no->filhos[0]);
  }
  case 29: { // 29) F -> ( Ea )
    return extrai_expressao(no->filhos[1]);
  }
  case 30: { // 30) F -> ID
    ExpressaoVariavel* res30 = new ExpressaoVariavel();
    res30->nome = ID::extrai_ID(no->filhos[0]);
    return res30;
  }
  case 31: // 31) F -> DEC_VAL
  case 32: // 32) F -> FLOAT_VAL
  case 33: { // 33) F -> BOOL_VAL
    ExpressaoValor* res = new ExpressaoValor();
    res->valor = ValorLiteral::extrai_valor_literal(no->filhos[0]);
    return res;
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
