#include "ComandoWhile.hpp"
#include <iostream>
#include "../debug-util.hpp"

using namespace std;

ComandoWhile::ComandoWhile() : condicao(NULL), corpo(NULL) {}

void ComandoWhile::debug_com_tab(int tab) {
  tab3(tab);
  cerr << "INI WHILE" << endl;
  condicao->debug_com_tab(tab+1);
  corpo->debug_com_tab(tab+1);
  tab3(tab);
  cerr << "FIM WHILE" << endl;
}
