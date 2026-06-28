#ifndef _EXP_BINOP_HPP_
#define _EXP_BINOP_HPP_
#include "Exp.hpp"
#include "../src-gram5/Expressao.hpp"

class ExpBinop : public Exp {
public:
  Operador* op;
  Exp* esq;
  Exp* dir;  
  ExpBinop();
  static ExpBinop* gera_Exp_Binop(Expressao* e);
};

#endif
