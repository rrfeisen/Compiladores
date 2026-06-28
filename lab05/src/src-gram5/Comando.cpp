#include "Comando.hpp"
#include "ComandoAtribuicao.hpp"
#include "ComandoLista.hpp"
#include "ID.hpp"
#include "Variavel.hpp"
#include <iostream>
#include "../debug-util.hpp"

vector<Comando*> Comando::extrai_lista_comandos(No_arv_parse* no) {
  // LC. 8) LC ->  9) LC -> C LC
  if (no->regra == 8) return vector<Comando*>();
  vector<Comando*> res;
  res.push_back(extrai_comando(no->filhos[0]));
  vector<Comando*> restante = extrai_lista_comandos(no->filhos[1]);
  res.insert(res.end(), restante.begin(), restante.end());
  return res;
}

Comando* Comando::extrai_comando(No_arv_parse* no) {
  // C. 12) C -> ID = E ; 13) C -> { LV LC }
  if (no->regra == 12) { //12) C -> ID = E ;                                 
    ComandoAtribuicao* res12 = new ComandoAtribuicao();
    res12->esquerda = ID::extrai_ID(no->filhos[0]);
    res12->direita = Expressao::extrai_expressao(no->filhos[2]);
    return res12;
  } else if (no->regra == 13) { // 13) C -> { LV LC }
    ComandoLista* res13 = new ComandoLista();
    res13->lista_variaveis = Variavel::extrai_lista_variaveis(no->filhos[1]);
    res13->lista_comandos = extrai_lista_comandos(no->filhos[2]);
    return res13;
  }else {
    return NULL;
  }		      
}

void Comando::debug_com_tab(int tab) {
  tab3(tab);
  cerr << "Comando generico"<< endl;
}
