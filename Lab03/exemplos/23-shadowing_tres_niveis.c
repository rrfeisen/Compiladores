// SHADOWING — nível 3 de complexidade
//
// Três níveis de aninhamento, cada um com sua própria 'v'.
// Nível 0 (externo): v = x * 2        (com x=2: v0 = 4)
// Nível 1 (médio):   v = v_externo * 3  — lê 'v' do escopo pai!
//                    (v1 = 4 * 3 = 12)
// Nível 2 (interno): v = v_medio + 1  — lê 'v' do escopo pai imediato!
//                    (v2 = 12 + 1 = 13)
// return está no nível 2, retorna v2.
//
// Resultado correto com x = 2: 13.
// Um analisador plano não distingue qual 'v' ler em cada nível.
int sombraTresNiveis(int x) {
  int v;
  v = x * 2;
  {
    int v;
    v = v * 3;
    {
      int v;
      v = v + 1;
      return v;
    }
  }
}
