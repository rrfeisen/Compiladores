#include "ExpressaoMultiplicacao.hpp"
#include <iostream>
#include "../debug-util.hpp"

using namespace std;
ExpressaoMultiplicacao::ExpressaoMultiplicacao() { }

void ExpressaoMultiplicacao::debug_com_tab(int tab) {
  tab3(tab);
  cerr << "* [ INI Multiplicacao"<< endl;
  esquerda->debug_com_tab(tab+1);
  tab3(tab);
  cerr << "*" << endl;
  direita->debug_com_tab(tab+1);
  tab3(tab);
  cerr << "* ] FIM Multiplicao"<< endl;
}
