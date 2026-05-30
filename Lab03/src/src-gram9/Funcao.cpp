/*
 * Funcao.cpp — gramática 9 (Icon)
 *
 * Regras relevantes:
 *   0  program    -> decl_list
 *   1  decl_list  -> decl_list decl
 *   2  decl_list  -> decl
 *   3  decl       -> record_decl
 *   4  decl       -> proc_decl
 *  10  proc_decl  -> KW_PROCEDURE ID LPAREN param_list RPAREN opt_local stmt_list KW_END
 *                    filhos:  [0]         [1] [2]      [3]     [4]     [5]        [6]      [7]
 *   6  param_list -> id_list
 *   7  param_list -> ''
 *   8  id_list    -> id_list COMMA ID
 *   9  id_list    -> ID
 *  11  opt_local  -> KW_LOCAL id_list
 *  12  opt_local  -> ''
 */

#include "Funcao.hpp"
#include <iostream>
#include "../debug-util.hpp"
using namespace std;

// ----------------------------------------------------------------
// Helpers privados
// ----------------------------------------------------------------

// id_list: regra 8 (recursiva) ou 9 (base)
vector<string> Funcao::extrai_id_list(No_arv_parse* no) {
  if (no->regra == 9) { // id_list -> ID
    vector<string> res;
    res.push_back(no->filhos[0]->dado_extra);
    return res;
  }
  // regra 8: id_list -> id_list COMMA ID
  vector<string> res = extrai_id_list(no->filhos[0]);
  res.push_back(no->filhos[2]->dado_extra);
  return res;
}

// param_list: regra 6 (tem id_list) ou 7 (vazio)
vector<string> Funcao::extrai_param_list(No_arv_parse* no) {
  if (no->regra == 7) return vector<string>(); // vazio
  return extrai_id_list(no->filhos[0]);        // regra 6
}

// opt_local: regra 11 (KW_LOCAL id_list) ou 12 (vazio)
vector<string> Funcao::extrai_opt_local(No_arv_parse* no) {
  if (no->regra == 12) return vector<string>(); // vazio
  return extrai_id_list(no->filhos[1]);          // regra 11
}

// Procura recursivamente o primeiro nó proc_decl (regra 10)
No_arv_parse* Funcao::acha_proc_decl(No_arv_parse* no) {
  if (no == NULL) return NULL;
  if (no->regra == 10) return no;
  for (auto filho : no->filhos) {
    No_arv_parse* r = acha_proc_decl(filho);
    if (r) return r;
  }
  return NULL;
}

// ----------------------------------------------------------------
// extrai_funcao — ponto de entrada
// ----------------------------------------------------------------
Funcao* Funcao::extrai_funcao(No_arv_parse* raiz) {
  No_arv_parse* proc = acha_proc_decl(raiz);
  if (proc == NULL) {
    cerr << "extrai_funcao: proc_decl nao encontrado" << endl;
    return NULL;
  }

  // proc_decl -> KW_PROCEDURE[0] ID[1] LPAREN[2] param_list[3] RPAREN[4] opt_local[5] stmt_list[6] KW_END[7]
  Funcao* f = new Funcao();
  f->nome      = proc->filhos[1]->dado_extra;
  f->parametros = extrai_param_list(proc->filhos[3]);
  f->locais     = extrai_opt_local(proc->filhos[5]);
  f->comandos   = Comando::extrai_stmt_list(proc->filhos[6]);
  return f;
}

// ----------------------------------------------------------------
// debug
// ----------------------------------------------------------------
void Funcao::debug() {
  cerr << "procedure " << nome << "(";
  for (int i = 0; i < (int)parametros.size(); ++i) {
    if (i) cerr << ", ";
    cerr << parametros[i];
  }
  cerr << ")" << endl;
  if (!locais.empty()) {
    cerr << "  local ";
    for (int i = 0; i < (int)locais.size(); ++i) {
      if (i) cerr << ", ";
      cerr << locais[i];
    }
    cerr << endl;
  }
  for (auto c : comandos) c->debug_com_tab(1);
  cerr << "end" << endl;
}
