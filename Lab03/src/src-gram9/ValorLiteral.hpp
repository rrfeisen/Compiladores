#ifndef _VALOR_LITERAL_HPP_
#define _VALOR_LITERAL_HPP_
#include "Tipo.hpp"
#include "../Arvore.hpp"
#include <string>
using namespace std;

class ValorLiteral {
public:
  Tipo* tipo;
  int valor_int;
  float valor_float;
  bool valor_bool;

  ValorLiteral();
  string como_string() const;
  static ValorLiteral* extrai_valor_literal(No_arv_parse* no);
};

#endif
