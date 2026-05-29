#ifndef _COMANDO_RETORNO_HPP_
#define _COMANDO_RETORNO_HPP_
#include "Comando.hpp"
#include "Expressao.hpp"
using namespace std;

class ComandoRetorno : public Comando {
public:
  Expressao * expressao;
  ComandoRetorno();
  void debug_com_tab(int tab);
};

#endif
