#ifndef _EXP_TEMPFRAMEPOINTER_HPP_
#define _EXP_TEMPFRAMEPOINTER_HPP_
#include "Exp.hpp"
#include "ExpTemp.hpp"
#include "../src-gram5/Expressao.hpp"

class ExpTempFramePointer : public ExpTemp {
public:
  static ExpTempFramePointer* gera_Exp_TempFramePointer(/*Adaptar aas necessidades*/);
};

#endif
