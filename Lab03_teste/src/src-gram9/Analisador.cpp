#include "Analisador.hpp"
#include <map>
#include <string>
#include <stdlib.h>
using namespace std;

int Analisador::calcula_retorno(Funcao *f, const vector<ValorLiteral> &params) {
  map<string, int*> T;
  T[string("a")] = (int*) malloc(sizeof(int));

  (*(T[string("a")])) = 10;
  // Preencher aqui.
  return 0;
}
