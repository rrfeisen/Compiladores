#include "ValorLiteral.hpp"
#include <algorithm>
#include <stdlib.h>
#include <sstream>

ValorLiteral::ValorLiteral() {
  tipo = NULL;
  valor_int = 0;
  valor_float = 0.0f;
  valor_bool = false;
}

string ValorLiteral::como_string() const {
  if (tipo == NULL) return "";
  stringstream ss;
  switch(tipo->valor) {
  case Tipo::INT: ss << valor_int; break;
  case Tipo::FLOAT: ss << valor_float; break;
  case Tipo::BOOL: ss << (valor_bool ? "true" : "false"); break;
  }
  return ss.str();
}

ValorLiteral* ValorLiteral::extrai_valor_literal(No_arv_parse* no) {
  if (no == NULL) return NULL;
  ValorLiteral* res = new ValorLiteral();
  
  res->tipo = new Tipo(Tipo::INT);
  if (no->simb == "INT_LIT") {
      res->valor_int = atoi(no->dado_extra.c_str());
  } else if (no->simb == "REAL_LIT") {
      res->tipo->valor = Tipo::FLOAT;
      res->valor_float = atof(no->dado_extra.c_str());
  } else if (no->simb == "KW_TRUE") {
      res->tipo->valor = Tipo::BOOL;
      res->valor_bool = true;
  } else if (no->simb == "KW_FALSE") {
      res->tipo->valor = Tipo::BOOL;
      res->valor_bool = false;
  }
  
  return res;
}
