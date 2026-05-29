#include "Funcao.hpp"
#include "../debug-util.hpp"
#include <iostream>

Funcao::Funcao() {}

Funcao* Funcao::extrai_funcao(No_arv_parse *no) {
  if (no == NULL) return NULL;
  
  // CORREÇÃO: proc_decl é a regra 11 na gramatica-9.site
  if (no->regra == 11) {
    Funcao* res = new Funcao();
    res->tipo_retorno = new Tipo(Tipo::INT);
    res->nome_funcao = ID::extrai_ID(no->filhos[1]);
    res->parametros = Variavel::extrai_lista_parametros(no->filhos[3]);
    res->comandos = Comando::extrai_lista_comandos(no->filhos[6]);
    return res;
  }
  
  for (size_t i = 0; i < no->filhos.size(); i++) {
    Funcao* f = extrai_funcao(no->filhos[i]);
    if (f != NULL) return f;
  }
  
  return NULL;
}

void debug_comandos(const vector<Comando*> &coms, int tab) {
  for (size_t ic = 0; ic < coms.size(); ++ic) {
    if (coms[ic]) coms[ic]->debug_com_tab(tab+1);
  }  
}

void Funcao::debug() {
  cerr << "Funcao:[nome=" << (nome_funcao ? nome_funcao->nome : "NULL") << "]" << endl;
  cerr << "{" << endl;
  debug_comandos(comandos, 1);
  cerr << "}" << endl;
}
