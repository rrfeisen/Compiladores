#include "ExpressaoSubtracao.hpp"
#include <iostream>
#include "../debug-util.hpp"

using namespace std;
ExpressaoSubtracao::ExpressaoSubtracao() { }

void ExpressaoSubtracao::debug_com_tab(int tab) {
  tab3(tab);
  cerr << "- [ INI Subtracao"<< endl;
  esquerda->debug_com_tab(tab+1);
  tab3(tab);
  cerr << "-" << endl;
  direita->debug_com_tab(tab+1);
  tab3(tab);
  cerr << "- ] FIM Subtracao"<< endl;
}
