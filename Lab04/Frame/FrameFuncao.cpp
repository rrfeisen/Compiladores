#include "FrameFuncao.hpp"
#include "FrameAcessoNoFrame.hpp"
#include "FrameAcessoTemp.hpp"
#include <set>
#include <map>
#include <sstream>
#include <iostream>
using namespace std;

static const int OVERHEAD_FRAME = 40; // 5 palavras de 8 bytes
static const int TAMANHO_VAR    = 8;  // toda variavel ocupa 8 bytes

// Acrescenta em 'destino' todas as ExpressaoVariavel encontradas dentro
// de 'e' (busca em profundidade, cobrindo todos os tipos de Expressao
// definidos em Expressao.hpp).
static void coleta_variaveis_em_expressao(Expressao* e, vector<ExpressaoVariavel*>& destino) {
  if (e == NULL) return;

  if (ExpressaoVariavel* ev = dynamic_cast<ExpressaoVariavel*>(e)) {
    destino.push_back(ev);
    return;
  }
  if (ExpressaoValor* ev2 = dynamic_cast<ExpressaoValor*>(e)) {
    (void)ev2;
    return; // literal, sem variaveis
  }
  if (ExpressaoChamada* ec = dynamic_cast<ExpressaoChamada*>(e)) {
    for (size_t i = 0; i < ec->argumentos.size(); ++i) {
      coleta_variaveis_em_expressao(ec->argumentos[i], destino);
    }
    return;
  }
  if (ExpressaoNot* en = dynamic_cast<ExpressaoNot*>(e)) {
    coleta_variaveis_em_expressao(en->operando, destino);
    return;
  }
  if (ExpressaoBinaria* eb = dynamic_cast<ExpressaoBinaria*>(e)) {
    coleta_variaveis_em_expressao(eb->esquerda, destino);
    coleta_variaveis_em_expressao(eb->direita, destino);
    return;
  }
}

// Marca em 'marcadas' (por nome) toda variável que aparece DIRETAMENTE
// como argumento de alguma chamada de função encontrada dentro de 'e'.
// Também atualiza 'max_args' com o maior número de argumentos visto em
// qualquer chamada.
static void marca_variaveis_arg_de_chamada(Expressao* e, set<string>& marcadas, int& max_args) {
  if (e == NULL) return;

  if (ExpressaoChamada* ec = dynamic_cast<ExpressaoChamada*>(e)) {
    if ((int)ec->argumentos.size() > max_args) max_args = (int)ec->argumentos.size();
    for (size_t i = 0; i < ec->argumentos.size(); ++i) {
      // todas as variaveis dentro do argumento (mesmo que aninhadas em
      // sub-expressoes aritmeticas) sao marcadas, pois "aparecerem dentro
      // de alguma expressao que e parametro de alguma funcao chamada"
      vector<ExpressaoVariavel*> vars;
      coleta_variaveis_em_expressao(ec->argumentos[i], vars);
      for (size_t j = 0; j < vars.size(); ++j) {
        marcadas.insert(vars[j]->nome->nome);
      }
      // a propria sub-expressao do argumento tambem pode conter outras
      // chamadas de funcao (chamadas aninhadas); continuamos a busca.
      marca_variaveis_arg_de_chamada(ec->argumentos[i], marcadas, max_args);
    }
    return;
  }
  if (ExpressaoNot* en = dynamic_cast<ExpressaoNot*>(e)) {
    marca_variaveis_arg_de_chamada(en->operando, marcadas, max_args);
    return;
  }
  if (ExpressaoBinaria* eb = dynamic_cast<ExpressaoBinaria*>(e)) {
    marca_variaveis_arg_de_chamada(eb->esquerda, marcadas, max_args);
    marca_variaveis_arg_de_chamada(eb->direita, marcadas, max_args);
    return;
  }
  // ExpressaoValor, ExpressaoVariavel isoladas: nada a fazer aqui.
}

// Percorre uma lista de comandos (incluindo if/while recursivamente),
// aplicando 'marca_variaveis_arg_de_chamada' a toda expressão encontrada.
static void varre_comandos_marcando(const vector<Comando*>& cmds, set<string>& marcadas, int& max_args) {
  for (size_t i = 0; i < cmds.size(); ++i) {
    Comando* c = cmds[i];
    if (ComandoAtribuicao* ca = dynamic_cast<ComandoAtribuicao*>(c)) {
      marca_variaveis_arg_de_chamada(ca->expressao, marcadas, max_args);
    } else if (ComandoExpr* ce = dynamic_cast<ComandoExpr*>(c)) {
      marca_variaveis_arg_de_chamada(ce->expressao, marcadas, max_args);
    } else if (ComandoRetorno* cr = dynamic_cast<ComandoRetorno*>(c)) {
      marca_variaveis_arg_de_chamada(cr->expressao, marcadas, max_args);
    } else if (ComandoIf* ci = dynamic_cast<ComandoIf*>(c)) {
      marca_variaveis_arg_de_chamada(ci->condicao, marcadas, max_args);
      varre_comandos_marcando(ci->then_cmds, marcadas, max_args);
      varre_comandos_marcando(ci->else_cmds, marcadas, max_args);
      if (ci->else_if) {
        vector<Comando*> tmp; tmp.push_back(ci->else_if);
        varre_comandos_marcando(tmp, marcadas, max_args);
      }
    } else if (ComandoWhile* cw = dynamic_cast<ComandoWhile*>(c)) {
      marca_variaveis_arg_de_chamada(cw->condicao, marcadas, max_args);
      varre_comandos_marcando(cw->corpo, marcadas, max_args);
    }
  }
}

