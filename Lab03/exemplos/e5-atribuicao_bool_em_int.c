// ERRO DE TIPO: variável declarada como int, mas recebe BOOL_VAL
// Tipo da variável: INT
// Tipo da expressão atribuída: BOOL
int erroAtribuicaoBool(int a) {
  int x;
  x = true;
  return x + a;
}
