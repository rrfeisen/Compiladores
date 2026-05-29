#ifndef _ANALISADOR_HPP_
#define _ANALISADOR_HPP_
#include "Funcao.hpp"
#include "ValorLiteral.hpp"
#include "Expressao.hpp"
#include "Comando.hpp"
#include <vector>
#include <map>
#include <string>
using namespace std;

class Analisador {
public:
  map<string, ValorLiteral> tabela_simbolos;
  ValorLiteral ultimo_valor_atribuido;

  int calcula_retorno(Funcao *f, const vector<ValorLiteral> &params);
  void executar_comando(Comando* c);
  ValorLiteral avaliar_expressao(Expressao* e);
};
#endif
