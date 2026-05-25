#include "Parser.hpp"

Parser::Parser(ifstream &arq_gramatica, ifstream &arq_tabela_lr1) :
  gram(arq_gramatica),
  tabela(arq_tabela_lr1) 
{}

string to_upper(string str) {
  transform(str.begin(), str.end(),str.begin(), ::toupper);
  return str;
}

string meu_getline(istream &input) {
  char s[2001];
  input.get(s, 2000, '\n');
  string resultado(s);
  return resultado;
}

void le_terminal(istream &input, string &tipo_terminal, string &texto_terminal) {
  string x;
  input >> x;
  //  tipo_terminal = to_upper(x); // Se voce estiver usando
  // case-insensitive nao tratado no lexico.  Nesse caso, voce nao
  // pode usar nome de nao terminal maiusculo que coincida com nome de
  // terminal minusculo
  tipo_terminal = x;
  char espaco;
  input.get(espaco);
  texto_terminal = meu_getline(input);  
}

Arvore_parse Parser::executa_parse(istream &input) {
  int estado_atual = 0;
  string tipo_terminal_lookahead;
  string texto_terminal;
  le_terminal(input, tipo_terminal_lookahead, texto_terminal);
  do {
    cerr << "Estado:" << estado_atual << " Lookahead:" <<  tipo_terminal_lookahead << endl;
    Transicao t = tabela.Tab[estado_atual][tipo_terminal_lookahead];
    cerr << "Transicao:"<<t.impressao() << endl;
    switch(t.tipo) {
    case 0: return Arvore_parse(NULL); break; //erro
    case 1: // terminal
      {
        No_arv_parse * ap_no = new No_arv_parse;
        ap_no->simb = tipo_terminal_lookahead;
	ap_no->dado_extra = texto_terminal;
	//cerr << "Empilhando:" << estado_atual << ":";
	//	No_arv_parse::debug_no(pilha.top().first);
        pilha.push(make_pair(ap_no,estado_atual));

        estado_atual = t.prox_estado;
	le_terminal(input, tipo_terminal_lookahead, texto_terminal);
      }
      break;
    case 2: //goto
      cerr << "ERRO goto em lookahead."<<endl;
      return Arvore_parse(NULL);
    case 3: //reducao
      {
        Regra r = gram.R[t.reducao];
	//	r.debug();
        No_arv_parse * ap_no = new No_arv_parse;
        ap_no->simb = r.esq;
        ap_no->regra = t.reducao;
        ap_no->filhos.resize(r.dir.size());
        int estado = estado_atual; 
	//	cerr << "BLOCO DE EMPILHAMENTO" << endl;
        for(int i = 0; i < r.dir.size(); ++i) {
	  //	  	  cerr << "DesEmpilha:" <<  pilha.top().second << ":";
	  //	  No_arv_parse::debug_no(pilha.top().first);
          ap_no->filhos[r.dir.size() - i - 1] = pilha.top().first;
          estado = pilha.top().second;
          pilha.pop();
        }
	//	cerr << "Empilhando:" << estado << ":";
	//	No_arv_parse::debug_no(ap_no);
        pilha.push(make_pair(ap_no,estado));
	//	cerr << "FIM BLOCO DE EMPILHAMENTO" << endl;
        Transicao go_to = tabela.Tab[estado][r.esq];
        if (go_to.tipo != 2) {
          cerr << "ausencia de goto apos reducao" << endl;
          return Arvore_parse(NULL);
        }
        estado_atual = go_to.prox_estado;
      }
      break;
    case 4:
            cerr << "   QUASE FIM PARSE" << endl;
            No_arv_parse::debug_no(pilha.top().first);
      cerr << "   FIM PARSE" << endl;
      return Arvore_parse(pilha.top().first);
    default: 
      cerr << "Codigo invalido" << endl;
      return Arvore_parse(NULL);
    }
  }while(!input.eof());
  cerr << "   ------------------------------------------------------------------" << endl;
  cerr << "   PARSE Terminou por fim de arquivo. Faltou o $ no final da entrada?" << endl;
  cerr << "   ------------------------------------------------------------------" << endl;
  return Arvore_parse(NULL);
}

