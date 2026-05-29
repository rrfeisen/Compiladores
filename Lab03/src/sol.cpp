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
  if (argc < 3) return 1;

  string nome_gramatica = string(argv[1]);
  string nome_tab_lr1 = string(argv[2]);
  string nome_params = (argc >= 4) ? string(argv[3]) : "";

  ifstream arq_gramatica(nome_gramatica);
  ifstream arq_tabela_lr1(nome_tab_lr1);
  
  cerr << "  [DEBUG] Iniciando Parser..." << endl;
  Parser parser(arq_gramatica, arq_tabela_lr1);
  Arvore_parse arv = parser.executa_parse(cin);
  
  cerr << "  [DEBUG] Extraindo AST da Funcao..." << endl;
  Funcao* func = Funcao::extrai_funcao(arv.raiz);
  if (!func) {
      cerr << "  [ERRO] Funcao nao encontrada na AST!" << endl;
      return 1;
  }

  vector<ValorLiteral> parametros_passados;
  if (!nome_params.empty()) {
    ifstream arq_params(nome_params);
    if (!arq_params.fail()) {
        string linha;
        int i = 0;
        while (getline(arq_params, linha) && i < func->parametros.size()) {
            if (linha.empty()) continue;
            ValorLiteral valor;
            valor.tipo = new Tipo(Tipo::INT);
            
            string up = linha;
            transform(up.begin(), up.end(), up.begin(), ::toupper);
            if (up == "TRUE" || up == "FALSE") {
                valor.tipo->valor = Tipo::BOOL;
                valor.valor_bool = (up == "TRUE");
            } else if (linha.find('.') != string::npos) {
                valor.tipo->valor = Tipo::FLOAT;
                valor.valor_float = stof(linha);
            } else {
                valor.tipo->valor = Tipo::INT;
                valor.valor_int = stoi(linha);
            }
            parametros_passados.push_back(valor);
            i++;
        }
    }
  }

  cerr << "  [DEBUG] Iniciando Maquina Virtual..." << endl;
  Analisador ana;
  ana.calcula_retorno(func, parametros_passados);
  
  cerr << "  [DEBUG] Teste Finalizado com Sucesso!" << endl;
  return 0;
}
