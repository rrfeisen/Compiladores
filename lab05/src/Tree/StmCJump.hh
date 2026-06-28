#ifndef _STM_CJUMP_HPP_
#define _STM_CJUMP_HPP_
#include "Stm.hpp"
#include "Exp.hpp"
#include<string>
using namespace std;

class StmCJump : public Stm {
publi:
  Exp* expressao_booleana;
  string destino_verdade;
  string destino_falso;
  StmCJump();
  static StmCJump* gera_Stm_CJump(/*Adaptar aas necessidades.*/);
};

#endif
