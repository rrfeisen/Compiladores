#ifndef _STM_JUMP_HPP_
#define _STM_JUMP_HPP_
#include "Stm.hpp"
#include "Exp.hpp"
#include<vector>
#include<string>
using namespace std;

class StmJump : public Stm {
publi:
  Exp* destino;
  vector<string> alvos;
  StmJump();
  static StmJump* gera_Stm_Jump(/*Adaptar aas necessidades.*/);
};

#endif
