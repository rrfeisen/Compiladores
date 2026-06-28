#ifndef _EXP_CALL_HPP_
#define _EXP_CALL_HPP_
#include "Exp.hpp"
#include "../src-gram5/Expressao.hpp"
#include<vector>
using namespace std;

class ExpCall : public Exp {
public:
  Exp* nome_funcao;
  vector<Exp*> parametros;
  static ExpCall* gera_Exp_Call(/*Adaptar aas necessidades*/);
};

#endif
