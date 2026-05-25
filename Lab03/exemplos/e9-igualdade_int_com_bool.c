// ERRO DE TIPO: operador == entre INT e BOOL
// Comparação de igualdade requer operandos do mesmo tipo
// Tipo do operando esquerdo: INT; direito: BOOL
boolean erroIgualdadeTiposMistos(int x, boolean b) {
  boolean resultado;
  resultado = x == b;
  return resultado;
}
