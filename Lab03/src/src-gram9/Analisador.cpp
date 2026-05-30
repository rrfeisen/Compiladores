/*
 * Analisador.cpp — gramática 9 (Icon)
 *
 * Regras semânticas (baseadas em C, conforme enunciado):
 *  - int op float  => float  (promoção implícita)
 *  - float op int  => float
 *  - int op int    => int
 *  - float op float=> float
 *  - ! só em bool  (gera erro se operando não for bool)
 *  - && e ||  só entre bool (gera erro caso contrário)
 *  - comparações   => bool
 *  - bool op arit  => ERRO
 */

#include "Analisador.hpp"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <stdexcept>
using namespace std;

// ----------------------------------------------------------------
// formata — saída final conforme enunciado
// ----------------------------------------------------------------
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

// ----------------------------------------------------------------
// converte — promoção int->float
// ----------------------------------------------------------------
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

// ----------------------------------------------------------------
// avalia — avalia recursivamente uma Expressao
// ----------------------------------------------------------------
ValorLiteral Analisador::avalia(Expressao* e) {
  if (e == NULL) return ValorLiteral();

  // --- literal ---
  if (ExpressaoValor* ev = dynamic_cast<ExpressaoValor*>(e)) {
    if (ev->valor == NULL) return ValorLiteral();
    return *(ev->valor);
  }

  // --- variável ---
  if (ExpressaoVariavel* evar = dynamic_cast<ExpressaoVariavel*>(e)) {
    string nome = evar->nome ? evar->nome->nome : "";
    if (env.find(nome) != env.end()) return env[nome];
    cerr << "ERRO: variavel nao declarada '" << nome << "'" << endl;
    return ValorLiteral();
  }

  // --- atribuição (:= , +:= , -:=) ---
  if (ExpressaoAtribuicao* ea = dynamic_cast<ExpressaoAtribuicao*>(e)) {
    // lvalue deve ser variável
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
      // promoção
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

  // --- NOT lógico ---
  if (ExpressaoNot* en = dynamic_cast<ExpressaoNot*>(e)) {
    ValorLiteral op = avalia(en->operando);
    if (op.tipo == NULL || op.tipo->valor != Tipo::BOOL) {
      cerr << "ERRO de tipo: operador '!' aplicado a nao-booleano ("
           << (op.tipo ? op.tipo->nome() : "nil") << ")" << endl;
    }
    return ValorLiteral(!op.valor_bool);
  }

  // --- OR ---
  if (ExpressaoOr* eo = dynamic_cast<ExpressaoOr*>(e)) {
    ValorLiteral l = avalia(eo->esquerda);
    ValorLiteral r = avalia(eo->direita);
    if (!l.tipo || l.tipo->valor != Tipo::BOOL ||
        !r.tipo || r.tipo->valor != Tipo::BOOL) {
      cerr << "ERRO de tipo: '||' requer operandos booleanos ("
           << (l.tipo ? l.tipo->nome() : "nil") << ", "
           << (r.tipo ? r.tipo->nome() : "nil") << ")" << endl;
    }
    return ValorLiteral(l.valor_bool || r.valor_bool);
  }

  // --- AND ---
  if (ExpressaoAnd* ea = dynamic_cast<ExpressaoAnd*>(e)) {
    ValorLiteral l = avalia(ea->esquerda);
    ValorLiteral r = avalia(ea->direita);
    if (!l.tipo || l.tipo->valor != Tipo::BOOL ||
        !r.tipo || r.tipo->valor != Tipo::BOOL) {
      cerr << "ERRO de tipo: '&&' requer operandos booleanos ("
           << (l.tipo ? l.tipo->nome() : "nil") << ", "
           << (r.tipo ? r.tipo->nome() : "nil") << ")" << endl;
    }
    return ValorLiteral(l.valor_bool && r.valor_bool);
  }

  // --- comparações ---
  if (ExpressaoCmp* ec = dynamic_cast<ExpressaoCmp*>(e)) {
    ValorLiteral l = avalia(ec->esquerda);
    ValorLiteral r = avalia(ec->direita);

    // Não permite bool em comparações < <= > >=
    if (ec->op != "==" && ec->op != "!=") {
      if ((l.tipo && l.tipo->valor == Tipo::BOOL) ||
          (r.tipo && r.tipo->valor == Tipo::BOOL)) {
        cerr << "ERRO de tipo: operador '" << ec->op
             << "' nao se aplica a booleanos" << endl;
      }
    }

    // Promoção int/float
    bool usa_float = (l.tipo && l.tipo->valor == Tipo::FLOAT) ||
                     (r.tipo && r.tipo->valor == Tipo::FLOAT);
    if (usa_float) {
      float lf = l.tipo && l.tipo->valor == Tipo::FLOAT ? l.valor_float : (float)l.valor_int;
      float rf = r.tipo && r.tipo->valor == Tipo::FLOAT ? r.valor_float : (float)r.valor_int;
      bool res = false;
      if      (ec->op == "==") res = (lf == rf);
      else if (ec->op == "!=") res = (lf != rf);
      else if (ec->op == "<")  res = (lf <  rf);
      else if (ec->op == "<=") res = (lf <= rf);
      else if (ec->op == ">")  res = (lf >  rf);
      else if (ec->op == ">=") res = (lf >= rf);
      return ValorLiteral(res);
    } else {
      // int ou bool
      int li = (l.tipo && l.tipo->valor == Tipo::BOOL) ? (int)l.valor_bool : l.valor_int;
      int ri = (r.tipo && r.tipo->valor == Tipo::BOOL) ? (int)r.valor_bool : r.valor_int;
      bool res = false;
      if      (ec->op == "==") res = (li == ri);
      else if (ec->op == "!=") res = (li != ri);
      else if (ec->op == "<")  res = (li <  ri);
      else if (ec->op == "<=") res = (li <= ri);
      else if (ec->op == ">")  res = (li >  ri);
      else if (ec->op == ">=") res = (li >= ri);
      return ValorLiteral(res);
    }
  }

  // --- aritméticos (soma, sub, mul, div, mod) ---
  // helper lambda não disponível em C++03 — usamos macro local
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

// ----------------------------------------------------------------
// executa_cmd
// ----------------------------------------------------------------
bool Analisador::executa_cmd(Comando* c, ValorLiteral& ultimo_atrib) {
  if (c == NULL) return false;

  // Atribuição
  if (ComandoAtribuicao* ca = dynamic_cast<ComandoAtribuicao*>(c)) {
    ultimo_atrib = avalia(ca->expressao);
    return false;
  }

  // Expressão genérica (pode ser atribuição também se veio de ComandoExpr)
  if (ComandoExpr* ce = dynamic_cast<ComandoExpr*>(c)) {
    ValorLiteral v = avalia(ce->expressao);
    // Se for atribuição, registra como último atribuído
    if (dynamic_cast<ExpressaoAtribuicao*>(ce->expressao)) {
      ultimo_atrib = v;
    }
    return false;
  }

  // Return
  if (ComandoRetorno* cr = dynamic_cast<ComandoRetorno*>(c)) {
    ultimo_atrib = avalia(cr->expressao);
    return true; // sinaliza return
  }

  // If
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

  // While
  if (ComandoWhile* cw = dynamic_cast<ComandoWhile*>(c)) {
    int guard = 100000; // limite de iterações
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

// ----------------------------------------------------------------
// executa — lista de comandos
// ----------------------------------------------------------------
bool Analisador::executa(const vector<Comando*>& cmds, ValorLiteral& ultimo_atrib) {
  for (auto c : cmds) {
    if (executa_cmd(c, ultimo_atrib)) return true;
  }
  return false;
}

// ----------------------------------------------------------------
// calcula_retorno — ponto de entrada público
// ----------------------------------------------------------------
string Analisador::calcula_retorno(Funcao* f, const vector<ValorLiteral>& params) {
  env.clear();

  // Inicializa parâmetros
  for (int i = 0; i < (int)f->parametros.size() && i < (int)params.size(); ++i) {
    env[f->parametros[i]] = params[i];
  }
  // Parâmetros não fornecidos ficam como INT 0
  for (int i = (int)params.size(); i < (int)f->parametros.size(); ++i) {
    env[f->parametros[i]] = ValorLiteral(0);
  }

  // Inicializa locais como INT 0
  for (const string& nome : f->locais) {
    env[nome] = ValorLiteral(0);
  }

  ValorLiteral ultimo;
  executa(f->comandos, ultimo);

  if (ultimo.tipo == NULL) return "nil";
  return formata(ultimo);
}
