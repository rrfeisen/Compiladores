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
  vector<string>   parametros;   // nomes dos parâmetros (param_list)
  vector<string>   locais;       // nomes das variáveis locais (opt_local)
  vector<Comando*> comandos;     // corpo (stmt_list)

  Funcao() {}

  // Extrai a partir da raiz da árvore (deve ser proc_decl, regra 10,
  // ou program->decl_list->decl->proc_decl)
  static Funcao* extrai_funcao(No_arv_parse* raiz);

  void debug();

private:
  // Navega pela árvore até encontrar um nó proc_decl (regra 10)
  static No_arv_parse* acha_proc_decl(No_arv_parse* no);

  // Extrai lista de IDs de um nó id_list (regras 8 e 9)
  static vector<string> extrai_id_list(No_arv_parse* no);

  // Extrai param_list (regras 6 e 7)
  static vector<string> extrai_param_list(No_arv_parse* no);

  // Extrai opt_local (regras 11 e 12)
  static vector<string> extrai_opt_local(No_arv_parse* no);
};

#endif