// Coleta, em ordem de aparecimento, todas as ExpressaoVariavel da função
// (necessário para a passagem de alocação, garantindo que toda ocorrência
// de uma mesma variável receba o mesmo FrameAcesso).
static void varre_comandos_coletando(const vector<Comando*>& cmds, vector<ExpressaoVariavel*>& todas) {
  for (size_t i = 0; i < cmds.size(); ++i) {
    Comando* c = cmds[i];
    if (ComandoAtribuicao* ca = dynamic_cast<ComandoAtribuicao*>(c)) {
      coleta_variaveis_em_expressao(ca->expressao, todas);
    } else if (ComandoExpr* ce = dynamic_cast<ComandoExpr*>(c)) {
      coleta_variaveis_em_expressao(ce->expressao, todas);
    } else if (ComandoRetorno* cr = dynamic_cast<ComandoRetorno*>(c)) {
      coleta_variaveis_em_expressao(cr->expressao, todas);
    } else if (ComandoIf* ci = dynamic_cast<ComandoIf*>(c)) {
      coleta_variaveis_em_expressao(ci->condicao, todas);
      varre_comandos_coletando(ci->then_cmds, todas);
      varre_comandos_coletando(ci->else_cmds, todas);
      if (ci->else_if) {
        vector<Comando*> tmp; tmp.push_back(ci->else_if);
        varre_comandos_coletando(tmp, todas);
      }
    } else if (ComandoWhile* cw = dynamic_cast<ComandoWhile*>(c)) {
      coleta_variaveis_em_expressao(cw->condicao, todas);
      varre_comandos_coletando(cw->corpo, todas);
    }
  }
}

// Construtor
FrameFuncao::FrameFuncao() {
  tamanho_frame          = 0;
  n_param_entrada        = 0;
  n_maximo_param_saida   = 0;
  n_pseudo_registradores = 0;
  n_variaveis_no_frame   = 0;
}

// gera_frame_de_funcao — ponto de entrada principal
FrameFuncao* FrameFuncao::gera_frame_de_funcao(Funcao* fun) {
  if (fun == NULL) return NULL;

  FrameFuncao* res = new FrameFuncao();
  res->n_param_entrada = (int)fun->parametros.size();

  set<string> marcadas_para_frame;
  int max_args = 0;
  varre_comandos_marcando(fun->comandos, marcadas_para_frame, max_args);
  res->n_maximo_param_saida = max_args;

  // Mapas de nome -> FrameAcesso já atribuído (garante mesmo ponteiro
  // para todas as ocorrências da mesma variável).
  map<string, FrameAcesso*> alocados;

  for (int i = 0; i < (int)fun->parametros.size(); ++i) {
    int posicao = (i + 1) * TAMANHO_VAR; // +8, +16, +24, ...
    alocados[fun->parametros[i]] = new FrameAcessoNoFrame(posicao);
  }

  vector<ExpressaoVariavel*> todas_ocorrencias;
  varre_comandos_coletando(fun->comandos, todas_ocorrencias);

  set<string> nomes_locais(fun->locais.begin(), fun->locais.end());
  int proxima_posicao_frame = -OVERHEAD_FRAME; // -40
  int proximo_id_temp       = 1;

  for (size_t i = 0; i < todas_ocorrencias.size(); ++i) {
    const string& nome = todas_ocorrencias[i]->nome->nome;
    if (alocados.find(nome) != alocados.end()) continue; // ja alocada
    if (nomes_locais.find(nome) == nomes_locais.end()) continue; // nao e local (tratado em 2c)

    if (marcadas_para_frame.find(nome) != marcadas_para_frame.end()) {
      alocados[nome] = new FrameAcessoNoFrame(proxima_posicao_frame);
      proxima_posicao_frame -= TAMANHO_VAR;
      res->n_variaveis_no_frame++;
    } else {
      alocados[nome] = new FrameAcessoTemp(proximo_id_temp);
      proximo_id_temp++;
      res->n_pseudo_registradores++;
    }
  }

  for (size_t i = 0; i < fun->locais.size(); ++i) {
    const string& nome = fun->locais[i];
    if (alocados.find(nome) != alocados.end()) continue;
    alocados[nome] = new FrameAcessoTemp(proximo_id_temp);
    proximo_id_temp++;
    res->n_pseudo_registradores++;
  }

  for (size_t i = 0; i < todas_ocorrencias.size(); ++i) {
    ExpressaoVariavel* ev = todas_ocorrencias[i];
    const string& nome = ev->nome->nome;
    map<string, FrameAcesso*>::iterator it = alocados.find(nome);
    if (it != alocados.end()) {
      ev->acesso_frame = it->second;
    } else {
      cerr << "AVISO: variavel '" << nome
           << "' usada mas nao declarada como parametro ou local." << endl;
      FrameAcessoTemp* extra = new FrameAcessoTemp(proximo_id_temp++);
      alocados[nome] = extra;
      ev->acesso_frame = extra;
      res->n_pseudo_registradores++;
    }
  }

  // ---------- Tamanho final do frame ----------
  res->tamanho_frame = OVERHEAD_FRAME + TAMANHO_VAR * res->n_variaveis_no_frame;

  return res;
}

// gera_relatorio — formato texto padronizado de saída
string FrameFuncao::gera_relatorio() {
  stringstream ss;
  ss << "tamanho_frame: "          << tamanho_frame          << "\n";
  ss << "n_param_entrada: "        << n_param_entrada        << "\n";
  ss << "n_maximo_param_saida: "   << n_maximo_param_saida   << "\n";
  ss << "n_pseudo_registradores: " << n_pseudo_registradores << "\n";
  ss << "n_variaveis_no_frame: "   << n_variaveis_no_frame   << "\n";
  return ss.str();
}
