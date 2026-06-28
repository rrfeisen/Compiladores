#include "ID.hpp"

ID* ID::extrai_ID(No_arv_parse* no) {
  ID* res = new ID();
  res->nome = no->dado_extra;
  return res;
}
