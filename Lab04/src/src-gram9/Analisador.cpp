/*
 * Analisador.cpp — gramática 9 (Icon) — Lab04
 *
 * NOTA Lab04: a gramática agora aceita chamadas de função dentro de
 * expressões (ExpressaoChamada). Como o Analisador deste laboratório
 * só tem acesso à função sendo analisada (não a um conjunto de funções
 * do programa), uma chamada de função é avaliada de forma simplificada:
 * apenas os argumentos são avaliados (o que é suficiente para o
 * propósito do Lab04 — alocação de Frame — já que o objetivo principal
 * desta entrega é a geração do FrameFuncao, não a execução completa de
 * programas multi-função). O valor de retorno da chamada é 0 (INT).
 */

#include "Analisador.hpp"
#include <iostream>
#include <iomanip>
#include <sstream>
using namespace std;

string Analisador::formata(const ValorLiteral& v) {
  if (v.tipo == NULL) return "nil";
  stringstream ss;
  switch(v.tipo->valor) {
  case Tipo::INT:
    ss << v.valor_int;
    break;
  case Tipo::FLOAT:
    ss << fixed << setprecision(2) << v.valor_float;
    break;
  case Tipo::BOOL:
    ss << (v.valor_bool ? "true" : "false");
    break;
  }
  return ss.str();
}

ValorLiteral Analisador::converte(const ValorLiteral& v, Tipo::Valor alvo) {
  if (v.tipo && v.tipo->valor == alvo) return v;
  if (alvo == Tipo::FLOAT && v.tipo && v.tipo->valor == Tipo::INT) {
    return ValorLiteral((float)v.valor_int);
  }
  if (alvo == Tipo::INT && v.tipo && v.tipo->valor == Tipo::FLOAT) {
    return ValorLiteral((int)v.valor_float);
  }
  return v;
}

