int triplo_aninhado() {
  int a;
  a = 1;
  {
    int b;
    b = a + 2;
    {
      int c;
      c = b * 3;
      {
        int d;
        d = c - 1;
        return d;
      }
    }
  }
}
