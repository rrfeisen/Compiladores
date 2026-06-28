#ifndef _STM_MOVE_HPP_
#define _STM_MOVE_HPP_
#include "Stm.hpp"
#include "Exp.hpp"

class StmMove : public Stm {
publi:
  Exp* destino;
  Exp* origem;
  StmMove();
  static StmMove* gera_Stm_Move(/*Adaptar aas necessidades.*/);
};

#endif
