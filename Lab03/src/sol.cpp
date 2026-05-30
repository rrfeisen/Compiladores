#include <algorithm>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <stack>
using namespace std;

#include "Gramatica.hpp"
#include "TabelaLR1.hpp"
#include "Arvore.hpp"
#include "Parser.hpp"
#include "Funcao.hpp"
#include "Analisador.hpp"

// Tenta interpretar uma string como float ou int
ValorLiteral le_valor(const string& linha) {
  ValorLiteral v;
  // verifica se tem ponto decimal
  bool tem_ponto = (linha.find('.') != string::npos);
  if (tem_ponto) {
    v.tipo        = new Tipo(Tipo::FLOAT);
    v.valor_float = (float)atof(linha.c_str());
  } else if (linha == "true" || linha == "false") {
    v.tipo       = new Tipo(Tipo::BOOL);
    v.valor_bool = (linha == "true");
  } else {
    v.tipo      = new Tipo(Tipo::INT);
    v.valor_int = atoi(linha.c_str());
  }
  return v;
}

int main(int argc, char* argv[]) {
  if (argc != 4 && argc != 3 && argc != 1) {
    cerr << "Uso: " << argv[0]
         << " <gramatica.site> <tabela_lr1.conf> <arquivo.params>" << endl;
    cerr << "     Os tokens sao lidos do stdin." << endl;
    return 1;
  }

  string nome_gramatica = "gramatica-9/gramatica-9.site";
  string nome_tab_lr1   = "gramatica-9/tabela_lr1.conf";
  string nome_params    = "";

  if (argc >= 3) {
    nome_gramatica = argv[1];
    nome_tab_lr1   = argv[2];
  }
  if (argc == 4) {
    nome_params = argv[3];
  }

  // --- abre gramática e tabela ---
  ifstream arq_gramatica(nome_gramatica);
  ifstream arq_tabela_lr1(nome_tab_lr1);
  if (arq_gramatica.fail() || arq_tabela_lr1.fail()) {
    cerr << "Falha ao abrir: "
         << (arq_gramatica.fail()  ? nome_gramatica + " " : "")
         << (arq_tabela_lr1.fail() ? nome_tab_lr1         : "") << endl;
    return 1;
  }

  Parser parser(arq_gramatica, arq_tabela_lr1);

  // --- parse dos tokens (stdin) ---
  Arvore_parse arv = parser.executa_parse(cin);
  cerr << "=== Arvore ===" << endl;
  arv.debug();

  // --- extração da função ---
  Funcao* func = Funcao::extrai_funcao(arv.raiz);
  if (func == NULL) {
    cerr << "ERRO: nao foi possivel extrair a funcao." << endl;
    return 1;
  }
  cerr << "=== Funcao ===" << endl;
  func->debug();

  // --- leitura dos parâmetros do .params ---
  vector<ValorLiteral> parametros_passados;

  if (nome_params != "") {
    ifstream arq_params(nome_params);
    if (arq_params.fail()) {
      cerr << "Falha ao abrir params: " << nome_params << endl;
      return 1;
    }
    string linha;
    while (getline(arq_params, linha)) {
      // ignora linhas vazias
      if (linha.find_first_not_of(" \t\r\n") == string::npos) continue;
      parametros_passados.push_back(le_valor(linha));
    }
  } else {
    // sem .params: inicializa todos como 0
    cerr << "Aviso: nenhum .params fornecido, parametros inicializados com 0." << endl;
    for (int i = 0; i < (int)func->parametros.size(); ++i) {
      parametros_passados.push_back(ValorLiteral(0));
    }
  }

  // --- executa e imprime resultado ---
  Analisador ana;
  string resultado = ana.calcula_retorno(func, parametros_passados);
  cout << resultado << endl;

  return 0;
}