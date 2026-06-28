#include "ComandoAtribuicao.hpp"
#include <iostream>
#include "../debug-util.hpp"
using namespace std;

ComandoAtribuicao::ComandoAtribuicao() {
  
}

void ComandoAtribuicao::debug_com_tab(int tab) {
  tab3(tab);
  cerr << "INI ATRIB: " << esquerda->nome << " = " << endl;
  direita->debug_com_tab(tab+1);
  tab3(tab);
  cerr << "FIM ATRIB" << endl;
  cerr << endl;
}
