#ifndef _EXPRESSAO_IGUALDADE_HPP_
#define _EXPRESSAO_IGUALDADE_HPP_
#include "Expressao.hpp"
using namespace std;

class ExpressaoIgualdade : public Expressao {
public:
  Expressao* esquerda;
  Expressao* direita;
  ExpressaoIgualdade();
  void debug_com_tab(int tab);
};

#endif
