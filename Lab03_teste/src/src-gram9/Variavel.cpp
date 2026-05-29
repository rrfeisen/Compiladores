#include "Variavel.hpp"
#include <iostream>
#include "../debug-util.hpp"

using namespace std;

vector<Variavel*> Variavel::extrai_lista_nao_vazia_parametros(No_arv_parse* no) {
  // LPx. 4) LPx -> P, 5) LPx -> P , LPx
  vector<Variavel*> res;
  res.push_back(extrai_variavel_P(no->filhos[0]));
  if (no->regra == 4) return res;
  // Tem que ser regra 5. LPx-> P , LPx
  vector<Variavel*> restante = extrai_lista_nao_vazia_parametros(no->filhos[2]);
  res.insert(res.end(), restante.begin(), restante.end());
  return res;
}

vector<Variavel *> Variavel::extrai_lista_parametros(No_arv_parse* no) {
  // LP
  if (no->regra == 2) return vector<Variavel*>();
  // Tem que ser regra 3, com unico filho.
  return extrai_lista_nao_vazia_parametros(no->filhos[0]);
}

Variavel* Variavel::extrai_variavel_P(No_arv_parse* no) {
  Variavel* res = new Variavel();
  // 8) P -> Tipo ID
  res->tipo = Tipo::extrai_Tipo(no->filhos[0]);
  res->nome = ID::extrai_ID(no->filhos[1]);
  return res;
}

void Variavel::debug_com_tab(int tab) {
  tab3(tab);
  cerr << "[" << nome->nome << ":" <<  (tipo == NULL ? "TIPO_INVALIDO" : tipo->nome()) << "] Variavel Declarada" << endl;
}
