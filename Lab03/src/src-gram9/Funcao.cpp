#include "Funcao.hpp"
#include "../debug-util.hpp"
#include <iostream>

Funcao::Funcao() {}

Funcao* Funcao::extrai_funcao(No_arv_parse *no) {
    if (!no) return NULL;
    
    // 10) proc_decl -> KW_PROCEDURE ID LPAREN param_list RPAREN opt_local stmt_list KW_END
    if (no->regra == 10) {
        Funcao* res = new Funcao();
        res->tipo_retorno = new Tipo(Tipo::INT); 
        res->nome_funcao = ID::extrai_ID(no->filhos[1]);
        res->parametros = Variavel::extrai_lista_parametros(no->filhos[3]);
        res->comandos = Comando::extrai_lista_comandos(no->filhos[6]);
        return res;
    }
    
    // Se não for a regra 10, continua a mergulhar na árvore
    for (size_t i = 0; i < no->filhos.size(); i++) {
        if (Funcao* f = extrai_funcao(no->filhos[i])) return f;
    }
    return NULL;
}

void debug_comandos(const vector<Comando*> &coms, int tab) {
    for (size_t ic = 0; ic < coms.size(); ++ic) {
        if (coms[ic]) coms[ic]->debug_com_tab(tab+1);
    }  
}

void Funcao::debug() {
    cerr << "Funcao..." << endl;
}
