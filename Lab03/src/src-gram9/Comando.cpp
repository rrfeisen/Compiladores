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
  if (no == NULL) return res;

  if (no->simb == "stmt_list") {
      for (auto filho : no->filhos) {
          if (filho->simb == "stmt_list") {
              vector<Comando*> sub = extrai_lista_comandos(filho);
              res.insert(res.end(), sub.begin(), sub.end());
          }
          else if (filho->simb == "stmt") {
              Comando* c = extrai_comando(filho);
              if (c) res.push_back(c);
          }
      }
  }
  return res;
}

Comando* Comando::extrai_comando(No_arv_parse* no) {
  if (no == NULL) return NULL;

  if (no->simb == "if_stmt") {
      ComandoIf* res = new ComandoIf();
      int stmt_count = 0;
      for (auto filho : no->filhos) {
          if (filho->simb == "expr" || filho->simb == "cmp_expr") res->condicao = Expressao::extrai_expressao(filho);
          if (filho->simb == "stmt") {
              if (stmt_count == 0) res->entao = extrai_comando(filho);
              else res->senao = extrai_comando(filho);
              stmt_count++;
          }
      }
      return res;
  }
  
  if (no->simb == "while_stmt") {
      ComandoWhile* res = new ComandoWhile();
      for (auto filho : no->filhos) {
          if (filho->simb == "expr" || filho->simb == "cmp_expr") res->condicao = Expressao::extrai_expressao(filho);
          if (filho->simb == "stmt") res->corpo = extrai_comando(filho);
      }
      return res;
  }
  
  if (no->simb == "return_stmt") {
      ComandoRetorno* res = new ComandoRetorno();
      for (auto filho : no->filhos) {
          if (filho->simb == "expr" || filho->simb == "add_expr" || filho->simb == "mul_expr") {
              res->expressao = Expressao::extrai_expressao(filho);
          }
      }
      return res;
  }
  
  if (no->simb == "assign_expr" && no->filhos.size() >= 3) {
      ComandoAtribuicao* res = new ComandoAtribuicao();
      if (no->filhos[0]->simb == "ID") {
          res->esquerda = ID::extrai_ID(no->filhos[0]);
      } else {
          Expressao* esq = Expressao::extrai_expressao(no->filhos[0]);
          if (auto var = dynamic_cast<ExpressaoVariavel*>(esq)) res->esquerda = var->nome;
      }
      res->direita = Expressao::extrai_expressao(no->filhos[2]);
      return res;
  }

  // Repasse quando um nó encapsula o outro (ex: stmt -> assign_expr)
  if (no->filhos.size() == 1) return extrai_comando(no->filhos[0]);

  return NULL;
}

void Comando::debug_com_tab(int tab) {
  tab3(tab); cerr << "Comando generico"<< endl;
}
