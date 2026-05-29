#include "ComandoRetorno.hpp"
#include <iostream>
#include "../debug-util.hpp"
using namespace std;

ComandoRetorno::ComandoRetorno() {
}

void ComandoRetorno::debug_com_tab(int tab) {
  tab3(tab);
  cerr << "INI RETURN" << endl;
  expressao->debug_com_tab(tab+1);
  tab3(tab);
  cerr << "FIM RETURN" << endl;
}
