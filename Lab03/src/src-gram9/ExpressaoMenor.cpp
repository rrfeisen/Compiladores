#include "ExpressaoMenor.hpp"
#include <iostream>
#include "../debug-util.hpp"

using namespace std;

ExpressaoMenor::ExpressaoMenor() { }

void ExpressaoMenor::debug_com_tab(int tab) {
  tab3(tab);
  cerr << "< [ INI Menor"<< endl;
  esquerda->debug_com_tab(tab+1);
  tab3(tab);
  cerr << "<" << endl;
  direita->debug_com_tab(tab+1);
  tab3(tab);
  cerr << "< ] FIM Menor"<< endl;
}
