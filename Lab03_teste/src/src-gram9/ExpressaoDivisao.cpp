#include "ExpressaoDivisao.hpp"
#include <iostream>
#include "../debug-util.hpp"

using namespace std;

ExpressaoDivisao::ExpressaoDivisao() { }

void ExpressaoDivisao::debug_com_tab(int tab) {
  tab3(tab);
  cerr << "/ [ INI Divisao"<< endl;
  esquerda->debug_com_tab(tab+1);
  tab3(tab);
  cerr << "/" << endl;
  direita->debug_com_tab(tab+1);
  tab3(tab);
  cerr << "/ ] FIM Divisao"<< endl;
}