ValorLiteral Analisador::avalia(Expressao* e) {
  if (e == NULL) return ValorLiteral();

  if (ExpressaoValor* ev = dynamic_cast<ExpressaoValor*>(e)) {
    if (ev->valor == NULL) return ValorLiteral();
    return *(ev->valor);
  }

  if (ExpressaoVariavel* evar = dynamic_cast<ExpressaoVariavel*>(e)) {
    string nome = evar->nome ? evar->nome->nome : "";
    if (env.find(nome) != env.end()) return env[nome];
    cerr << "ERRO: variavel nao declarada '" << nome << "'" << endl;
    return ValorLiteral();
  }

  // --- chamada de função (Lab04): avalia argumentos, retorna 0 ---
  if (ExpressaoChamada* ec = dynamic_cast<ExpressaoChamada*>(e)) {
    for (size_t i = 0; i < ec->argumentos.size(); ++i) {
      avalia(ec->argumentos[i]); // efeitos colaterais avaliados, valor descartado
    }
    return ValorLiteral(0);
  }

  if (ExpressaoAtribuicao* ea = dynamic_cast<ExpressaoAtribuicao*>(e)) {
    ExpressaoVariavel* lv = dynamic_cast<ExpressaoVariavel*>(ea->esquerda);
    if (lv == NULL) {
      cerr << "ERRO: lvalue nao e variavel" << endl;
      return ValorLiteral();
    }
    string nome = lv->nome ? lv->nome->nome : "";
    ValorLiteral rval = avalia(ea->direita);

    if (ea->op == ":=") {
      env[nome] = rval;
    } else if (ea->op == "+:=") {
      ValorLiteral cur = env[nome];
      if (cur.tipo && rval.tipo &&
          (cur.tipo->valor == Tipo::FLOAT || rval.tipo->valor == Tipo::FLOAT)) {
        float a = (cur.tipo->valor  == Tipo::FLOAT) ? cur.valor_float  : (float)cur.valor_int;
        float b = (rval.tipo->valor == Tipo::FLOAT) ? rval.valor_float : (float)rval.valor_int;
        env[nome] = ValorLiteral(a + b);
      } else {
        env[nome] = ValorLiteral(cur.valor_int + rval.valor_int);
      }
    } else if (ea->op == "-:=") {
      ValorLiteral cur = env[nome];
      if (cur.tipo && rval.tipo &&
          (cur.tipo->valor == Tipo::FLOAT || rval.tipo->valor == Tipo::FLOAT)) {
        float a = (cur.tipo->valor  == Tipo::FLOAT) ? cur.valor_float  : (float)cur.valor_int;
        float b = (rval.tipo->valor == Tipo::FLOAT) ? rval.valor_float : (float)rval.valor_int;
        env[nome] = ValorLiteral(a - b);
      } else {
        env[nome] = ValorLiteral(cur.valor_int - rval.valor_int);
      }
    }
    return env[nome];
  }

  if (ExpressaoNot* en = dynamic_cast<ExpressaoNot*>(e)) {
    ValorLiteral op = avalia(en->operando);
    if (op.tipo == NULL || op.tipo->valor != Tipo::BOOL) {
      cerr << "ERRO de tipo: operador '!' aplicado a nao-booleano ("
           << (op.tipo ? op.tipo->nome() : "nil") << ")" << endl;
    }
    return ValorLiteral(!op.valor_bool);
  }

  if (ExpressaoOr* eo = dynamic_cast<ExpressaoOr*>(e)) {
    ValorLiteral l = avalia(eo->esquerda);
    ValorLiteral r = avalia(eo->direita);
    if (!l.tipo || l.tipo->valor != Tipo::BOOL ||
        !r.tipo || r.tipo->valor != Tipo::BOOL) {
      cerr << "ERRO de tipo: '||' requer operandos booleanos" << endl;
    }
    return ValorLiteral(l.valor_bool || r.valor_bool);
  }

  if (ExpressaoAnd* ea = dynamic_cast<ExpressaoAnd*>(e)) {
    ValorLiteral l = avalia(ea->esquerda);
    ValorLiteral r = avalia(ea->direita);
    if (!l.tipo || l.tipo->valor != Tipo::BOOL ||
        !r.tipo || r.tipo->valor != Tipo::BOOL) {
      cerr << "ERRO de tipo: '&&' requer operandos booleanos" << endl;
    }
    return ValorLiteral(l.valor_bool && r.valor_bool);
  }

  if (ExpressaoCmp* ec2 = dynamic_cast<ExpressaoCmp*>(e)) {
    ValorLiteral l = avalia(ec2->esquerda);
    ValorLiteral r = avalia(ec2->direita);

    if (ec2->op != "==" && ec2->op != "!=") {
      if ((l.tipo && l.tipo->valor == Tipo::BOOL) ||
          (r.tipo && r.tipo->valor == Tipo::BOOL)) {
        cerr << "ERRO de tipo: operador '" << ec2->op
             << "' nao se aplica a booleanos" << endl;
      }
    }

    bool usa_float = (l.tipo && l.tipo->valor == Tipo::FLOAT) ||
                     (r.tipo && r.tipo->valor == Tipo::FLOAT);
    if (usa_float) {
      float lf = l.tipo && l.tipo->valor == Tipo::FLOAT ? l.valor_float : (float)l.valor_int;
      float rf = r.tipo && r.tipo->valor == Tipo::FLOAT ? r.valor_float : (float)r.valor_int;
      bool res = false;
      if      (ec2->op == "==") res = (lf == rf);
      else if (ec2->op == "!=") res = (lf != rf);
      else if (ec2->op == "<")  res = (lf <  rf);
      else if (ec2->op == "<=") res = (lf <= rf);
      else if (ec2->op == ">")  res = (lf >  rf);
      else if (ec2->op == ">=") res = (lf >= rf);
      return ValorLiteral(res);
    } else {
      int li = (l.tipo && l.tipo->valor == Tipo::BOOL) ? (int)l.valor_bool : l.valor_int;
      int ri = (r.tipo && r.tipo->valor == Tipo::BOOL) ? (int)r.valor_bool : r.valor_int;
      bool res = false;
      if      (ec2->op == "==") res = (li == ri);
      else if (ec2->op == "!=") res = (li != ri);
      else if (ec2->op == "<")  res = (li <  ri);
      else if (ec2->op == "<=") res = (li <= ri);
      else if (ec2->op == ">")  res = (li >  ri);
      else if (ec2->op == ">=") res = (li >= ri);
      return ValorLiteral(res);
    }
  }

  #define ARITH(CLASS, OP_INT, OP_FLOAT) \
  if (CLASS* ea2 = dynamic_cast<CLASS*>(e)) { \
    ValorLiteral l = avalia(ea2->esquerda); \
    ValorLiteral r = avalia(ea2->direita); \
    if ((l.tipo && l.tipo->valor == Tipo::BOOL) || \
        (r.tipo && r.tipo->valor == Tipo::BOOL)) { \
      cerr << "ERRO de tipo: operador aritmetico com booleano" << endl; \
    } \
    bool usa_float2 = (l.tipo && l.tipo->valor == Tipo::FLOAT) || \
                      (r.tipo && r.tipo->valor == Tipo::FLOAT); \
    if (usa_float2) { \
      float lf = l.tipo && l.tipo->valor == Tipo::FLOAT ? l.valor_float : (float)l.valor_int; \
      float rf = r.tipo && r.tipo->valor == Tipo::FLOAT ? r.valor_float : (float)r.valor_int; \
      return ValorLiteral((float)(lf OP_FLOAT rf)); \
    } else { \
      return ValorLiteral((int)(l.valor_int OP_INT r.valor_int)); \
    } \
  }

  ARITH(ExpressaoSoma,         +, +)
  ARITH(ExpressaoSubtracao,    -, -)
  ARITH(ExpressaoMultiplicacao,*, *)
  ARITH(ExpressaoDivisao,      /, /)

  #undef ARITH

  if (ExpressaoMod* em = dynamic_cast<ExpressaoMod*>(e)) {
    ValorLiteral l = avalia(em->esquerda);
    ValorLiteral r = avalia(em->direita);
    if ((l.tipo && l.tipo->valor == Tipo::FLOAT) ||
        (r.tipo && r.tipo->valor == Tipo::FLOAT)) {
      cerr << "ERRO de tipo: operador '%' nao se aplica a float" << endl;
    }
    int ri = r.valor_int;
    if (ri == 0) { cerr << "ERRO: divisao por zero em %" << endl; return ValorLiteral(0); }
    return ValorLiteral(l.valor_int % ri);
  }

  cerr << "avalia: expressao desconhecida" << endl;
  return ValorLiteral();
}

