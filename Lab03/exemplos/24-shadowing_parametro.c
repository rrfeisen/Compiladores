// SHADOWING — nível 4 de complexidade
//
// O parâmetro 'x' da função é sombreado por uma variável local
// declarada dentro do bloco interno com o mesmo nome.
//
// Escopo base (parâmetro): x  (ex: x = 10)
// Escopo externo:
//   acum = x * 2       → acum = 20  (usa o parâmetro x)
// Bloco interno:
//   int x;             → nova variável 'x' local, sombra o parâmetro
//   x = acum + 9;      → x_local = 20 + 9 = 29  (acum vem do escopo externo)
//   int resultado;
//   resultado = x * x; → resultado = 29 * 29 = 841  (usa x_local)
//   return resultado;
//
// Resultado correto com x_param = 10: 841.
// Analisador com mapa plano: ao redeclarar 'x', perde o parâmetro;
// 'acum' ainda está acessível porque não foi sombreado.
// O erro típico seria calcular acum com x errado ou misturar os dois 'x'.
int sombraParametro(int x) {
  int acum;
  acum = x * 2;
  {
    int x;
    x = acum + 9;
    int resultado;
    resultado = x * x;
    return resultado;
  }
}
