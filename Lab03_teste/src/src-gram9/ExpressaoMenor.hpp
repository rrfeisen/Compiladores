#ifndef _EXPRESSAO_MENOR_HPP_
#define _EXPRESSAO_MENOR_HPP_
#include "Expressao.hpp"
using namespace std;

class ExpressaoMenor : public Expressao {
public:
  Expressao* esquerda;
  Expressao* direita;
  ExpressaoMenor();
  void debug_com_tab(int tab);
};

#endif
