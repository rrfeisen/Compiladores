#ifndef _FRAME_FUNCAO_HPP_
#define _FRAME_FUNCAO_HPP_
#include "../src-gram5/Funcao.hpp"
using namespace std;

class FrameFuncao {
public:
  int tamanho_frame;
  int n_param_entrada;
  int n_maximo_param_saida;
  int n_pseudo_registradores;
  int n_variaveis_no_frame;

  FrameFuncao();
  static FrameFuncao* gera_frame_de_funcao(Funcao* fun);
};

#endif
