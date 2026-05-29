#include "Comando.hpp"
#include "ComandoAtribuicao.hpp"
#include "ComandoDeclaracao.hpp"
#include "ComandoLista.hpp"
#include "ComandoRetorno.hpp"
#include "ComandoIf.hpp"
#include "ComandoWhile.hpp"
#include "ID.hpp"
#include "Variavel.hpp"
#include <iostream>
#include "../debug-util.hpp"

vector<Comando*> Comando::extrai_lista_comandos(No_arv_parse* no) {
  if (no->regra == 13) return vector<Comando*>(); // stmt_list -> stmt
  vector<Comando*> res;
  if (no->filhos.size() > 0) {
      res.push_back(extrai_comando(no->filhos[0]));
  }
  if (no->filhos.size() > 1) {
      vector<Comando*> restante = extrai_lista_comandos(no->filhos[1]);
      res.insert(res.end(), restante.begin(), restante.end());
  }
  return res;
}

Comando* Comando::extrai_comando(No_arv_parse* no) {
  if (no == NULL) return NULL;
  
  // Repasse de stmt -> if_stmt, etc (Regras 14 a 17)
  if (no->regra >= 14 && no->regra <= 17) {
      return extrai_comando(no->filhos[0]);
  }

  // IF: Regras 18, 19, 20
  if (no->regra >= 18 && no->regra <= 20) {
    ComandoIf* res = new ComandoIf();
    res->condicao = Expressao::extrai_expressao(no->filhos[1]);
    res->entao = extrai_comando(no->filhos[3]);
    if (no->regra == 19 || no->regra == 20) res->senao = extrai_comando(no->filhos[5]);
    return res;
  }
  
  // WHILE: Regra 21
  if (no->regra == 21) {
    ComandoWhile* res = new ComandoWhile();
    res->condicao = Expressao::extrai_expressao(no->filhos[1]);
    res->corpo = extrai_comando(no->filhos[3]);
    return res;
  }
  
  // RETURN: Regra 22
  if (no->regra == 22) {
    ComandoRetorno* res = new ComandoRetorno();
    res->expressao = Expressao::extrai_expressao(no->filhos[1]);
    return res;
  }
  
  // ATRIBUICAO: Regra 28, 29, 30
  if (no->regra >= 28 && no->regra <= 30) {
    ComandoAtribuicao* res = new ComandoAtribuicao();

    // res->esquerda = ... 
    // res->direita = Expressao::extrai_expressao(no->filhos[2]);
    return res;
  }
  
  // BLOCO: Regra 23, 24
  if (no->regra == 23 || no->regra == 24) {
    ComandoLista* res = new ComandoLista();
    if (no->regra == 23) res->lista_comandos = extrai_lista_comandos(no->filhos[1]);
    return res;
  }

  return NULL;
}

void Comando::debug_com_tab(int tab) {
  tab3(tab);
  cerr << "Comando generico"<< endl;
}
