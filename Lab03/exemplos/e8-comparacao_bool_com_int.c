// ERRO DE TIPO: operador < aplicado a BOOL e INT
// O operador < é válido apenas entre INT ou entre FLOAT
// Tipo do operando esquerdo: BOOL; direito: INT
boolean erroMenorBool(boolean ativo, int limite) {
  boolean resultado;
  resultado = ativo < limite;
  return resultado;
}
