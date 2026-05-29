#include "ComandoIf.hpp"
#include <iostream>
#include "../debug-util.hpp"

using namespace std;

ComandoIf::ComandoIf() : condicao(NULL), entao(NULL), senao(NULL) {}

void ComandoIf::debug_com_tab(int tab) {
  tab3(tab);
  cerr << "INI IF" << endl;
  condicao->debug_com_tab(tab+1);
  entao->debug_com_tab(tab+1);
  if (senao) {
      tab3(tab);
      cerr << "ELSE" << endl;
      senao->debug_com_tab(tab+1);
  }
  tab3(tab);
  cerr << "FIM IF" << endl;
}
