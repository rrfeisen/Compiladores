#ifndef _EXPRESSAO_NUMERO_HPP_
#define _EXPRESSAO_NUMERO_HPP_
#include "Expressao.hpp"
#include "NUM.hpp"
using namespace std;

class ExpressaoNumero : public Expressao {
public:
  NUM* numero;
  ExpressaoNumero();
  void debug_com_tab(int tab);
};

#endif
