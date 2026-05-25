// ERRO DE TIPO: função declarada como int, mas retorna BOOL_VAL
// Tipo de retorno declarado: INT
// Tipo da expressão de retorno: BOOL
int erroRetornoBool(int x) {
  boolean flag;
  flag = x < 0;
  return flag;
}
