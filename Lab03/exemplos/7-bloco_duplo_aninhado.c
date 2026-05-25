int calcular() {
  int a;
  a = 5;
  {
    int b;
    b = a * 3;
    {
      int c;
      c = b + 1;
      return c;
    }
  }
}
