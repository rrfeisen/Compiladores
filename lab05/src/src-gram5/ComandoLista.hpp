#ifndef _COMANDO_LISTA_HPP_
#define _COMANDO_LISTA_HPP_
#include "Comando.hpp"
#include "Variavel.hpp"
#include <vector>
using namespace std;

class ComandoLista : public Comando {
public:
  vector<Variavel*> lista_variaveis;
  vector<Comando*> lista_comandos;
  ComandoLista();
  void debug_com_tab(int tab);
};

#endif
