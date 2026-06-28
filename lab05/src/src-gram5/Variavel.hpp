#ifndef _VARIAVEL_HPP_
#define _VARIAVEL_HPP_
#include "ID.hpp"
#include "../Arvore.hpp"
#include "../Frame/FrameAcesso.hpp"
using namespace std;

class Variavel {
public:
  ID* tipo;
  ID* nome;
  FrameAcesso* acesso_frame;
  static vector<Variavel*> extrai_lista_parametros(No_arv_parse* no);
  static vector<Variavel*> extrai_lista_nao_vazia_parametros(No_arv_parse* no);
  static vector<Variavel*> extrai_lista_variaveis(No_arv_parse* no);
  static Variavel* extrai_variavel_P(No_arv_parse* no);
  void debug_com_tab(int tab);
};

#endif
