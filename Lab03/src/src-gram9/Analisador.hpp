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
  // Retorna string formatada do último valor atribuído
  string calcula_retorno(Funcao* f, const vector<ValorLiteral>& params);

private:
  map<string, ValorLiteral> env; // ambiente de variáveis

  // Avalia uma expressão e retorna seu valor
  ValorLiteral avalia(Expressao* e);

  // Executa uma lista de comandos; retorna true se houve return
  bool executa(const vector<Comando*>& cmds, ValorLiteral& ultimo_atrib);

  // Executa um único comando
  bool executa_cmd(Comando* c, ValorLiteral& ultimo_atrib);

  // Converte valor para string de saída
  string formata(const ValorLiteral& v);

  // Checagem de tipos com conversão implícita (int <-> float)
  ValorLiteral converte(const ValorLiteral& v, Tipo::Valor alvo);
};

#endif
