#include "Comando.hpp"
#include "ComandoAtribuicao.hpp"
#include "ComandoDeclaracao.hpp"
#include "ComandoLista.hpp"
#include "ComandoRetorno.hpp"
#include "ComandoIf.hpp"
#include "ComandoWhile.hpp"
#include "ExpressaoVariavel.hpp"
#include "ID.hpp"
#include "Variavel.hpp"
#include <iostream>
#include "../debug-util.hpp"

vector<Comando*> Comando::extrai_lista_comandos(No_arv_parse* no) {
  vector<Comando*> res;
  if (!no) return res;

  if (no->regra == 13) { // stmt_list -> stmt
      Comando* c = extrai_comando(no->filhos[0]);
      if (c) res.push_back(c);
  } 
  else if (no->regra == 12) { // stmt_list -> stmt_list stmt
      res = extrai_lista_comandos(no->filhos[0]);
      Comando* c = extrai_comando(no->filhos[1]);
      if (c) res.push_back(c);
  }
  return res;
}

Comando* Comando::extrai_comando(No_arv_parse* no) {
  if (no == NULL) return NULL;
  
  if (no->regra >= 14 && no->regra <= 17) return extrai_comando(no->filhos[0]);

  if (no->regra >= 18 && no->regra <= 20) { // IF
    ComandoIf* res = new ComandoIf();
    res->condicao = Expressao::extrai_expressao(no->filhos[1]);
    res->entao = extrai_comando(no->filhos[3]);
    if (no->regra == 19 || no->regra == 20) res->senao = extrai_comando(no->filhos[5]);
    return res;
  }
  if (no->regra == 21) { // WHILE
    ComandoWhile* res = new ComandoWhile();
    res->condicao = Expressao::extrai_expressao(no->filhos[1]);
    res->corpo = extrai_comando(no->filhos[3]);
    return res;
  }
  if (no->regra == 22) { // RETURN
    ComandoRetorno* res = new ComandoRetorno();
    res->expressao = Expressao::extrai_expressao(no->filhos[1]);
    return res;
  }
  if (no->regra >= 28 && no->regra <= 30) { // ATRIBUICAO
    ComandoAtribuicao* res = new ComandoAtribuicao();
    Expressao* esq_exp = Expressao::extrai_expressao(no->filhos[0]);
    if (auto var = dynamic_cast<ExpressaoVariavel*>(esq_exp)) res->esquerda = var->nome;
    res->direita = Expressao::extrai_expressao(no->filhos[2]);
    return res;
  }
  if (no->regra == 23 || no->regra == 24) { // BLOCO
    ComandoLista* res = new ComandoLista();
    if (no->regra == 23) res->lista_comandos = extrai_lista_comandos(no->filhos[1]);
    return res;
  }
  return NULL;
}

void Comando::debug_com_tab(int tab) {
  tab3(tab); cerr << "Comando generico"<< endl;
}
