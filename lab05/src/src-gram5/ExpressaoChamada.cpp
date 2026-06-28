#include "Expressao.hpp"
#include "ExpressaoChamada.hpp"
#include <iostream>
#include <vector>
#include "../debug-util.hpp"

using namespace std;
ExpressaoChamada::ExpressaoChamada() { }

vector<Expressao*> ExpressaoChamada::extrai_lista_expressoes_parametro_nao_vazia
(No_arv_parse* no) {
  vector<Expressao*> res;
  res.push_back(Expressao::extrai_expressao(no->filhos[0]));
  if (no->regra == 27) return res; // LEX -> E
  // Senao sera regra LEX -> E , LEX
  vector<Expressao*> restante = extrai_lista_expressoes_parametro_nao_vazia(no->filhos[2]);
  res.insert(res.end(), restante.begin(), restante.end());
  return res;
}

vector<Expressao*> ExpressaoChamada::extrai_lista_expressoes_parametro
(No_arv_parse* no) {
  switch(no->regra) { //LE
  case 25: { // 25) LE -> 
    return vector<Expressao*>();
  }
  case 26: { // 26) LE -> LEX
    return extrai_lista_expressoes_parametro_nao_vazia(no->filhos[0]);
  }
  default: {
    return vector<Expressao*>();
  } 
  }
}

void ExpressaoChamada::debug_com_tab(int tab) {
  tab3(tab);
  cerr << "CHAMADA [" << nome_funcao->nome << "(" << endl;
  for (int i = 0; i < lista_parametros.size(); ++i) {
    lista_parametros[i]->debug_com_tab(tab+1);
  }
  tab3(tab);
  cerr << "CHAMADA ])" << endl;
}
