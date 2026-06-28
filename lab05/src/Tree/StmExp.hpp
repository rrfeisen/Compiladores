#ifndef _STM_EXP_HPP_
#define _STM_EXP_HPP_
#include "Stm.hpp"
#include "Exp.hpp"

class StmExp : public Stm {
publi:
  Exp* exp;
  StmExp();
  static StmExp* gera_Stm_Exp(/*Adaptar aas necessidades.*/);
};

#endif
