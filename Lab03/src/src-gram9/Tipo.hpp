#ifndef _TIPO_HPP_
#define _TIPO_HPP_
#include "../Arvore.hpp"
#include <string>
using namespace std;

class Tipo {
public:
  enum Valor {
    INT,
    FLOAT,
    BOOL
  };

  Valor valor;

  Tipo(Valor valor);
  string nome() const;

  // Extrai tipo a partir de um nó terminal (INT_LIT, REAL_LIT, etc.)
  static Tipo* extrai_tipo_literal(No_arv_parse* no);
};

#endif
