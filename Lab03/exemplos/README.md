# Exemplos para gramatica-9

Os arquivos `*.tokens` estao no formato lido por `Parser::executa_parse`: um terminal por linha, no formato `TERMINAL lexema`.

Use, por exemplo:

```sh
make run < exemplos/0-retorna_soma_int.tokens
```

## 0-retorna_soma_int.tokens

Equivalente aproximado:

```c
int soma(int a, int b) {
  int resultado;
  resultado = a + b * 2;
  return resultado;
}
```

## 1-retorna_float.tokens

Equivalente aproximado:

```c
float media(float a, float b) {
  float m;
  m = (a + b) / 2.0;
  return m;
}
```

## 2-retorna_boolean.tokens

Equivalente aproximado:

```c
boolean testa(boolean ativo, int x) {
  boolean ok;
  ok = !false == (x < 10);
  return ok;
}
```

## 3-bloco_aninhado.tokens

Equivalente aproximado:

```c
int principal() {
  int x;
  x = 1;
  {
    int y;
    y = x + 2;
    return y;
  }
}
```
