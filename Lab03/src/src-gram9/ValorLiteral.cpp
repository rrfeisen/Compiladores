#include "ValorLiteral.hpp"
#include <sstream>
#include <iomanip>
#include <stdlib.h>

ValorLiteral::ValorLiteral() {
  tipo        = NULL;
  valor_int   = 0;
  valor_float = 0.0f;
  valor_bool  = false;
}

ValorLiteral::ValorLiteral(int v) {
  tipo        = new Tipo(Tipo::INT);
  valor_int   = v;
  valor_float = 0.0f;
  valor_bool  = false;
}

ValorLiteral::ValorLiteral(float v) {
  tipo        = new Tipo(Tipo::FLOAT);
  valor_int   = 0;
  valor_float = v;
  valor_bool  = false;
}

ValorLiteral::ValorLiteral(bool v) {
  tipo        = new Tipo(Tipo::BOOL);
  valor_int   = 0;
  valor_float = 0.0f;
  valor_bool  = v;
}

string ValorLiteral::como_string() const {
  if (tipo == NULL) return "nil";
  stringstream ss;
  switch(tipo->valor) {
  case Tipo::INT:
    ss << valor_int;
    break;
  case Tipo::FLOAT:
    ss << fixed << setprecision(2) << valor_float;
    break;
  case Tipo::BOOL:
    ss << (valor_bool ? "true" : "false");
    break;
  }
  return ss.str();
}

ValorLiteral* ValorLiteral::extrai_valor_literal(No_arv_parse* no) {
  if (no == NULL) return NULL;
  ValorLiteral* res = new ValorLiteral();
  res->tipo = Tipo::extrai_tipo_literal(no);
  if (res->tipo == NULL) return res;

  switch(res->tipo->valor) {
  case Tipo::INT:
    res->valor_int = atoi(no->dado_extra.c_str());
    break;
  case Tipo::FLOAT:
    res->valor_float = (float)atof(no->dado_extra.c_str());
    break;
  default:
    break;
  }
  return res;
}
