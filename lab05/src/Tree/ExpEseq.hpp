#ifndef _EXP_ESEQ_HPP_
#define _EXP_ESEQ_HPP_
#include "Exp.hpp"
#include "../src-gram5/Expressao.hpp"

class ExpEseq : public Exp {
public:
  Stm* stm;
  Exp* exp;
  ExpEseq();
  static ExpEseq* gera_Exp_Eseq(/*Adaptar aas necessidades*/);
};

#endif
