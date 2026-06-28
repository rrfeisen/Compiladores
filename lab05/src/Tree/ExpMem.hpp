#ifndef _EXP_MEM_HPP_
#define _EXP_MEM_HPP_
#include "Exp.hpp"
#include "../src-gram5/Expressao.hpp"

class ExpMem : public Exp {
public:
  Exp* endereco;
  ExpMem();
  static ExpMem* gera_Exp_Mem(Expressao* e);
};

#endif
