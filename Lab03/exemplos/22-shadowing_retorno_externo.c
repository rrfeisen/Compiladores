// SHADOWING — nível 2 de complexidade
//
// 'resultado' é declarada e atribuída no escopo externo (x * 100).
// O bloco interno declara sua PRÓPRIA 'resultado' e a altera (x * 1),
// mas o return está FORA do bloco interno, no escopo externo.
//
// O valor correto de retorno é o da 'resultado' EXTERNA: x * 100.
// Um analisador com mapa plano que não restaura o escopo ao sair
// do bloco retornará x * 1 — ERRADO.
//
// Com x = 5: resultado correto = 500. Resultado errado = 5.
int sombraRetornoExterno(int x) {
  int resultado;
  resultado = x * 100;
  {
    int resultado;
    resultado = x * 1;
  }
  return resultado;
}
