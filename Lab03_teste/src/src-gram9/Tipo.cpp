#include "Tipo.hpp"

Tipo::Tipo(Valor valor) {
  this->valor = valor;
}

string Tipo::nome() const {
  switch(valor) {
  case INT:   return "INT";
  case FLOAT: return "FLOAT";
  case BOOL:  return "BOOL";
  }
  return "?";
}

Tipo* Tipo::extrai_tipo_literal(No_arv_parse* no) {
  if (no == NULL) return NULL;
  if (no->simb == "INT_LIT")    return new Tipo(INT);
  if (no->simb == "REAL_LIT")   return new Tipo(FLOAT);
  // sem BOOL_LIT na gramática 9 — booleanos vêm de expressões relacionais
  return NULL;
}
