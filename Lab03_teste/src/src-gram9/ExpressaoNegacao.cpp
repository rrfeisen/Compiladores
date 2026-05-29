#include "ExpressaoNegacao.hpp"
#include <iostream>
#include "../debug-util.hpp"

using namespace std;

ExpressaoNegacao::ExpressaoNegacao() { }

void ExpressaoNegacao::debug_com_tab(int tab) {
  tab3(tab);
  cerr << "! [ INI Negacao"<< endl;
  expressao->debug_com_tab(tab+1);
  tab3(tab);
  cerr << "! ] FIM Negacao"<< endl;
}
