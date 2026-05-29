#ifndef _VARIAVEL_HPP_
#define _VARIAVEL_HPP_
#include "ID.hpp"
#include "Tipo.hpp"
#include "../Arvore.hpp"
#include <vector>
using namespace std;

class Variavel {
public:
  Tipo* tipo;
  ID* nome;
  static vector<Variavel*> extrai_lista_parametros(No_arv_parse* no);
  static vector<Variavel*> extrai_lista_nao_vazia_parametros(No_arv_parse* no);
  static Variavel* extrai_variavel_P(No_arv_parse* no);
  void debug_com_tab(int tab);
};

#endif
