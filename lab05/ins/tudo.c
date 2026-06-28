int f() {
  int a;
  int b;
  int c;
  int d;
  
  a = 10;
  b = 25;
  c = 17;
  d = b*b-4*a*c;
  return d;
}
int f(int a, int b, int c) {
  int d;
  d = b*b-4*a*c;
  return d;
}
int f(int a, int b, int c) {
  int d;
  d = b*b-4*a*c;
  return d;
}
void F4() {
    int x = 10;
    x = x + 5;
}
void F5() {
    int x = 5;
    int y = 0;
    if (x > 2) {
        y = 1;
    }
}
void F6() {
    int x = 4;
    int y;
    if (x < 3) {
        y = 0;
    } else {
        y = 1;
    }
}
void F7() {
    int i = 0;
    int sum = 0;
    while (i < 5) {
        sum = sum + i;
        i = i + 1;
    }
}
void F8() {
    int i;
    int total = 0;
    for (i = 0; i < 4; i = i + 1) {
        total = total + i;
    }
}
void F9() {
    int r = square(5);
}
int fact(int n) {
    if (n <= 1) return 1;
    return n * fact(n - 1);
}
void F11() {
    int a = 1;
    int b = 0;
    int result = a && b;
}
void F12() {
    int x = 5;
    {
        int x = 10;
        x = x + 1;
    }
}
void F13(int a) {
    int x = a;
    x = x + 5;
}
void F14(int x) {
    int y = 0;
    if (x > 2) {
        y = 1;
    }
}
void F15(int x) {
    int y;
    if (x < 3) {
        y = 0;
    } else {
        y = 1;
    }
}
void F16(int limit) {
    int i = 0;
    int sum = 0;
    while (i < limit) {
        sum = sum + i;
        i = i + 1;
    }
}
void F18(int max) {
    int i;
    int total = 0;
    for (i = 0; i < max; i = i + 1) {
        total = total + i;
    }
}
void F19(int a) {
    int r = square(a);
}
void F20(int a, int b, int c) {
    int x = a + b + c;
    x = x + 5;
}
void F21(int x, int y, int z) {
    int r = 0;
    if (x > y + z) {
        r = 1;
    }
}
void F22(int a, int b, int c) {
    int flag;
    if (a < b + c) {
        flag = 0;
    } else {
        flag = 1;
    }
}
void F23(int start, int limit, int step) {
    int i = start;
    int total = 0;
    while (i < limit) {
        total = total + i;
        i = i + step;
    }
}
void F24(int from, int to, int step) {
    int i;
    int product = 1;
    for (i = from; i < to; i = i + step) {
        product = product * i;
    }
}
void F25(int val, int times, int factor) {
    int i = 0;
    int result = val;
    do {
        result = result * factor;
        i = i + 1;
    } while (i < times);
}
int power(int base, int exp) {
    if (exp == 0) return 1;
    return base * power(base, exp - 1);
}
void F27(int base, int exp, int offset) {
    int result = power(base, exp) + offset;
}
int F28(int n, int a, int b) {
    int sum = 0;                    /* 1. Variable declaration */

    if (n <= 0) {                   /* 2. If statement */
        return 0;
    }

    if (a > b) {                    /* 3. If-else statement */
        sum = a - b;
    } else {
        sum = b - a;
    }

    while (n > 0) {                 /* 4. While loop */
        sum = sum + n;
        n = n - 1;
    }

    for (n = 0; n < 3; n = n + 1) { /* 5. For loop */
        sum = sum + n;
    }

    if (sum > 10) {                 /* 6. Function call + 7. Recursion */
        return F50(sum / 3, a, b);  /* recursive call */
    }

    return sum;
}
int F50(int n, int a, int b) {
    int sum = 0;                                /* 1. Variable declaration */
    int temp = (a * b) - (a + b) / 2;

    if ((n % 2 == 0) && (a + b > 0)) {          /* 2. If statement */
        sum = (n * n) + (a * 2) - (b / 2);
    }

    if ((a * a + b * b) % 3 == 0) {             /* 3. If-else statement */
        sum = sum + (a + b) * (a - b);
    } else {
        sum = sum - ((a + b) * (a + b)) / (n + 1);
    }

    while (n > 1) {                             /* 4. While loop */
        sum = sum + (n * temp) - (n % 3) + (a + b);
        n = n - ((n % 2) + 1);
    }

    for (n = 0; n < 4; n = n + 1) {             /* 5. For loop */
        sum = sum + (n * n) - (a - b) * (n + 1);
    }

    if (sum > 100) {
        /* 6. Function call (7. Recursion) embedded in expression */
        int r = ((sum / 3) - F50(n + 1, a - 1, b + 2)) * 2 + (a % 3);
        sum = sum + r;
    }

    return sum + ((a * b) % (n + 2)) - temp;
}

int F50(int n, int a, int b) {
    int sum = 0;                               /* Level 0 */
    int temp = (a * b) - (a + b) / 2;         /* Level 0 */

    if ((n % 2 == 0) && (a + b > 0)) {        /* Level 1 */
        int x = (n * n) + (a * 2);             /* Level 2 */
        {
            int y = x - (b / 2);                /* Level 3 */
            sum = y + helper(a, b, n);          /* Function call embedded in expression */
        }
    }

    if ((a * a + b * b) % 3 == 0) {            /* Level 1 */
        int y = (a + b) * (a - b);             /* Level 2 */
        {
            sum = sum + y;                      /* Level 3 */
        }
    } else {
        int d = ((a + b) * (a + b)) / (n + 1); /* Level 2 */
        {
            sum = sum - d;                      /* Level 3 */
        }
    }

    while (n > 1) {                            /* Level 1 */
        int step = ((n % 3) + 1);              /* Level 2 */
        {
            sum = sum + (n * temp) - step + (a + b); /* Level 3 */
            n = n - ((n % 2) + 1);
        }
    }

    for (n = 0; n < 4; n = n + 1) {            /* Level 1 */
        int v = (n * n) - (a - b) * (n + 1);   /* Level 2 */
        {
            sum = sum + v;                      /* Level 3 */
        }
    }

    if (sum > 100) {                           /* Level 1 */
        {
            int r = ((sum / 3) - F50(n + 1, a - 1, b + 2)) * 2 + (a % 3); /* Recursive call *inside* expression */
            sum = sum + r;
        }
    }

    int tail = ((a * b) % (n + 2)) - temp;     /* Level 0 */
    sum = sum + tail;

    return sum;
}
