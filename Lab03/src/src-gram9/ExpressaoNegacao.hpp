#ifndef _EXPRESSAO_NEGACAO_HPP_
#define _EXPRESSAO_NEGACAO_HPP_
#include "Expressao.hpp"
using namespace std;

class ExpressaoNegacao : public Expressao {
public:
  Expressao* expressao;
  ExpressaoNegacao();
  void debug_com_tab(int tab);
};

#endif
