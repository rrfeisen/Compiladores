#ifndef _EXP_HPP_
#define _EXP_HPP_
#include "../src-gram5/Expressao.hpp"
using namespace std;

class Exp {
public:
  Exp();
  static Exp* gera_Exp_de_Expressao(Expressao* e);
};

#endif
