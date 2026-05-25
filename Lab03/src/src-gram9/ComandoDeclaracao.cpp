#include "ComandoDeclaracao.hpp"
#include <iostream>
#include "../debug-util.hpp"
using namespace std;

ComandoDeclaracao::ComandoDeclaracao() {
}

void ComandoDeclaracao::debug_com_tab(int tab) {
  tab3(tab);
  cerr << "[" << variavel->nome->nome << ":" << (variavel->tipo == NULL ? "TIPO_INVALIDO" : variavel->tipo->nome()) << "] Declaracao" << endl;
}
