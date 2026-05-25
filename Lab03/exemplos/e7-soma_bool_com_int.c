// ERRO DE TIPO: operador + aplicado a BOOL e INT
// Operadores aritméticos são válidos apenas entre INT ou entre FLOAT
// Tipo do operando esquerdo: BOOL; direito: INT
int erroSomaBool(boolean b, int x) {
  int resultado;
  resultado = b + x;
  return resultado;
}
