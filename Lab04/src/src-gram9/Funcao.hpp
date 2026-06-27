#ifndef _FUNCAO_HPP_
#define _FUNCAO_HPP_
#include <vector>
#include <string>
#include "ID.hpp"
#include "Comando.hpp"
#include "../Arvore.hpp"
using namespace std;

class Funcao {
public:
  string           nome;
  vector<string>   parametros;
  vector<string>   locais;
  vector<Comando*> comandos;

  Funcao() {}

  static Funcao* extrai_funcao(No_arv_parse* raiz);

  void debug();

private:
  static No_arv_parse* acha_proc_decl(No_arv_parse* no);
  static vector<string> extrai_id_list(No_arv_parse* no);
  static vector<string> extrai_param_list(No_arv_parse* no);
  static vector<string> extrai_opt_local(No_arv_parse* no);
};

#endif
