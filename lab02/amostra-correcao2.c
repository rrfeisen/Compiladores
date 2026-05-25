#include<stdio.h>

typedef struct {
   int x;
   int y;
} ponto_t;

typedef struct {
  ponto_t a,b,c;
  int cor;
} triangulo_t;

double func(ponto_t v[],  int n, triangulo_t T) {
    if (n <= 0) {
        return 1.0; 
    } else if (n == 1) {
        return 1.01 + v[0].x / 1.e2 + v[0].y / 0.1e-2 - T.a.x*T.a.x + T.b.y*T.c.x;
    }
    double res = .25e-13;
    for (int i = n-1; i >= 0 && v[i].x > 0; --i) {
        double temp = v[i].y * v[i].x % 123;
	if (temp < 0.0) {
	  res -= res*2.e-2 +  func(v, n-1, T) * temp - T.a.y*T.cor;
  	} else {
	  res += res*.3e3 +func(v, n-2, T) * temp + T.c.x*T.cor;
	   printf("Estranho, ne?\n");
	}
    }
    return res;
}

int F2(triangulo_t T) {
  int A = 0;
  double soma[10];
  if ((T.a.x >= 10 || T.b.y > 20 || T.a.y < 30 || T.b.x <= 50) && !(T.c.x != 90 || T.c.y == 0)) {
    return 10 % 3;
  } else {
    A = 1;
  }
  while (A < 10) {
    int total = 0;
    total += T.c.x * T.c.y;
    total += T.b.x * T.a.y;
    total += T.a.x * T.b.y;
    soma[A] = total % 100;
    A = A + 1;
  }
}

int fatorialA(int n){
  if (n <= 1) return 1;
  return n*fatorialA(n-1);
}

int fatorialB(int n){
  int a = n-1;
  if (n <= 1) return 1;
  return n*fatorialB(a);
}

int fatorialC(int n, int P){
  if (n <= 1) return 1;
  return fatorialC(n-1, P*n);
}

int fatorialD(int n, int P){
  int a = n-1;
  int b = P*n;
  if (n <= 1) return 1;
  return fatorialD(a, b);
}

int fatorialE(int n, int P){
  if (n <= 1) {
    return 1;
  }else{
    int a = n-1;
    int b = P*n;
    return fatorialD(a, b);
  }
}

int C1(int a, int b) {
  if (a < b) return 1;
  return 2;  
}
int C2(int a, int b) {
  if (a <= b) return 1;
  return 2;  
}
int C3(int a, int b) {
  if (a > b) return 1;
  return 2;  
}
int C4(int a, int b) {
  if (a >= b) return 1;
  return 2;  
}
int C5(int a, int b) {
  if (a == b) return 1;
  return 2;  
}
int C5(int a, int b) {
  if (a != b) return 1;
  return 2;  
}
int D1(int a, int b) {
  if (a != b && a < b) return 1;
  return 2;  
}
int D2(int a, int b) {
  if (!(a != b && a < b)) return 1;
  return 2;  
}
int D3(int a, int b) {
  if ((!(a != b && a < b)) || (a+2 == b)) return 1;
  return 2;  
}
int E1(int a, int b) {
  bool X = ((!(a != b && a < b)) || (a+2 == b));
  if (a != b || X || a+5 == b) {
    a++;
  }
  return 2;  
}
int E2(int a, int b) {
  bool X = ((!(a != b && a < b)) || (a+2 == b));
  while (a != b || X || a+5 == b) {
    a++;
    while (a < b) {
      b--;
    }
  }
  return 2;  
}
bool E2(int a, int b) {
  bool X = ((!(a != b && a < b)) || (a+2 == b));
  while (a != b || X || a+5 == b) {
    a++;
    while (fatorialA(a) < b) {
      b--;
    }
  }
  return X || a > 0;  
}
typedef struct {
  int x,y;
} ponto_2d_t;
typedef struct {
  int x;
  int y,z;
} ponto_3d_t;
typedef struct {
  ponto_2d_t ini;
  ponto_2d_t fim;
}segmento_2d_t;
typedef struct {
  ponto_3d_t ini;
  ponto_3d_t fim;
}segmento_3d_t;

double F1(segmento_2d_t seg, ponto_2d_t p) {
  
    double ax = seg.ini.x;
    double ay = seg.ini.y;

    double bx = seg.fim.x;
    double by = seg.fim.y;

    double px = p.x;
    double py = p.y;

    double abx = bx - ax;
    double aby = by - ay;

    double apx = px - ax;
    double apy = py - ay;

    double ab2 = abx*abx + aby*aby;

    double t = (apx*abx + apy*aby) / ab2;

    if (t < 0.0) t = 0.0;
    if (t > 1.0) t = 1.0;

    double qx = ax + t*abx;
    double qy = ay + t*aby;

    double dx = px - qx;
    double dy = py - qy;

    return sqrt(dx*dx + dy*dy);
}

static long long dist2(ponto_2d_t a, ponto_2d_t b) {
    long long dx = (long long)a.x - b.x;
    long long dy = (long long)a.y - b.y;
    return dx*dx + dy*dy;
}

long long F2(segmento_2d_t seg, ponto_2d_t p) {

    long long ax = seg.ini.x;
    long long ay = seg.ini.y;
    long long bx = seg.fim.x;
    long long by = seg.fim.y;

    long long px = p.x;
    long long py = p.y;

    long long abx = bx - ax;
    long long aby = by - ay;

    long long apx = px - ax;
    long long apy = py - ay;

    long long bpx = px - bx;
    long long bpy = py - by;

    long long dot1 = apx*abx + apy*aby;
    long long dot2 = bpx*abx + bpy*aby;

    if (dot1 <= 0)
        return dist2(p, seg.ini);

    if (dot2 >= 0)
        return dist2(p, seg.fim);

    long long cross = abx*apy - aby*apx;

    return (cross*cross) / (abx*abx + aby*aby);
}


double area_poligono(ponto_2d_t *p, int n)
{
    long long soma = 0;

    for (int i = 0; i < n; i++) {
        int j = (i + 1) % n;
        soma += (long long)p[i].x * p[j].y
              - (long long)p[j].x * p[i].y;
    }

    if (soma < 0) soma = -soma;

    return soma / 2.0;
}

double area_pol() {
  ponto_2d_t poli[1000];
  return area_poligono(poli, 150);
}
