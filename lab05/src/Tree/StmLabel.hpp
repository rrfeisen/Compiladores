#ifndef _STM_LABEL_HPP_
#define _STM_LABEL_HPP_
#include "Stm.hpp"
#include<string>
using namespace std;

class StmLabel : public Stm {
publi:
  string nome;
  StmLabel();
  static StmLabel* gera_Stm_Label(/*Adaptar aas necessidades.*/);
};

#endif
