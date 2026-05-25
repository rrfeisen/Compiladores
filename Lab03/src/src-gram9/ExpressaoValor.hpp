#ifndef _EXPRESSAO_VALOR_HPP_
#define _EXPRESSAO_VALOR_HPP_
#include "Expressao.hpp"
#include "ValorLiteral.hpp"
using namespace std;

class ExpressaoValor : public Expressao {
public:
  ValorLiteral* valor;
  ExpressaoValor();
  void debug_com_tab(int tab);
};

#endif
