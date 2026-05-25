#include "ComandoLista.hpp"
#include <iostream>
#include "../debug-util.hpp"
using namespace std;

ComandoLista::ComandoLista(){ }

void ComandoLista::debug_com_tab(int tab) {
  tab3(tab);
  cerr << "{ INI Lista Comandos" << endl;
  for (int ic = 0; ic < lista_comandos.size(); ++ic) {
    lista_comandos[ic]->debug_com_tab(tab+1);
  }
  tab3(tab);
  cerr << "} FIM Lista Comandos" << endl;
}
