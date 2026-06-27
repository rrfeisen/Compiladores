#include "Funcao.hpp"
#include <iostream>
#include "../debug-util.hpp"
using namespace std;

vector<string> Funcao::extrai_id_list(No_arv_parse* no) {
  if (no->regra == 9) {
    vector<string> res;
    res.push_back(no->filhos[0]->dado_extra);
    return res;
  }
  vector<string> res = extrai_id_list(no->filhos[0]);
  res.push_back(no->filhos[2]->dado_extra);
  return res;
}

vector<string> Funcao::extrai_param_list(No_arv_parse* no) {
  if (no->regra == 7) return vector<string>();
  return extrai_id_list(no->filhos[0]);
}

vector<string> Funcao::extrai_opt_local(No_arv_parse* no) {
  if (no->regra == 12) return vector<string>();
  return extrai_id_list(no->filhos[1]);
}

No_arv_parse* Funcao::acha_proc_decl(No_arv_parse* no) {
  if (no == NULL) return NULL;
  if (no->regra == 10) return no;
  for (size_t i = 0; i < no->filhos.size(); ++i) {
    No_arv_parse* r = acha_proc_decl(no->filhos[i]);
    if (r) return r;
  }
  return NULL;
}

Funcao* Funcao::extrai_funcao(No_arv_parse* raiz) {
  No_arv_parse* proc = acha_proc_decl(raiz);
  if (proc == NULL) {
    cerr << "extrai_funcao: proc_decl nao encontrado" << endl;
    return NULL;
  }

  Funcao* f = new Funcao();
  f->nome       = proc->filhos[1]->dado_extra;
  f->parametros = extrai_param_list(proc->filhos[3]);
  f->locais     = extrai_opt_local(proc->filhos[5]);
  f->comandos   = Comando::extrai_stmt_list(proc->filhos[6]);
  return f;
}

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
  for (size_t i = 0; i < comandos.size(); ++i) comandos[i]->debug_com_tab(1);
  cerr << "end" << endl;
}
