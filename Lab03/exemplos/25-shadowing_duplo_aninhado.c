// SHADOWING — nível 5 de complexidade (máximo)
//
// Duas variáveis ('v' e 'w') são sombreadas simultaneamente
// em blocos aninhados. O resultado correto depende de identificar
// com precisão qual versão de cada variável está visível em cada ponto.
//
// Parâmetros: a, b  (ex: a=3, b=4)
//
// Escopo externo (corpo da função):
//   int v;  v = a * 10;    → v0 = 30
//   int w;  w = b + 1;     → w0 = 5
//
// Bloco nível 1:
//   int v;  v = w0 * 2;    → v1 = 10   (w ainda é w0 = 5)
//   (w0 = 5 ainda visível aqui)
//
//   Bloco nível 2:
//     int w;  w = v1 + v1; → w2 = 20   ('v' visível é v1 = 10)
//     return w;             → retorna w2 = 20
//
// Resultado correto com a=3, b=4: 20.
//
// Erros típicos de analisador plano:
//   — ao calcular w2, usa v0 (30) em vez de v1 (10) → retorna 60
//   — ao calcular v1, usa w_param (b=4) em vez de w0 (5) → v1=8, w2=16
int sombraDupla(int a, int b) {
  int v;
  v = a * 10;
  int w;
  w = b + 1;
  {
    int v;
    v = w * 2;
    {
      int w;
      w = v + v;
      return w;
    }
  }
}
