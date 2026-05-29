#ifndef _COMANDO_DECLARACAO_HPP_
#define _COMANDO_DECLARACAO_HPP_
#include "Comando.hpp"
#include "Variavel.hpp"
using namespace std;

class ComandoDeclaracao : public Comando {
public:
  Variavel* variavel;
  ComandoDeclaracao();
  void debug_com_tab(int tab);
};

#endif
