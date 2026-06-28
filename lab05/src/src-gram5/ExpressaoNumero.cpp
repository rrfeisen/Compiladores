#include "ExpressaoNumero.hpp"
#include <iostream>
#include "../debug-util.hpp"

using namespace std;

ExpressaoNumero::ExpressaoNumero() { }

void ExpressaoNumero::debug_com_tab(int tab) {
  tab3(tab);
  cerr << numero->valor << " [Numero]"<< endl;
}
