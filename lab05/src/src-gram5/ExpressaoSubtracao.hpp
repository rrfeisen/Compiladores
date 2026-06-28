#ifndef _EXPRESSAO_SUBTRACAO_HPP_
#define _EXPRESSAO_SUBTRACAO_HPP_
#include "Expressao.hpp"
using namespace std;

class ExpressaoSubtracao : public Expressao {
public:
  Expressao* esquerda;
  Expressao* direita;
  ExpressaoSubtracao();
  void debug_com_tab(int tab);
};

#endif
