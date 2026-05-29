#ifndef _FUNCAO_HPP_
#define _FUNCAO_HPP_
#include<vector>
#include "ID.hpp"
#include "Variavel.hpp"
#include "Comando.hpp"
#include "Tipo.hpp"
#include "../Arvore.hpp"
using namespace std;

class Funcao {
public:
  Tipo* tipo_retorno;
  ID* nome_funcao;
  vector<Variavel*> parametros;
  vector<Comando*> comandos;

  Funcao();
  static Funcao* extrai_funcao(No_arv_parse *arv);
  void debug();
};

#endif
