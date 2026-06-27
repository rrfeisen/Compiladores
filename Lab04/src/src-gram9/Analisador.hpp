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
  string calcula_retorno(Funcao* f, const vector<ValorLiteral>& params);

private:
  map<string, ValorLiteral> env;

  ValorLiteral avalia(Expressao* e);
  bool executa(const vector<Comando*>& cmds, ValorLiteral& ultimo_atrib);
  bool executa_cmd(Comando* c, ValorLiteral& ultimo_atrib);
  string formata(const ValorLiteral& v);
  ValorLiteral converte(const ValorLiteral& v, Tipo::Valor alvo);
};

#endif
