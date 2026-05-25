#include "ExpressaoMod.hpp"
#include <iostream>
#include "../debug-util.hpp"

using namespace std;
ExpressaoMod::ExpressaoMod() { }

void ExpressaoMod::debug_com_tab(int tab) {
  tab3(tab);
  cerr << "% [ INI Mod"<< endl;
  esquerda->debug_com_tab(tab+1);
  tab3(tab);
  cerr << "%" << endl;
  direita->debug_com_tab(tab+1);
  tab3(tab);
  cerr << "% ] FIM Mod"<< endl;
}
