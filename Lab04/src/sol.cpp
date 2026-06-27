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
#include "../Frame/FrameFuncao.hpp"
#include "../Frame/FrameAcessoNoFrame.hpp"
#include "../Frame/FrameAcessoTemp.hpp"

// Tenta interpretar uma string como float ou int
ValorLiteral le_valor(const string& linha) {
  ValorLiteral v;
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

// ----------------------------------------------------------------
// Imprime, para cada ocorrência de variável na função, qual o seu
// FrameAcesso (no frame em FP+delta, ou pseudo-registrador com id).
// Reaproveita a mesma varredura usada em FrameFuncao para manter a
// ordem de aparecimento.
// ----------------------------------------------------------------
static void imprime_acesso(Expressao* e, ostream& out);

static void imprime_acesso_var(ExpressaoVariavel* ev, ostream& out) {
  out << "  " << ev->nome->nome << " -> ";
  if (ev->acesso_frame == NULL) {
    out << "(sem acesso atribuido)" << endl;
    return;
  }
  if (FrameAcessoNoFrame* fnf = dynamic_cast<FrameAcessoNoFrame*>(ev->acesso_frame)) {
    out << "FRAME FP" << (fnf->posicao_no_frame >= 0 ? "+" : "")
        << fnf->posicao_no_frame << endl;
  } else if (FrameAcessoTemp* ft = dynamic_cast<FrameAcessoTemp*>(ev->acesso_frame)) {
    out << "TEMP r" << ft->id << endl;
  } else {
    out << "(tipo de acesso desconhecido)" << endl;
  }
}

static void imprime_acesso(Expressao* e, ostream& out) {
  if (e == NULL) return;
  if (ExpressaoVariavel* ev = dynamic_cast<ExpressaoVariavel*>(e)) {
    imprime_acesso_var(ev, out);
    return;
  }
  if (ExpressaoChamada* ec = dynamic_cast<ExpressaoChamada*>(e)) {
    for (size_t i = 0; i < ec->argumentos.size(); ++i) imprime_acesso(ec->argumentos[i], out);
    return;
  }
  if (ExpressaoNot* en = dynamic_cast<ExpressaoNot*>(e)) {
    imprime_acesso(en->operando, out);
    return;
  }
  if (ExpressaoBinaria* eb = dynamic_cast<ExpressaoBinaria*>(e)) {
    imprime_acesso(eb->esquerda, out);
    imprime_acesso(eb->direita, out);
    return;
  }
}

static void imprime_acessos_comandos(const vector<Comando*>& cmds, ostream& out) {
  for (size_t i = 0; i < cmds.size(); ++i) {
    Comando* c = cmds[i];
    if (ComandoAtribuicao* ca = dynamic_cast<ComandoAtribuicao*>(c)) {
      imprime_acesso(ca->expressao, out);
    } else if (ComandoExpr* ce = dynamic_cast<ComandoExpr*>(c)) {
      imprime_acesso(ce->expressao, out);
    } else if (ComandoRetorno* cr = dynamic_cast<ComandoRetorno*>(c)) {
      imprime_acesso(cr->expressao, out);
    } else if (ComandoIf* ci = dynamic_cast<ComandoIf*>(c)) {
      imprime_acesso(ci->condicao, out);
      imprime_acessos_comandos(ci->then_cmds, out);
      imprime_acessos_comandos(ci->else_cmds, out);
      if (ci->else_if) {
        vector<Comando*> tmp; tmp.push_back(ci->else_if);
        imprime_acessos_comandos(tmp, out);
      }
    } else if (ComandoWhile* cw = dynamic_cast<ComandoWhile*>(c)) {
      imprime_acesso(cw->condicao, out);
      imprime_acessos_comandos(cw->corpo, out);
    }
  }
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

  // --- geração do Frame (Lab04) ---
  FrameFuncao* frame = FrameFuncao::gera_frame_de_funcao(func);
  if (frame == NULL) {
    cerr << "ERRO: nao foi possivel gerar o frame da funcao." << endl;
    return 1;
  }

  cout << "=== FrameFuncao: " << func->nome << " ===" << endl;
  cout << frame->gera_relatorio();
  cout << "=== Acessos por ocorrencia de variavel ===" << endl;
  imprime_acessos_comandos(func->comandos, cout);

  // --- leitura dos parâmetros do .params (mantido do Lab03, opcional) ---
  vector<ValorLiteral> parametros_passados;
  if (nome_params != "") {
    ifstream arq_params(nome_params);
    if (!arq_params.fail()) {
      string linha;
      while (getline(arq_params, linha)) {
        if (linha.find_first_not_of(" \t\r\n") == string::npos) continue;
        parametros_passados.push_back(le_valor(linha));
      }
    }
  }
  for (int i = (int)parametros_passados.size(); i < (int)func->parametros.size(); ++i) {
    parametros_passados.push_back(ValorLiteral(0));
  }

  // --- execução semântica (Lab03), mantida para referência ---
  Analisador ana;
  string resultado = ana.calcula_retorno(func, parametros_passados);
  cout << "=== Retorno calculado (Lab03) ===" << endl;
  cout << resultado << endl;

  return 0;
}
