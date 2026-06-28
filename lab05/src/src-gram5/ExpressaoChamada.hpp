#ifndef _EXPRESSAO_CHAMADA_HPP_
#define _EXPRESSAO_CHAMADA_HPP_
#include "Expressao.hpp"
#include "Variavel.hpp"
#include "ID.hpp"
#include "../Arvore.hpp"
#include <vector>
using namespace std;

class ExpressaoChamada : public Expressao {
public:
  ID* nome_funcao;
  vector<Expressao *> lista_parametros; 
  ExpressaoChamada();
  void debug_com_tab(int tab);
  static vector<Expressao*> extrai_lista_expressoes_parametro(No_arv_parse* no);
private:
  static vector<Expressao*> extrai_lista_expressoes_parametro_nao_vazia(No_arv_parse* no);
};

#endif
