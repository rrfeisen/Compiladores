#ifndef _EXP_TEMP_HPP_
#define _EXP_TEMP_HPP_
#include "Exp.hpp"
#include "../src-gram5/Expressao.hpp"

class ExpTemp : public Exp {
public:
  int id;
  static ExpTemp* gera_Exp_Temp(/*Adaptar aas necessidades*/);
};

#endif
