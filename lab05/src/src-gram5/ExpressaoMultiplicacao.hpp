#ifndef _EXPRESSAO_MULTIPLICACAO_HPP_
#define _EXPRESSAO_MULTIPLICACAO_HPP_
#include "Expressao.hpp"
using namespace std;

class ExpressaoMultiplicacao : public Expressao {
public:
  Expressao* esquerda;
  Expressao* direita;
  ExpressaoMultiplicacao();
  void debug_com_tab(int tab);
};

#endif
