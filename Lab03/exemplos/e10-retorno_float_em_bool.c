// ERRO DE TIPO: função declarada como boolean, mas retorna FLOAT
// Adicionalmente: variável float recebe resultado de operação INT * FLOAT
// sem problema em si, mas o retorno float em boolean é o erro principal.
// Tipo de retorno declarado: BOOL
// Tipo da expressão de retorno: FLOAT
boolean erroRetornoFloatEmBool(float taxa, int valor) {
  float desconto;
  desconto = valor * taxa;
  return desconto;
}