bool Analisador::executa_cmd(Comando* c, ValorLiteral& ultimo_atrib) {
  if (c == NULL) return false;

  if (ComandoAtribuicao* ca = dynamic_cast<ComandoAtribuicao*>(c)) {
    ultimo_atrib = avalia(ca->expressao);
    return false;
  }

  if (ComandoExpr* ce = dynamic_cast<ComandoExpr*>(c)) {
    ValorLiteral v = avalia(ce->expressao);
    if (dynamic_cast<ExpressaoAtribuicao*>(ce->expressao)) {
      ultimo_atrib = v;
    }
    return false;
  }

  if (ComandoRetorno* cr = dynamic_cast<ComandoRetorno*>(c)) {
    ultimo_atrib = avalia(cr->expressao);
    return true;
  }

  if (ComandoIf* ci = dynamic_cast<ComandoIf*>(c)) {
    ValorLiteral cond = avalia(ci->condicao);
    bool cond_val = false;
    if (cond.tipo) {
      switch(cond.tipo->valor) {
      case Tipo::BOOL:  cond_val = cond.valor_bool;        break;
      case Tipo::INT:   cond_val = (cond.valor_int != 0);  break;
      case Tipo::FLOAT: cond_val = (cond.valor_float != 0.0f); break;
      }
    }
    if (cond_val) {
      if (executa(ci->then_cmds, ultimo_atrib)) return true;
    } else {
      if (!ci->else_cmds.empty()) {
        if (executa(ci->else_cmds, ultimo_atrib)) return true;
      } else if (ci->else_if != NULL) {
        if (executa_cmd(ci->else_if, ultimo_atrib)) return true;
      }
    }
    return false;
  }

  if (ComandoWhile* cw = dynamic_cast<ComandoWhile*>(c)) {
    int guard = 100000;
    while (guard-- > 0) {
      ValorLiteral cond = avalia(cw->condicao);
      bool cond_val = false;
      if (cond.tipo) {
        switch(cond.tipo->valor) {
        case Tipo::BOOL:  cond_val = cond.valor_bool;        break;
        case Tipo::INT:   cond_val = (cond.valor_int != 0);  break;
        case Tipo::FLOAT: cond_val = (cond.valor_float != 0.0f); break;
        }
      }
      if (!cond_val) break;
      if (executa(cw->corpo, ultimo_atrib)) return true;
    }
    return false;
  }

  cerr << "executa_cmd: comando desconhecido" << endl;
  return false;
}

bool Analisador::executa(const vector<Comando*>& cmds, ValorLiteral& ultimo_atrib) {
  for (size_t i = 0; i < cmds.size(); ++i) {
    if (executa_cmd(cmds[i], ultimo_atrib)) return true;
  }
  return false;
}

string Analisador::calcula_retorno(Funcao* f, const vector<ValorLiteral>& params) {
  env.clear();

  for (int i = 0; i < (int)f->parametros.size() && i < (int)params.size(); ++i) {
    env[f->parametros[i]] = params[i];
  }
  for (int i = (int)params.size(); i < (int)f->parametros.size(); ++i) {
    env[f->parametros[i]] = ValorLiteral(0);
  }
  for (size_t i = 0; i < f->locais.size(); ++i) {
    env[f->locais[i]] = ValorLiteral(0);
  }

  ValorLiteral ultimo;
  executa(f->comandos, ultimo);

  if (ultimo.tipo == NULL) return "nil";
  return formata(ultimo);
}
