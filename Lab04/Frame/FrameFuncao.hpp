#ifndef _FRAME_FUNCAO_HPP_
#define _FRAME_FUNCAO_HPP_
#include "../src/src-gram9/Funcao.hpp"
using namespace std;

class FrameFuncao {
public:
  int tamanho_frame;
  int n_param_entrada;
  int n_maximo_param_saida;
  int n_pseudo_registradores;
  int n_variaveis_no_frame;

  FrameFuncao();

  // Gera a descricao de Frame para a funcao, preenchendo tambem o campo
  // acesso_frame de cada ExpressaoVariavel encontrada na arvore da funcao.
  static FrameFuncao* gera_frame_de_funcao(Funcao* fun);

  // Imprime o FrameFuncao no formato texto padronizado (ver gera_relatorio)
  string gera_relatorio();
};

#endif
