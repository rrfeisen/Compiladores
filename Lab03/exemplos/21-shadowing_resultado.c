// A variável 'resultado' é declarada no escopo externo com valor 100.
// No bloco interno, uma NOVA 'resultado' é declarada (shadowing) e
// recebe o valor 1. O return está dentro do bloco interno, então
// deve retornar 1, e NÃO 100.
// Um analisador sem pilha de escopos que usa um map<string,*> plano
// vai sobrescrever a entrada de 'resultado' na atribuição interna,
// mas ao sair do bloco interno precisaria restaurar o valor externo.
// Como o return está dentro do bloco interno, o valor correto é 1.
int sombra(int x) {
  int resultado;
  resultado = x * 100;
  {
    int resultado;
    resultado = x * 1;
    return resultado;
  }
}
