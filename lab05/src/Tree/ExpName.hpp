#ifndef _EXP_NAME_HPP_
#define _EXP_NAME_HPP_
#include "Exp.hpp"
#include "../src-gram5/Expressao.hpp"

class ExpName : public Exp {
public:
  string nome;
  static ExpName* gera_Exp_Name(/*Adaptar aas necessidades*/);
};

#endif
