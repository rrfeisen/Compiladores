#include "Variavel.hpp"
#include <iostream>
#include "../debug-util.hpp"

using namespace std;

vector<Variavel*> Variavel::extrai_lista_nao_vazia_parametros(No_arv_parse* no) {
  vector<Variavel*> res;
  if (!no) return res;
  
  if (no->regra == 8) { // id_list -> ID
      Variavel* v = new Variavel();
      v->tipo = new Tipo(Tipo::INT);
      v->nome = ID::extrai_ID(no->filhos[0]);
      res.push_back(v);
  } 
  else if (no->regra == 7) { // id_list -> id_list COMMA ID
      res = extrai_lista_nao_vazia_parametros(no->filhos[0]);
      Variavel* v = new Variavel();
      v->tipo = new Tipo(Tipo::INT);
      v->nome = ID::extrai_ID(no->filhos[2]);
      res.push_back(v);
  }
  return res;
}

vector<Variavel *> Variavel::extrai_lista_parametros(No_arv_parse* no) {
  if (!no || no->regra == 6) return vector<Variavel*>(); // Vazio
  if (no->regra == 5) return extrai_lista_nao_vazia_parametros(no->filhos[0]);
  return vector<Variavel*>();
}

Variavel* Variavel::extrai_variavel_P(No_arv_parse* no) {
  return NULL; 
}

void Variavel::debug_com_tab(int tab) {
  tab3(tab);
  cerr << "[" << nome->nome << "] Variavel Declarada" << endl;
}
