#include "Comando.hpp"
#include "ComandoAtribuicao.hpp"
#include "ComandoDeclaracao.hpp"
#include "ComandoLista.hpp"
#include "ComandoRetorno.hpp"
#include "ID.hpp"
#include "Variavel.hpp"
#include <iostream>
#include "../debug-util.hpp"

vector<Comando*> Comando::extrai_lista_comandos(No_arv_parse* no) {
  // LC. 6) LC ->  7) LC -> C LC
  if (no->regra == 6) return vector<Comando*>();
  vector<Comando*> res;
  res.push_back(extrai_comando(no->filhos[0]));
  vector<Comando*> restante = extrai_lista_comandos(no->filhos[1]);
  res.insert(res.end(), restante.begin(), restante.end());
  return res;
}

Comando* Comando::extrai_comando(No_arv_parse* no) {
  // C. 9) C -> P ;  10) C -> ID = Ea ;  11) C -> { LC }  12) C -> RETURN Ea ;
  if (no->regra == 9) {
    ComandoDeclaracao* res9 = new ComandoDeclaracao();
    res9->variavel = Variavel::extrai_variavel_P(no->filhos[0]);
    return res9;
  } else if (no->regra == 10) {
    ComandoAtribuicao* res10 = new ComandoAtribuicao();
    res10->esquerda = ID::extrai_ID(no->filhos[0]);
    res10->direita = Expressao::extrai_expressao(no->filhos[2]);
    return res10;
  } else if (no->regra == 11) {
    ComandoLista* res11 = new ComandoLista();
    res11->lista_comandos = extrai_lista_comandos(no->filhos[1]);
    return res11;
  } else if (no->regra == 12) {
    ComandoRetorno* res12 = new ComandoRetorno();
    res12->expressao = Expressao::extrai_expressao(no->filhos[1]);
    return res12;
  }else {
    return NULL;
  }		      
}

void Comando::debug_com_tab(int tab) {
  tab3(tab);
  cerr << "Comando generico"<< endl;
}
