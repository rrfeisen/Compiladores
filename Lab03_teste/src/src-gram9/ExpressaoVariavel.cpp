#include "ExpressaoVariavel.hpp"
#include <iostream>
#include "../debug-util.hpp"

using namespace std;
ExpressaoVariavel::ExpressaoVariavel() { }

void ExpressaoVariavel::debug_com_tab(int tab) {
  tab3(tab);
  cerr << "VAR[" << nome->nome << "]" << endl;
}
