#include "Variavel.hpp"
#include <iostream>
#include "../debug-util.hpp"

using namespace std;

vector<Variavel*> Variavel::extrai_lista_nao_vazia_parametros(No_arv_parse* no) {
  vector<Variavel*> res;
  if (no == NULL) return res;

  if (no->regra == 10) { // CORREÇÃO: id_list -> ID (Regra 10)
    Variavel* v = new Variavel();
    v->tipo = new Tipo(Tipo::INT);
    v->nome = ID::extrai_ID(no->filhos[0]);
    res.push_back(v);
  } 
  else if (no->regra == 9) { // CORREÇÃO: id_list -> id_list COMMA ID (Regra 9)
    res = extrai_lista_nao_vazia_parametros(no->filhos[0]);
    Variavel* v = new Variavel();
    v->tipo = new Tipo(Tipo::INT);
    v->nome = ID::extrai_ID(no->filhos[2]);
    res.push_back(v);
  }
  return res;
}

vector<Variavel*> Variavel::extrai_lista_parametros(No_arv_parse* no) {
  vector<Variavel*> res;
  if (no == NULL) return res;
  
  if (no->regra == 7) { // CORREÇÃO: param_list -> id_list (Regra 7)
    return extrai_lista_nao_vazia_parametros(no->filhos[0]);
  }
  // Se for a regra 8 (param_list -> ''), retorna lista vazia
  return res;
}

Variavel* Variavel::extrai_variavel_P(No_arv_parse* no) {
  return NULL;
}

void Variavel::debug_com_tab(int tab) {
  tab3(tab);
  cerr << "[" << (nome ? nome->nome : "NULL") << "] Variavel" << endl;
}
