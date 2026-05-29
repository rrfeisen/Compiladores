#include "Variavel.hpp"
#include <iostream>
#include "../debug-util.hpp"

using namespace std;

vector<Variavel*> Variavel::extrai_lista_nao_vazia_parametros(No_arv_parse* no) {
  vector<Variavel*> res;
  if (no == NULL) return res;

  if (no->simb == "id_list") {
      for (auto filho : no->filhos) {
          if (filho->simb == "id_list") {
              vector<Variavel*> sub = extrai_lista_nao_vazia_parametros(filho);
              res.insert(res.end(), sub.begin(), sub.end());
          }
          if (filho->simb == "ID") {
              Variavel* v = new Variavel();
              v->tipo = new Tipo(Tipo::INT);
              v->nome = ID::extrai_ID(filho);
              res.push_back(v);
          }
      }
  }
  return res;
}

vector<Variavel*> Variavel::extrai_lista_parametros(No_arv_parse* no) {
  vector<Variavel*> res;
  if (no == NULL) return res;
  
  if (no->simb == "param_list") {
      for (auto filho : no->filhos) {
          if (filho->simb == "id_list") return extrai_lista_nao_vazia_parametros(filho);
      }
  }
  return res;
}

Variavel* Variavel::extrai_variavel_P(No_arv_parse* no) {
  return NULL;
}

void Variavel::debug_com_tab(int tab) {
  tab3(tab);
  cerr << "[" << (nome ? nome->nome : "NULL") << "] Variavel" << endl;
}
