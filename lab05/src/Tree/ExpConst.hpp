#ifndef _EXP_CONST_HPP_
#define _EXP_CONST_HPP_
#include "Exp.hpp"
#include "../src-gram5/ExpressaoNumero.hpp"

class ExpConst : public Exp {
public:
  int valor;
  ExpConst();
  static ExpConst* gera_Exp_Const(ExpressaoNumero* e);
};

#endif
