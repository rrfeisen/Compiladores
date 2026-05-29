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

int main(int argc, char * argv[]) {
  if (argc < 3) {
    cerr << "Uso: ./compilador <gramatica> <tabela_lr1> [arquivo.params]" << endl;
    return 1;
  }
  string nome_gramatica = string(argv[1]);
  string nome_tab_lr1 = string(argv[2]);
  string nome_params = (argc >= 4) ? string(argv[3]) : "";

  ifstream arq_gramatica(nome_gramatica);
  ifstream arq_tabela_lr1(nome_tab_lr1);
  if (arq_tabela_lr1.fail() || arq_gramatica.fail()) return 1;    

  Parser parser(arq_gramatica, arq_tabela_lr1);
  Arvore_parse arv = parser.executa_parse(cin);
  Funcao* func = Funcao::extrai_funcao(arv.raiz);

  vector<ValorLiteral> parametros_passados;

  // Lógica para ler os parâmetros do arquivo criado por si
  if (!nome_params.empty() && func != NULL) {
    ifstream arq_params(nome_params);
    if (!arq_params.fail()) {
        string linha;
        int i = 0;
        while (getline(arq_params, linha) && i < func->parametros.size()) {
            if (linha.empty()) continue;
            ValorLiteral valor;
            valor.tipo = new Tipo(func->parametros[i]->tipo->valor);

            if (valor.tipo->valor == Tipo::INT) valor.valor_int = stoi(linha);
            else if (valor.tipo->valor == Tipo::FLOAT) valor.valor_float = stof(linha);
            else if (valor.tipo->valor == Tipo::BOOL) {
                string up = linha;
                transform(up.begin(), up.end(), up.begin(), ::toupper);
                valor.valor_bool = (up == "TRUE" || up == "1");
            }
            parametros_passados.push_back(valor);
            i++;
        }
    }
  }

  if (func != NULL) {
      Analisador ana;
      ana.calcula_retorno(func, parametros_passados);
  }
  return 0;
}
